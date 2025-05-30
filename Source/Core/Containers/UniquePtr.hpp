/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Concepts.hpp>
#include <Core/Memory.hpp>

namespace Mach::Core {
	template <typename Base>
	class UniquePtr {
	public:
		explicit UniquePtr() : m_ptr(nullptr) {}
		explicit UniquePtr(NullPtr) : m_ptr(nullptr) {}

		template <typename... Args>
		static MACH_ALWAYS_INLINE UniquePtr<Base> create(Args&&... args)
			requires ConstructibleFrom<Base, Args...>
		{
			return UniquePtr<Base>{ Base(std::forward<Args>(args)...) };
		}

		UniquePtr(const UniquePtr<Base>& copy) noexcept
			requires CopyConstructible<Base>
			: UniquePtr{ Base{ *copy } } {}

		UniquePtr& operator=(const UniquePtr<Base>& copy) noexcept
			requires CopyConstructible<Base>
		{
			this->~UniquePtr();
			*this = UniquePtr<Base>{ Base{ *copy } };
			return *this;
		}

		template <typename Derived = Base>
		UniquePtr(UniquePtr<Derived>&& move) noexcept
			requires DerivedFrom<Derived, Base> || SameAs<Derived, Base>
			: m_ptr(move.m_ptr) {
			move.m_ptr = nullptr;
		}

		template <typename Derived = Base>
		UniquePtr& operator=(UniquePtr<Derived>&& move) noexcept
			requires DerivedFrom<Derived, Base> || SameAs<Derived, Base>
		{
			this->~UniquePtr();

			m_ptr = move.m_ptr;
			move.m_ptr = nullptr;

			return *this;
		}

		~UniquePtr() {
			if (m_ptr) {
				m_ptr->~Base();
				Memory::free(m_ptr);
				m_ptr = nullptr;
			}
		}

		MACH_ALWAYS_INLINE operator Base*() { return m_ptr; }
		MACH_ALWAYS_INLINE operator Base*() const { return m_ptr; }
		MACH_ALWAYS_INLINE operator Base&() { return *m_ptr; }
		MACH_ALWAYS_INLINE operator Base&() const { return *m_ptr; }
		MACH_ALWAYS_INLINE Base* operator->() { return m_ptr; }
		MACH_ALWAYS_INLINE Base* operator->() const { return m_ptr; }
		MACH_ALWAYS_INLINE Base& operator*() { return *m_ptr; }
		MACH_ALWAYS_INLINE Base& operator*() const { return *m_ptr; }
		MACH_ALWAYS_INLINE bool is_valid() const { return m_ptr != nullptr; }

	private:
		MACH_ALWAYS_INLINE explicit UniquePtr(Base&& base)
			requires MoveConstructible<Base>
		{
			const auto ptr = Memory::alloc(Memory::Layout::single<Base>());
			m_ptr = Memory::emplace<Base>(ptr, Mach::forward<Base>(base));
		}

		template <typename Derived>
		friend class UniquePtr;

		template <typename T, typename... Args>
		UniquePtr<T> make_unique(Args&&... args);

		Base* m_ptr;
	};

	template <typename T>
	class UniquePtr<T[]> {
	public:
		explicit UniquePtr() : m_ptr(nullptr) {}
		explicit UniquePtr(NullPtr) : m_ptr(nullptr) {}

		static MACH_ALWAYS_INLINE UniquePtr create(usize len)
			requires DefaultInitializable<T>
		{
			MACH_ASSERT(len > 0);
			const auto memory = Memory::alloc(Memory::Layout::array<T>(len));
			T* const ptr = reinterpret_cast<T*>(*memory);
			for (usize i = 0; i < len; ++i) {
				new (ptr + i) T{};
			}
			return UniquePtr{ ptr, len };
		}

		UniquePtr(const UniquePtr& copy) noexcept
			requires CopyConstructible<T>
		{
			MACH_ASSERT(copy.len() > 0);
			const auto memory = Memory::alloc(Memory::Layout::array<T>(copy.len()));
			T* const ptr = reinterpret_cast<T*>(*memory);
			for (usize i = 0; i < copy.len(); ++i) {
				new (ptr + i) T{ copy[i] };
			}
			return UniquePtr{ ptr, copy.len() };
		}

		UniquePtr& operator=(const UniquePtr& copy) noexcept
			requires CopyConstructible<T>
		{
			this->~UniquePtr();
			MACH_ASSERT(copy.len() > 0);
			const auto memory = Memory::alloc(Memory::Layout::array<T>(copy.len()));
			T* const ptr = reinterpret_cast<T*>(*memory);
			for (usize i = 0; i < copy.len(); ++i) {
				new (ptr + i) T{ copy[i] };
			}
			m_ptr = ptr;
			m_len = copy.len();
			return *this;
		}

		UniquePtr(UniquePtr&& move) noexcept : m_ptr(move.m_ptr), m_len(move.m_len) {
			move.m_ptr = nullptr;
			move.m_len = 0;
		}

		UniquePtr& operator=(UniquePtr&& move) noexcept {
			this->~UniquePtr();

			m_ptr = move.m_ptr;
			m_len = move.m_len;
			move.m_ptr = nullptr;
			move.m_len = 0;

			return *this;
		}

		MACH_NO_DISCARD MACH_ALWAYS_INLINE usize len() const { return m_len; }
		MACH_NO_DISCARD MACH_ALWAYS_INLINE bool is_valid_index(usize index) const { return index < len(); }

		MACH_ALWAYS_INLINE T* begin() { return m_ptr; }
		MACH_ALWAYS_INLINE T* end() { return m_ptr + m_len; }
		MACH_ALWAYS_INLINE const T* begin() const { return m_ptr; }
		MACH_ALWAYS_INLINE const T* end() const { return m_ptr + m_len; }
		MACH_ALWAYS_INLINE const T* cbegin() const { return m_ptr; }
		MACH_ALWAYS_INLINE const T* cend() const { return m_ptr + m_len; }

		MACH_ALWAYS_INLINE operator T*() { return m_ptr; }
		MACH_ALWAYS_INLINE operator T*() const { return m_ptr; }
		MACH_ALWAYS_INLINE T& operator[](usize index) {
			MACH_ASSERT(is_valid_index(index), "Index out of bounds");
			return m_ptr[index];
		}
		MACH_ALWAYS_INLINE T const& operator[](usize index) const {
			MACH_ASSERT(is_valid_index(index), "Index out of bounds");
			return m_ptr[index];
		}
		MACH_ALWAYS_INLINE bool is_valid() const { return m_ptr != nullptr; }

		~UniquePtr() {
			if (m_ptr) {
				if constexpr (!is_trivially_destructible<T>) {
					for (usize i = 0; i < m_len; ++i) {
						T& item = m_ptr[i];
						item.~T();
					}
				}
				Memory::free(m_ptr);
				m_ptr = nullptr;
				m_len = 0;
			}
		}

	private:
		MACH_ALWAYS_INLINE explicit UniquePtr(T* ptr, usize len) : m_ptr(ptr), m_len(len) {}

		T* m_ptr;
		usize m_len;
	};
} // namespace Mach::Core

namespace Mach {
	using Core::UniquePtr;
} // namespace Mach
