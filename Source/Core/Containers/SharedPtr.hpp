/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Atomic.hpp>
#include <Core/Concepts.hpp>
#include <Core/Memory.hpp>
#include <Core/TypeTraits.hpp>

namespace Mach::Core {
	enum class SharedType { NonAtomic, Atomic };

	template <SharedType Type>
	class SharedCounter;

	class SharedPtrFromThisBase {};

	template <typename Base, SharedType Type>
	class WeakPtr;

	template <>
	class SharedCounter<SharedType::NonAtomic> {
	public:
		SharedCounter() = default;

		MACH_ALWAYS_INLINE u32 strong() const { return m_strong; }
		MACH_ALWAYS_INLINE u32 weak() const { return m_weak; }

		MACH_ALWAYS_INLINE u32 add_strong() const {
			m_strong += 1;
			return m_strong - 1;
		}
		MACH_ALWAYS_INLINE u32 remove_strong() const {
			m_strong -= 1;
			return m_strong + 1;
		}

		MACH_ALWAYS_INLINE u32 add_weak() const {
			m_weak += 1;
			return m_weak - 1;
		}
		MACH_ALWAYS_INLINE u32 remove_weak() const {
			m_weak -= 1;
			return m_weak + 1;
		}

	private:
		mutable u32 m_strong = 1;
		mutable u32 m_weak = 0;
	};

	template <>
	class SharedCounter<SharedType::Atomic> {
	public:
		SharedCounter() = default;

		MACH_ALWAYS_INLINE u32 strong() const { return m_strong.load(Order::Acquire); }
		MACH_ALWAYS_INLINE u32 weak() const { return m_weak.load(Order::Acquire); }

		MACH_ALWAYS_INLINE u32 add_strong() const { return m_strong.fetch_add(1, Order::AcqRel); }
		MACH_ALWAYS_INLINE u32 remove_strong() const { return m_strong.fetch_sub(1, Order::AcqRel); }

		MACH_ALWAYS_INLINE u32 add_weak() const { return m_weak.fetch_add(1, Order::AcqRel); }
		MACH_ALWAYS_INLINE u32 remove_weak() const { return m_weak.fetch_sub(1, Order::AcqRel); }

	private:
		Atomic<u32> m_strong{ 1 };
		Atomic<u32> m_weak{ 0 };
	};

	template <typename Base, SharedType Type>
	class SharedPtr {
	public:
		using Counter = SharedCounter<Type>;

		explicit SharedPtr() : m_counter(nullptr), m_base(nullptr) {}

		template <typename... Args>
		static MACH_ALWAYS_INLINE SharedPtr<Base, Type> create(Args&&... args)
			requires ConstructibleFrom<Base, Args...>
		{
			struct Combined {
				SharedCounter<Type> counter;
				Base base;
			};

			const auto layout = Memory::Layout::single<Combined>();
			Combined* ptr = Memory::emplace<Combined>(
				Memory::alloc(layout),
				Combined{
					.counter = SharedCounter<Type>{},
					.base = Base{ Mach::forward<Args>(args)... },
				});

			auto* counter = &ptr->counter;
			auto* base = &ptr->base;

			const auto result = SharedPtr<Base, Type>(counter, base);
			if constexpr (is_base_of<SharedPtrFromThisBase, Base>) {
				base->m_this = result.downgrade();
			}

			return result;
		}

