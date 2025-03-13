/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Concepts.hpp>
#include <Core/Memory.hpp>

namespace Forge::Core {
	template <typename Base>
	class Unique {
	public:
		explicit Unique()
			requires DefaultInitializable<Base>
			: Unique{ Base{} } {}

		template <typename... Args>
		static FORGE_ALWAYS_INLINE Unique<Base> create(Args&&... args)
			requires ConstructibleFrom<Base, Args...>
		{
			return Unique<Base>{ Base(std::forward<Args>(args)...) };
		}

		Unique(const Unique<Base>& copy) noexcept
			requires CopyConstructible<Base>
			: Unique{ Base{ *copy } } {}

		Unique& operator=(const Unique<Base>& copy) noexcept
			requires CopyConstructible<Base>
		{
			this->~Unique();
			*this = Unique<Base>{ Base{ *copy } };
			return *this;
		}

		template <typename Derived = Base>
		Unique(Unique<Derived>&& move) noexcept
			requires DerivedFrom<Derived, Base> || SameAs<Derived, Base>
			: m_ptr(move.m_ptr) {
			move.m_ptr = nullptr;
		}

		template <typename Derived = Base>
		Unique& operator=(Unique<Derived>&& move) noexcept
			requires DerivedFrom<Derived, Base> || SameAs<Derived, Base>
		{
			this->~Unique();

			m_ptr = move.m_ptr;
			move.m_ptr = nullptr;

			return *this;
		}

		~Unique() {
			if (m_ptr) {
				m_ptr->~Base();
				Memory::free(m_ptr);
				m_ptr = nullptr;
			}
		}

		FORGE_ALWAYS_INLINE operator Base*() { return m_ptr; }
		FORGE_ALWAYS_INLINE operator Base*() const { return m_ptr; }
		FORGE_ALWAYS_INLINE operator Base&() { return *m_ptr; }
		FORGE_ALWAYS_INLINE operator Base&() const { return *m_ptr; }
		FORGE_ALWAYS_INLINE Base* operator->() { return m_ptr; }
		FORGE_ALWAYS_INLINE Base* operator->() const { return m_ptr; }
		FORGE_ALWAYS_INLINE Base& operator*() { return *m_ptr; }
		FORGE_ALWAYS_INLINE Base& operator*() const { return *m_ptr; }

	private:
		FORGE_ALWAYS_INLINE explicit Unique(Base&& base)
			requires MoveConstructible<Base>
		{
			const auto ptr = Memory::alloc(Memory::Layout::single<Base>());
			m_ptr = Memory::emplace<Base>(ptr, Forge::forward<Base>(base));
		}

		template <typename Derived>
		friend class Unique;

		Base* m_ptr;
	};

	template <typename T>
	class Unique<T[]> {
	public:
		static FORGE_ALWAYS_INLINE Unique create(usize len)
			requires DefaultInitializable<T>
		{
			FORGE_ASSERT(len > 0);
			const auto memory = Memory::alloc(Memory::Layout::array<T>(len));
			T* const ptr = reinterpret_cast<T*>(*memory);
			for (usize i = 0; i < len; ++i) {
				new (ptr + i) T{};
			}
			return Unique{ ptr, len };
		}

		Unique(const Unique& copy) noexcept
			requires CopyConstructible<T>
		{
			FORGE_ASSERT(copy.len() > 0);
			const auto memory = Memory::alloc(Memory::Layout::array<T>(copy.len()));
			T* const ptr = reinterpret_cast<T*>(*memory);
			for (usize i = 0; i < copy.len(); ++i) {
				new (ptr + i) T{ copy[i] };
			}
			return Unique{ ptr, copy.len() };
		}

		Unique& operator=(const Unique& copy) noexcept
			requires CopyConstructible<T>
		{
			this->~Unique();
			FORGE_ASSERT(copy.len() > 0);
			const auto memory = Memory::alloc(Memory::Layout::array<T>(copy.len()));
			T* const ptr = reinterpret_cast<T*>(*memory);
			for (usize i = 0; i < copy.len(); ++i) {
				new (ptr + i) T{ copy[i] };
			}
			m_ptr = ptr;
			m_len = copy.len();
			return *this;
		}

		Unique(Unique&& move) noexcept : m_ptr(move.m_ptr), m_len(move.m_len) {
			move.m_ptr = nullptr;
			move.m_len = 0;
		}

		Unique& operator=(Unique&& move) noexcept {
			this->~Unique();

			m_ptr = move.m_ptr;
			m_len = move.m_len;
			move.m_ptr = nullptr;
			move.m_len = 0;

			return *this;
		}

		FORGE_NO_DISCARD FORGE_ALWAYS_INLINE usize len() const { return m_len; }
		FORGE_NO_DISCARD FORGE_ALWAYS_INLINE bool is_valid_index(usize index) const { return index < len(); }

		FORGE_ALWAYS_INLINE T* begin() { return m_ptr; }
		FORGE_ALWAYS_INLINE T* end() { return m_ptr + m_len; }
		FORGE_ALWAYS_INLINE const T* begin() const { return m_ptr; }
		FORGE_ALWAYS_INLINE const T* end() const { return m_ptr + m_len; }
		FORGE_ALWAYS_INLINE const T* cbegin() const { return m_ptr; }
		FORGE_ALWAYS_INLINE const T* cend() const { return m_ptr + m_len; }

		FORGE_ALWAYS_INLINE operator T*() { return m_ptr; }
		FORGE_ALWAYS_INLINE operator T*() const { return m_ptr; }
		FORGE_ALWAYS_INLINE T& operator[](usize index) {
			FORGE_ASSERT(is_valid_index(index), "Index out of bounds");
			return m_ptr[index];
		}
		FORGE_ALWAYS_INLINE T const& operator[](usize index) const {
			FORGE_ASSERT(is_valid_index(index), "Index out of bounds");
			return m_ptr[index];
		}

		~Unique() {
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
		FORGE_ALWAYS_INLINE explicit Unique(T* ptr, usize len) : m_ptr(ptr), m_len(len) {}

		T* m_ptr;
		usize m_len;
	};
} // namespace Forge::Core

namespace Forge {
	using Core::Unique;
} // namespace Forge
