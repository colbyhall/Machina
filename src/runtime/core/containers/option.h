/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include "core/concepts.h"
#include "core/memory.h"

#define TRY(expression) 						\
	({   										\
		auto&& _temp_result = (expression); 	\
  		if (!_temp_result.is_set()) { 			\
  			return op::core::nullopt;			\
		}										\
		_temp_result.unwrap();					\
  	})

namespace op::core {
	struct NullOpt {
		constexpr explicit NullOpt(int){};
	};
	constexpr NullOpt nullopt{ 0 };

	template <typename T>
	class Option;

	template <typename T>
		requires(!is_trivially_copyable<T> && !is_reference<T>)
	class Option<T> {
	public:
		// Constructors
		Option() = default;
		OP_ALWAYS_INLINE constexpr Option(NullOpt) : m_set(false), m_data() {}
		OP_ALWAYS_INLINE Option(T&& t)
			requires MoveConstructible<T>
			: m_set(true)
			, m_data() {
			auto* p = m_data;
			new (p) T(op::forward<T>(t));
		}
		OP_ALWAYS_INLINE Option& operator=(T&& t)
			requires MoveConstructible<T>
		{
			if (m_set) {
				auto* p = reinterpret_cast<T*>(&m_data[0]);
				p->~T();
			}
			auto* p = m_data;
			m_set = true;
			new (p) T(op::forward<T>(t));
			return *this;
		}

		OP_ALWAYS_INLINE Option(const T& t)
			requires CopyConstructible<T>
			: m_set(true)
			, m_data() {
			auto* p = m_data;
			new (p) T(t);
		}
		OP_ALWAYS_INLINE Option& operator=(const T& t)
			requires CopyConstructible<T>
		{
			if (m_set) {
				auto* p = reinterpret_cast<T*>(&m_data[0]);
				p->~T();
			}
			auto* p = m_data;
			m_set = true;
			new (p) T(t);
			return *this;
		}

		OP_ALWAYS_INLINE Option(const Option<T>& copy)
			requires CopyConstructible<T>
		{
			m_set = copy.m_set;
			if (m_set) {
				auto* p = m_data;
				new (p) T(copy.as_const_ref().unwrap());
			}
		}

		OP_ALWAYS_INLINE Option& operator=(const Option<T>& copy)
			requires CopyConstructible<T>
		{
			auto to_destroy = op::move(*this);
			OP_UNUSED(to_destroy);

			m_set = copy.m_set;
			if (m_set) {
				auto* p = m_data;
				new (p) T(copy.as_const_ref().unwrap());
			}

			return *this;
		}

		OP_ALWAYS_INLINE Option(Option<T>&& move) noexcept : m_set(move.m_set) {
			if (m_set) {
				core::copy(m_data, move.m_data, sizeof(T));
				core::set(move.m_data, 0, sizeof(T));
			}
			move.m_set = false;
		}

		OP_ALWAYS_INLINE Option& operator=(Option<T>&& move) noexcept {
			auto to_destroy = op::move(*this);
			OP_UNUSED(to_destroy);

			m_set = move.m_set;
			move.m_set = false;

			if (m_set) {
				core::copy(m_data, move.m_data, sizeof(T));
				core::set(move.m_data, 0, sizeof(T));
			}
			return *this;
		}

		OP_ALWAYS_INLINE ~Option() {
			if (m_set) {
				auto* p = reinterpret_cast<T*>(&m_data[0]);
				p->~T();
				m_set = false;
				core::set(m_data, 0, sizeof(T));
			}
		}

		OP_NO_DISCARD OP_ALWAYS_INLINE bool is_set() const { return m_set; }
		OP_ALWAYS_INLINE explicit operator bool() const { return is_set(); }

		OP_ALWAYS_INLINE T unwrap()
			requires Movable<T> || Copyable<T>
		{
			OP_ASSERT(is_set(), "Value must be set to be unwrapped");
			m_set = false;

			auto* p = reinterpret_cast<T*>(&m_data[0]);

			if constexpr (Movable<T>) {
				return op::move(*p);
			} else {
				return *p;
			}
		}

		OP_ALWAYS_INLINE T unwrap_or_default()
			requires is_default_constructible<T>
		{
			if (is_set()) {
				return unwrap();
			}
			return T{};
		}

		OP_ALWAYS_INLINE T unwrap_or(T&& t)
			requires Movable<T>
		{
			if (is_set()) {
				return unwrap();
			}
			return op::move(t);
		}