		SharedPtr(const SharedPtr& copy) noexcept : m_counter(copy.m_counter), m_base(copy.m_base) {
			auto& c = counter();
			c.add_strong();
		}
		template <typename Derived = Base>
		SharedPtr(const SharedPtr<Derived, Type>& copy) noexcept : m_counter(copy.m_counter)
																 , m_base(copy.m_base) {
			auto& c = counter();
			c.add_strong();
		}
		SharedPtr& operator=(const SharedPtr& copy) noexcept {
			m_counter = copy.m_counter;
			m_base = copy.m_base;

			auto& c = counter();
			c.add_strong();

			return *this;
		}
		template <typename Derived = Base>
		SharedPtr& operator=(const SharedPtr<Derived, Type>& copy) noexcept {
			m_counter = copy.m_counter;
			m_base = copy.m_base;

			auto& c = counter();
			c.add_strong();

			return *this;
		}
		template <typename Derived = Base>
		SharedPtr(SharedPtr<Derived, Type>&& move) noexcept
			requires DerivedFrom<Derived, Base> || SameAs<Derived, Base>
			: m_counter(move.m_counter)
			, m_base(move.m_base) {

			move.m_counter = nullptr;
			move.m_base = nullptr;
		}
		template <typename Derived = Base>
		SharedPtr& operator=(SharedPtr<Derived, Type>&& move) noexcept
			requires DerivedFrom<Derived, Base> || SameAs<Derived, Base>
		{
			this->~SharedPtr();

			m_counter = move.m_counter;
			m_base = move.m_base;

			move.m_counter = nullptr;
			move.m_base = nullptr;

			return *this;
		}
		~SharedPtr() {
			if (m_counter) {
				auto& c = counter();

				// Decrement the strong count
				const auto strong_count = c.remove_strong();
				const auto weak_count = c.weak();

				// If there are no strong references deconstruct the object
				if (strong_count == 1) {
					m_base->~Base();

					// Base that are derived from SharedFromThis will have a weak ptr to itself. Free when weak ptr is 1
					// to account for this
					if constexpr (is_base_of<SharedPtrFromThisBase, Base>) {
						if (weak_count == 1) {
							Memory::free(m_counter);
						}
					}
					// Free the memory if we have no weak references
					else {
						if (weak_count == 0) {
							Memory::free(m_counter);
						}
					}
				}

				m_counter = nullptr;
				m_base = nullptr;
			}
		}

		MACH_ALWAYS_INLINE WeakPtr<Base, Type> downgrade() const {
			auto& c = counter();
			c.add_weak();
			return WeakPtr<Base, Type>{ m_counter, m_base };
		}

		// Non Atomic Accessors
		MACH_ALWAYS_INLINE explicit operator Base*() const
			requires(Type == SharedType::NonAtomic)
		{
			return &value();
		}
		MACH_ALWAYS_INLINE explicit operator Base&() const
			requires(Type == SharedType::NonAtomic)
		{
			return value();
		}
		MACH_ALWAYS_INLINE Base* operator->() const
			requires(Type == SharedType::NonAtomic)
		{
			return &value();
		}
		MACH_ALWAYS_INLINE Base& operator*() const
			requires(Type == SharedType::NonAtomic)
		{
			return value();
		}

		// Atomic Accessors are const only
		MACH_NO_DISCARD Base& unsafe_get_mut() const
			requires(Type == SharedType::Atomic)
		{
			return value();
		}

		MACH_ALWAYS_INLINE explicit operator Base const*() const
			requires(Type == SharedType::Atomic)
		{
			return &value();
		}
		MACH_ALWAYS_INLINE explicit operator Base const&() const
			requires(Type == SharedType::Atomic)
		{
			return value();
		}
		MACH_ALWAYS_INLINE Base const* operator->() const
			requires(Type == SharedType::Atomic)
		{
			return &value();
		}
		MACH_ALWAYS_INLINE Base const& operator*() const
			requires(Type == SharedType::Atomic)
		{
			return value();
		}

		MACH_ALWAYS_INLINE bool is_valid() const { return m_counter != nullptr; }

		MACH_ALWAYS_INLINE bool is_null() const { return m_counter != nullptr; }

		MACH_NO_DISCARD MACH_ALWAYS_INLINE u32 strong() const { return m_counter != nullptr ? counter().strong() : 0; }
		MACH_NO_DISCARD MACH_ALWAYS_INLINE u32 weak() const { return m_counter != nullptr ? counter().weak() : 0; }

	private:
		explicit SharedPtr(Counter* counter, Base* base) : m_counter(counter), m_base(base) {}

		template <typename, SharedType>
		friend class SharedPtr;

		template <typename, SharedType>
		friend class WeakPtr;

		MACH_ALWAYS_INLINE Counter const& counter() const { return *m_counter; }
		MACH_ALWAYS_INLINE Base& value() const { return *m_base; }

