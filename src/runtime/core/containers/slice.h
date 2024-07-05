/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include "core/debug/assertions.h"
#include "core/initializer_list.h"
#include "core/primitives.h"
#include "core/type_traits.h"

namespace op::core {
	template <typename T>
	class Slice {
	public:
		Slice() = default;
		OP_ALWAYS_INLINE constexpr explicit Slice(T* ptr, usize len);
		OP_ALWAYS_INLINE Slice(InitializerList<T const> list)
			requires(is_const<T>)
			: m_ptr(list.begin())
			, m_len(static_cast<usize>(list.end() - list.begin())) {}

		// Copy and Move operators
		OP_ALWAYS_INLINE Slice(const Slice<T>& copy);
		OP_ALWAYS_INLINE Slice<T>& operator=(const Slice<T>& copy);
		OP_ALWAYS_INLINE Slice(Slice<T>&& move) noexcept;
		OP_ALWAYS_INLINE Slice<T>& operator=(Slice<T>&& move) noexcept;

		// Array functionality
		OP_NO_DISCARD OP_ALWAYS_INLINE usize len() const { return m_len; }
		OP_NO_DISCARD OP_ALWAYS_INLINE bool is_empty() const { return m_len == 0; }
		OP_NO_DISCARD OP_ALWAYS_INLINE bool is_valid_index(usize index) const { return index < m_len; }
		OP_ALWAYS_INLINE explicit operator bool() const { return !is_empty(); }

		// Range accessors
		OP_ALWAYS_INLINE T* begin() const { return m_ptr; }
		OP_ALWAYS_INLINE T* end() const { return m_ptr + m_len; }
		OP_ALWAYS_INLINE T* cbegin() const { return m_ptr; }
		OP_ALWAYS_INLINE T* cend() const { return m_ptr + m_len; }

		// Accessor
		OP_ALWAYS_INLINE T& operator[](usize index) const {
			OP_ASSERT(is_valid_index(index), "Index out of bounds.");
			return m_ptr[index];
		}

		OP_ALWAYS_INLINE explicit operator Slice<T const>() const
			requires(!is_const<T>)
		{
			return Slice<T const>{ m_ptr, m_len };
		}

		OP_NO_DISCARD OP_ALWAYS_INLINE Slice shrink(usize amount) const {
			OP_ASSERT(amount <= m_len, "Cannot shrink more than len");
			return Slice{ m_ptr, m_len - amount };
		}

	private:
		T* m_ptr = nullptr;
		usize m_len = 0;
	};

	template <typename T>
	OP_ALWAYS_INLINE constexpr Slice<T>::Slice(T* ptr, usize len) : m_ptr(ptr)
																  , m_len(len) {}

	template <typename T>
	OP_ALWAYS_INLINE Slice<T>::Slice(const Slice<T>& copy) : m_ptr(copy.m_ptr)
														   , m_len(copy.m_len) {}
	template <typename T>
	OP_ALWAYS_INLINE Slice<T>& Slice<T>::operator=(const Slice<T>& copy) {
		m_ptr = copy.m_ptr;
		m_len = copy.m_len;

		return *this;
	}

	template <typename T>
	OP_ALWAYS_INLINE Slice<T>::Slice(Slice<T>&& move) noexcept {
		m_ptr = move.m_ptr;
		m_len = move.m_len;

		move.m_ptr = nullptr;
		move.m_len = 0;
	}

	template <typename T>
	OP_ALWAYS_INLINE Slice<T>& Slice<T>::operator=(Slice<T>&& move) noexcept {
		m_ptr = move.m_ptr;
		m_len = move.m_len;

		move.m_ptr = nullptr;
		move.m_len = 0;

		return *this;
	}
} // namespace op::core

namespace op {
	using core::Slice;
} // namespace op