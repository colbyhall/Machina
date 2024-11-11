/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Concepts.hpp>
#include <Core/Memory.hpp>

namespace Grizzly::Core {
	template <typename Base>
	class Unique {
	public:
		explicit Unique()
			requires DefaultInitializable<Base>
			: Unique{ Base{} } {}

		template <typename... Args>
		static GRIZZLY_ALWAYS_INLINE Unique<Base> create(Args&&... args)
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

		GRIZZLY_ALWAYS_INLINE operator Base*() { return m_ptr; }
		GRIZZLY_ALWAYS_INLINE operator Base*() const { return m_ptr; }
		GRIZZLY_ALWAYS_INLINE Base* operator->() { return m_ptr; }
		GRIZZLY_ALWAYS_INLINE Base* operator->() const { return m_ptr; }
		GRIZZLY_ALWAYS_INLINE Base& operator*() { return *m_ptr; }
		GRIZZLY_ALWAYS_INLINE Base& operator*() const { return *m_ptr; }

	private:
		GRIZZLY_ALWAYS_INLINE explicit Unique(Base&& base)
			requires MoveConstructible<Base>
		{
			const auto ptr = Memory::alloc(Memory::Layout::single<Base>());
			m_ptr = Memory::emplace<Base>(ptr, Grizzly::forward<Base>(base));
		}

		template <typename Derived>
		friend class Unique;

		Base* m_ptr;
	};
} // namespace Grizzly::Core

namespace Grizzly {
	using Core::Unique;
} // namespace Grizzly