		Counter* m_counter = nullptr;
		Base* m_base = nullptr;
	};

	template <typename Base, SharedType Type>
	class WeakPtr {
	public:
		using Counter = SharedCounter<Type>;

		WeakPtr() = default;

		template <typename Derived = Base>
		WeakPtr(const WeakPtr<Derived, Type>& copy) noexcept
			requires DerivedFrom<Derived, Base>
			: m_counter(copy.m_counter)
			, m_base(copy.m_base) {
			auto& c = counter();
			c.add_weak();
		}
		template <typename Derived = Base>
		WeakPtr& operator=(const WeakPtr<Derived, Type>& copy) noexcept
			requires DerivedFrom<Derived, Base>
		{
			this->~WeakPtr();

			m_counter = copy.m_counter;
			m_base = copy.m_base;

			auto& c = counter();
			c.add_weak();

			return *this;
		}
		template <typename Derived = Base>
		WeakPtr(WeakPtr<Derived, Type>&& move) noexcept
			requires DerivedFrom<Derived, Base>
			: m_counter(move.m_counter)
			, m_base(move.m_base) {
			move.m_counter = nullptr;
			move.m_base = nullptr;
		}
		template <typename Derived = Base>
		WeakPtr& operator=(WeakPtr<Derived, Type>&& m) noexcept
			requires DerivedFrom<Derived, Base>
		{
			this->~WeakPtr();

			m_counter = m.m_counter;
			m_base = m.m_base;
			m.m_counter = nullptr;
			m.m_base = nullptr;

			return *this;
		}
		~WeakPtr() {
			if (m_counter) {
				auto& c = counter();

				const auto strong_count = c.strong();
				const auto weak_count = c.remove_weak();

				if (strong_count == 0 && weak_count == 0) {
					Memory::free(m_counter);
					m_counter = nullptr;
				}
			}
		}

		MACH_NO_DISCARD Option<SharedPtr<Base, Type>> upgrade() const {
			auto& c = counter();
			const auto strong_count = c.strong();
			if (strong_count > 0) {
				c.add_strong();
				return SharedPtr<Base, Type>{ m_counter, m_base };
			}
			return nullopt;
		}

		MACH_NO_DISCARD MACH_ALWAYS_INLINE u32 strong() const { return m_counter != nullptr ? counter().strong() : 0; }
		MACH_NO_DISCARD MACH_ALWAYS_INLINE u32 weak() const { return m_counter != nullptr ? counter().weak() : 0; }

		MACH_ALWAYS_INLINE bool is_valid() const { return m_counter != nullptr; }
		MACH_ALWAYS_INLINE bool is_null() const { return m_counter != nullptr; }

	private:
		MACH_ALWAYS_INLINE Counter const& counter() const { return *m_counter; }

		explicit WeakPtr(Counter* counter, Base* base) : m_counter(counter), m_base(base) {}

		template <typename Derived, SharedType>
		friend class SharedPtr;

		template <typename Derived, SharedType>
		friend class WeakPtr;

		Counter* m_counter = nullptr;
		Base* m_base = nullptr;
	};

	template <typename T, SharedType Type>
	class SharedPtrFromThis : SharedPtrFromThisBase {
	public:
		using Counter = SharedCounter<Type>;

		MACH_NO_DISCARD MACH_ALWAYS_INLINE SharedPtr<T, Type> to_shared() const {
			return m_this.as_const_ref().unwrap().upgrade().unwrap();
		}

	private:
		template <typename Derived, SharedType>
		friend class SharedPtr;

		Option<WeakPtr<T, Type>> m_this = nullopt;
	};
} // namespace Mach::Core

namespace Mach {
	template <typename T>
	using SharedPtr = Core::SharedPtr<T, Core::SharedType::Atomic>;

	template <typename T>
	using SharedPtrFromThis = Core::SharedPtrFromThis<T, Core::SharedType::Atomic>;

	template <typename T>
	using WeakPtr = Core::WeakPtr<T, Core::SharedType::Atomic>;
} // namespace Mach
