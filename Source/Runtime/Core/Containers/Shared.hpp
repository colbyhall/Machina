/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Atomic.hpp>
#include <Core/Concepts.hpp>
#include <Core/Memory.hpp>

namespace Grizzly::Core {
	enum class SharedType { NonAtomic, Atomic };

	template <SharedType Type>
	class SharedCounter;

	class SharedFromThisBase {};

	template <typename Base, SharedType Type>
	class Weak;

	template <>
	class SharedCounter<SharedType::NonAtomic> {
	public:
		SharedCounter() = default;

		GRIZZLY_ALWAYS_INLINE u32 strong() const { return m_strong; }
		GRIZZLY_ALWAYS_INLINE u32 weak() const { return m_weak; }

		GRIZZLY_ALWAYS_INLINE u32 add_strong() const {
			m_strong += 1;
			return m_strong - 1;
		}
		GRIZZLY_ALWAYS_INLINE u32 remove_strong() const {
			m_strong -= 1;
			return m_strong + 1;
		}

		GRIZZLY_ALWAYS_INLINE u32 add_weak() const {
			m_weak += 1;
			return m_weak - 1;
		}
		GRIZZLY_ALWAYS_INLINE u32 remove_weak() const {
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

		GRIZZLY_ALWAYS_INLINE u32 strong() const { return m_strong.load(Order::Acquire); }
		GRIZZLY_ALWAYS_INLINE u32 weak() const { return m_weak.load(Order::Acquire); }

		GRIZZLY_ALWAYS_INLINE u32 add_strong() const { return m_strong.fetch_add(1, Order::AcqRel); }
		GRIZZLY_ALWAYS_INLINE u32 remove_strong() const { return m_strong.fetch_sub(1, Order::AcqRel); }

		GRIZZLY_ALWAYS_INLINE u32 add_weak() const { return m_weak.fetch_add(1, Order::AcqRel); }
		GRIZZLY_ALWAYS_INLINE u32 remove_weak() const { return m_weak.fetch_sub(1, Order::AcqRel); }

	private:
		Atomic<u32> m_strong{ 1 };
		Atomic<u32> m_weak{ 0 };
	};

	template <typename Base, SharedType Type>
	class Shared {
	public:
		using Counter = SharedCounter<Type>;

		explicit Shared()
			requires DefaultInitializable<Base>
			: Shared{ Base{} } {}

		template <typename... Args>
		static GRIZZLY_ALWAYS_INLINE Shared<Base, Type> create(Args&&... args)
			requires ConstructibleFrom<Base, Args...>
		{
			return Shared<Base, Type>{ Grizzly::move(Base{ Grizzly::forward<Args>(args)... }) };
		}

		Shared(const Shared& copy) noexcept : m_counter(copy.m_counter), m_base(copy.m_base) {
			auto& c = counter();
			c.add_strong();
		}
		Shared& operator=(const Shared& copy) noexcept {
			m_counter = copy.m_counter;
			m_base = copy.m_base;

			auto& c = counter();
			c.add_strong();

			return *this;
		}
		template <typename Derived = Base>
		Shared(Shared<Derived, Type>&& move) noexcept
			requires DerivedFrom<Derived, Base> || SameAs<Derived, Base>
			: m_counter(move.m_counter)
			, m_base(move.m_base) {
			move.m_counter = nullptr;
			move.m_base = nullptr;
		}
		template <typename Derived = Base>
		Shared& operator=(Shared<Derived, Type>&& move) noexcept
			requires DerivedFrom<Derived, Base> || SameAs<Derived, Base>
		{
			m_counter = move.m_counter;
			m_base = move.m_base;
			move.m_counter = nullptr;
			move.m_base = nullptr;

			return *this;
		}
		~Shared() {
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
					if constexpr (is_base_of<SharedFromThisBase, Base>) {
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

					m_counter = nullptr;
					m_base = nullptr;
				}
			}
		}

		GRIZZLY_ALWAYS_INLINE Weak<Base, Type> downgrade() const {
			auto& c = counter();
			c.add_weak();
			return Weak<Base, Type>{ m_counter, m_base };
		}

		// Accessors
		GRIZZLY_ALWAYS_INLINE explicit operator Base*() { return &value(); }
		GRIZZLY_ALWAYS_INLINE explicit operator Base*() const { return &value(); }
		GRIZZLY_ALWAYS_INLINE explicit operator Base&() { return value(); }
		GRIZZLY_ALWAYS_INLINE explicit operator Base&() const { return value(); }
		GRIZZLY_ALWAYS_INLINE Base* operator->() { return &value(); }
		GRIZZLY_ALWAYS_INLINE Base* operator->() const { return &value(); }
		GRIZZLY_ALWAYS_INLINE Base& operator*() { return value(); }
		GRIZZLY_ALWAYS_INLINE Base& operator*() const { return value(); }

