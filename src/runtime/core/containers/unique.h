/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include "core/concepts.h"
#include "core/memory.h"

namespace op::core {
	template <typename Base>
	class Unique {
	public:
		explicit Unique()
			requires DefaultInitializable<Base>
			: Unique{ Base{} } {}

		template <typename... Args>
		static OP_ALWAYS_INLINE Unique<Base> create(Args&&... args)
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
				op::core::free(m_ptr);
				m_ptr = nullptr;
			}
		}

		OP_ALWAYS_INLINE explicit operator Base*() { return m_ptr; }
		OP_ALWAYS_INLINE explicit operator Base const*() const { return m_ptr; }
		OP_ALWAYS_INLINE Base* operator->() { return m_ptr; }
		OP_ALWAYS_INLINE Base const* operator->() const { return m_ptr; }
		OP_ALWAYS_INLINE Base& operator*() { return *m_ptr; }
		OP_ALWAYS_INLINE Base const& operator*() const { return *m_ptr; }

	private:
		OP_ALWAYS_INLINE explicit Unique(Base&& base)
			requires MoveConstructible<Base>
		{
			const auto ptr = alloc(Layout::single<Base>());
			m_ptr = new (ptr) Base{ op::forward<Base>(base) };
		}

		template <typename Derived>
		friend class Unique;

		Base* m_ptr;
	};
} // namespace op::core

namespace op {
	using core::Unique;
} // namespace op