		OP_ALWAYS_INLINE T unwrap_or(const T& t)
			requires Copyable<T>
		{
			if (is_set()) {
				return unwrap();
			}
			return t;
		}

		OP_ALWAYS_INLINE Option<T&> as_ref() {
			if (is_set()) {
				auto* p = reinterpret_cast<T*>(&m_data[0]);
				return Option<T&>{ *p };
			}

			return nullopt;
		}

		OP_ALWAYS_INLINE Option<T const&> as_const_ref() const {
			if (is_set()) {
				auto* p = reinterpret_cast<T const*>(&m_data[0]);
				return Option<T const&>{ *p };
			}

			return nullopt;
		}

	private:
		bool m_set = false;
		alignas(T) u8 m_data[sizeof(T)] = {};
	};

	template <typename T>
		requires is_trivially_copyable<T>
	class Option<T> {
	public:
		Option() = default;
		OP_ALWAYS_INLINE constexpr Option(NullOpt) : m_set(false), m_data() {}
		OP_ALWAYS_INLINE Option(const T& t) : m_set(true), m_data() {
			auto* p = m_data;
			new (p) T(t);
		}

		OP_ALWAYS_INLINE Option& operator=(const T& t) {
			if (m_set) {
				auto* p = reinterpret_cast<T*>(&m_data[0]);
				*p = t;
			} else {
				auto* p = m_data;
				m_set = true;
				new (p) T(t);
			}

			return *this;
		}

		OP_ALWAYS_INLINE Option(const Option<T>& copy) {
			core::copy(m_data, copy.m_data, sizeof(m_data));
			m_set = copy.m_set;
		}

		OP_ALWAYS_INLINE Option& operator=(const Option<T>& copy) {
			core::copy(m_data, copy.m_data, sizeof(m_data));
			m_set = copy.m_set;

			return *this;
		}

		OP_ALWAYS_INLINE ~Option() {
			if (m_set) {
				auto* p = reinterpret_cast<T*>(&m_data[0]);
				p->~T();
				m_set = false;
			}
		}

		OP_NO_DISCARD OP_ALWAYS_INLINE bool is_set() const { return m_set; }
		OP_ALWAYS_INLINE explicit operator bool() const { return is_set(); }

		OP_ALWAYS_INLINE T unwrap() const {
			OP_ASSERT(is_set(), "Value must be set to be unwrapped");

			// Do not reset m_set for trivially copyable types

			auto* p = reinterpret_cast<const T*>(&m_data[0]);
			return *p;
		}

		OP_ALWAYS_INLINE T unwrap_or_default() const
			requires is_default_constructible<T>
		{
			if (is_set()) {
				return unwrap();
			}
			return T{};
		}

		OP_ALWAYS_INLINE T unwrap_or(const T& t) const {
			if (is_set()) {
				return unwrap();
			}
			return t;
		}

		OP_ALWAYS_INLINE Option<T&> as_ref() {
			if (is_set()) {
				auto* p = reinterpret_cast<T*>(&m_data[0]);
				return Option<T&>(*p);
			} else {
				return Option<T&>();
			}
		}

		OP_ALWAYS_INLINE Option<T const&> as_const_ref() const {
			if (is_set()) {
				auto* p = reinterpret_cast<T const*>(&m_data[0]);
				return Option<T const&>(*p);
			} else {
				return Option<T const&>();
			}
		}

	private:
		bool m_set = false;
		alignas(T) u8 m_data[sizeof(T)] = {};
	};

	template <typename T>
		requires is_reference<T>
	class Option<T> {
	public:
		explicit Option() = default;
		OP_ALWAYS_INLINE constexpr Option(NullOpt) : m_ptr(nullptr) {}
		OP_ALWAYS_INLINE constexpr Option(T t) : m_ptr(&t) {}

		OP_NO_DISCARD OP_ALWAYS_INLINE bool is_set() const { return m_ptr != nullptr; }
		OP_ALWAYS_INLINE operator bool() const { return is_set(); }

		OP_ALWAYS_INLINE T& unwrap() {
			OP_ASSERT(is_set());
			return *m_ptr;
		}

	private:
		RemoveReference<T>* m_ptr = nullptr;
	};
} // namespace op::core

namespace op {
	using core::nullopt;
	using core::Option;
} // namespace op