		GRIZZLY_NO_DISCARD GRIZZLY_ALWAYS_INLINE u32 strong() const {
			return m_counter != nullptr ? counter().strong() : 0;
		}
		GRIZZLY_NO_DISCARD GRIZZLY_ALWAYS_INLINE u32 weak() const {
			return m_counter != nullptr ? counter().weak() : 0;
		}

	private:
		explicit Shared(Counter* counter, Base* base) : m_counter(counter), m_base(base) {}
		explicit Shared(Base&& base)
			requires Movable<Base>
		{
			struct Combined {
				SharedCounter<Type> counter;
				Base base;
			};

			const auto layout = Memory::Layout::single<Combined>();
			Combined* ptr =
				Memory::emplace<Combined>(Memory::alloc(layout), SharedCounter<Type>{}, Grizzly::forward<Base>(base));

			m_counter = &ptr->counter;
			m_base = &ptr->base;
			if constexpr (is_base_of<SharedFromThisBase, Base>) {
				m_base->m_this = downgrade();
			}
		}

		template <typename, SharedType>
		friend class Shared;

		template <typename, SharedType>
		friend class Weak;

		GRIZZLY_ALWAYS_INLINE Counter const& counter() const { return *m_counter; }
		GRIZZLY_ALWAYS_INLINE Base& value() const { return *m_base; }

		Counter* m_counter = nullptr;
		Base* m_base = nullptr;
	};

	template <typename Base, SharedType Type>
	class Weak {
	public:
		using Counter = SharedCounter<Type>;

		Weak() = default;

		template <typename Derived = Base>
		Weak(const Weak<Derived, Type>& copy) noexcept
			requires DerivedFrom<Derived, Base>
			: m_counter(copy.m_counter)
			, m_base(copy.m_base) {
			auto& c = counter();
			c.add_weak();
		}
		template <typename Derived = Base>
		Weak& operator=(const Weak<Derived, Type>& copy) noexcept
			requires DerivedFrom<Derived, Base>
		{
			this->~Weak();

			m_counter = copy.m_counter;
			m_base = copy.m_base;

			auto& c = counter();
			c.add_weak();

			return *this;
		}
		template <typename Derived = Base>
		Weak(Weak<Derived, Type>&& move) noexcept
			requires DerivedFrom<Derived, Base>
			: m_counter(move.m_counter)
			, m_base(move.m_base) {
			move.m_counter = nullptr;
			move.m_base = nullptr;
		}
		template <typename Derived = Base>
		Weak& operator=(Weak<Derived, Type>&& m) noexcept
			requires DerivedFrom<Derived, Base>
		{
			this->~Weak();

			m_counter = m.m_counter;
			m_base = m.m_base;
			m.m_counter = nullptr;
			m.m_base = nullptr;

			return *this;
		}
		~Weak() {
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

		GRIZZLY_NO_DISCARD Option<Shared<Base, Type>> upgrade() const {
			auto& c = counter();
			const auto strong_count = c.strong();
			if (strong_count > 0) {
				c.add_strong();
				return Shared<Base, Type>{ m_counter, m_base };
			}
			return nullopt;
		}

		GRIZZLY_NO_DISCARD GRIZZLY_ALWAYS_INLINE u32 strong() const {
			return m_counter != nullptr ? counter().strong() : 0;
		}
		GRIZZLY_NO_DISCARD GRIZZLY_ALWAYS_INLINE u32 weak() const {
			return m_counter != nullptr ? counter().weak() : 0;
		}

	private:
		GRIZZLY_ALWAYS_INLINE Counter const& counter() const { return *m_counter; }

		explicit Weak(Counter* counter, Base* base) : m_counter(counter), m_base(base) {}

		template <typename Derived, SharedType>
		friend class Shared;

		template <typename Derived, SharedType>
		friend class Weak;

		Counter* m_counter = nullptr;
		Base* m_base = nullptr;
	};

	template <typename T, SharedType Type>
	class SharedFromThis : SharedFromThisBase {
	public:
		using Counter = SharedCounter<Type>;

		GRIZZLY_NO_DISCARD GRIZZLY_ALWAYS_INLINE Shared<T, Type> to_shared() const {
			return m_this.as_const_ref().unwrap().upgrade().unwrap();
		}

	private:
		template <typename Derived, SharedType>
		friend class Shared;

		Option<Weak<T, Type>> m_this = nullopt;
	};
} // namespace Grizzly::Core

namespace Grizzly {
	template <typename T>
	using Shared = Core::Shared<T, Core::SharedType::NonAtomic>;

	template <typename T>
	using SharedFromThis = Core::SharedFromThis<T, Core::SharedType::NonAtomic>;

	template <typename T>
	using Weak = Core::Weak<T, Core::SharedType::NonAtomic>;

	template <typename T>
	using AtomicShared = Core::Shared<T, Core::SharedType::Atomic>;

	template <typename T>
	using AtomicSharedFromThis = Core::SharedFromThis<T, Core::SharedType::Atomic>;

	template <typename T>
	using AtomicWeak = Core::Weak<T, Core::SharedType::Atomic>;
} // namespace Grizzly
