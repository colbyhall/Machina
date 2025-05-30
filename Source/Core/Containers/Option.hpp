/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Concepts.hpp>
#include <Core/Memory.hpp>

#define TRY(expression)                                                                                                \
	({                                                                                                                 \
		auto&& _temp_result = (expression);                                                                            \
		if (!_temp_result.is_set()) {                                                                                  \
			return Mach::Core::nullopt;                                                                                \
		}                                                                                                              \
		_temp_result.unwrap();                                                                                         \
	})

namespace Mach::Core {
	struct NullOpt {
		constexpr explicit NullOpt(int) {};
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
		MACH_ALWAYS_INLINE constexpr Option(NullOpt) : m_set(false), m_data() {}
		MACH_ALWAYS_INLINE Option(T&& t)
			requires MoveConstructible<T>
			: m_set(true)
			, m_data() {
			auto* p = m_data;
			Memory::emplace<T>(p, Mach::forward<T>(t));
		}
		MACH_ALWAYS_INLINE Option& operator=(T&& t)
			requires MoveConstructible<T>
		{
			if (m_set) {
				auto* p = reinterpret_cast<T*>(&m_data[0]);
				p->~T();
			}
			auto* p = m_data;
			m_set = true;
			Memory::emplace<T>(p, Mach::forward<T>(t));
			return *this;
		}

		MACH_ALWAYS_INLINE Option(const T& t)
			requires CopyConstructible<T>
			: m_set(true)
			, m_data() {
			auto* p = m_data;
			Memory::emplace<T>(p, t);
		}
		MACH_ALWAYS_INLINE Option& operator=(const T& t)
			requires CopyConstructible<T>
		{
			if (m_set) {
				auto* p = reinterpret_cast<T*>(&m_data[0]);
				p->~T();
			}
			auto* p = m_data;
			m_set = true;
			Memory::emplace<T>(p, t);
			return *this;
		}

		MACH_ALWAYS_INLINE Option(const Option<T>& copy)
			requires CopyConstructible<T>
		{
			m_set = copy.m_set;
			if (m_set) {
				auto* p = m_data;
				Memory::emplace<T>(p, copy.as_const_ref().unwrap());
			}
		}

		MACH_ALWAYS_INLINE Option& operator=(const Option<T>& copy)
			requires CopyConstructible<T>
		{
			auto to_destroy = Mach::move(*this);
			MACH_UNUSED(to_destroy);

			m_set = copy.m_set;
			if (m_set) {
				auto* p = m_data;
				Memory::emplace<T>(p, copy.as_const_ref().unwrap());
			}

			return *this;
		}

		MACH_ALWAYS_INLINE Option(Option<T>&& move) noexcept : m_set(move.m_set) {
			if (m_set) {
				Memory::copy(m_data, move.m_data, sizeof(T));
				Memory::set(move.m_data, 0, sizeof(T));
			}
			move.m_set = false;
		}

		MACH_ALWAYS_INLINE Option& operator=(Option<T>&& move) noexcept {
			auto to_destroy = Mach::move(*this);
			MACH_UNUSED(to_destroy);

			m_set = move.m_set;
			move.m_set = false;

			if (m_set) {
				Memory::copy(m_data, move.m_data, sizeof(T));
#if MACH_BUILD == MACH_BUILD_DEBUG
				Memory::set(move.m_data, 0, sizeof(T));
#endif
			}
			return *this;
		}

		MACH_ALWAYS_INLINE ~Option() {
			if (m_set) {
				auto* p = reinterpret_cast<T*>(&m_data[0]);
				p->~T();
				m_set = false;
				Memory::set(m_data, 0, sizeof(T));
			}
		}

		MACH_NO_DISCARD MACH_ALWAYS_INLINE bool is_set() const { return m_set; }
		MACH_ALWAYS_INLINE explicit operator bool() const { return is_set(); }

		MACH_ALWAYS_INLINE T unwrap()
			requires Movable<T> || Copyable<T>
		{
			MACH_ASSERT(is_set(), "Value must be set to be unwrapped");
			m_set = false;

			auto* p = reinterpret_cast<T*>(&m_data[0]);

			if constexpr (Movable<T>) {
				return Mach::move(*p);
			} else {
				return *p;
			}
		}

		MACH_ALWAYS_INLINE T unwrap_or_default()
			requires is_default_constructible<T>
		{
			if (is_set()) {
				return unwrap();
			}
			return T{};
		}

		MACH_ALWAYS_INLINE T unwrap_or(T&& t)
			requires Movable<T>
		{
			if (is_set()) {
				return unwrap();
			}
			return Mach::move(t);
		}

		MACH_ALWAYS_INLINE T unwrap_or(const T& t)
			requires Copyable<T>
		{
			if (is_set()) {
				return unwrap();
			}
			return t;
		}

		MACH_ALWAYS_INLINE Option<T&> as_ref() {
			if (is_set()) {
				auto* p = reinterpret_cast<T*>(&m_data[0]);
				return Option<T&>{ *p };
			}

			return nullopt;
		}

		MACH_ALWAYS_INLINE Option<T const&> as_const_ref() const {
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
		MACH_ALWAYS_INLINE constexpr Option(NullOpt) : m_set(false), m_data() {}
		MACH_ALWAYS_INLINE Option(const T& t) : m_set(true), m_data() {
			auto* p = m_data;
			Memory::emplace<T>(p, t);
		}

		MACH_ALWAYS_INLINE Option& operator=(const T& t) {
			if (m_set) {
				auto* p = reinterpret_cast<T*>(&m_data[0]);
				*p = t;
			} else {
				auto* p = m_data;
				m_set = true;
				Memory::emplace<T>(p, t);
			}

			return *this;
		}

		MACH_ALWAYS_INLINE Option(const Option<T>& copy) {
			Memory::copy(m_data, copy.m_data, sizeof(m_data));
			m_set = copy.m_set;
		}

		MACH_ALWAYS_INLINE Option& operator=(const Option<T>& copy) {
			Memory::copy(m_data, copy.m_data, sizeof(m_data));
			m_set = copy.m_set;

			return *this;
		}

		MACH_ALWAYS_INLINE ~Option() {
			if (m_set) {
				auto* p = reinterpret_cast<T*>(&m_data[0]);
				p->~T();
				m_set = false;
			}
		}

		MACH_NO_DISCARD MACH_ALWAYS_INLINE bool is_set() const { return m_set; }
		MACH_ALWAYS_INLINE explicit operator bool() const { return is_set(); }

		MACH_ALWAYS_INLINE T unwrap() const {
			MACH_ASSERT(is_set(), "Value must be set to be unwrapped");

			// Do not reset m_set for trivially copyable types

			auto* p = reinterpret_cast<const T*>(&m_data[0]);
			return *p;
		}

		MACH_ALWAYS_INLINE T unwrap_or_default() const
			requires is_default_constructible<T>
		{
			if (is_set()) {
				return unwrap();
			}
			return T{};
		}

		MACH_ALWAYS_INLINE T unwrap_or(const T& t) const {
			if (is_set()) {
				return unwrap();
			}
			return t;
		}

		MACH_ALWAYS_INLINE Option<T&> as_ref() {
			if (is_set()) {
				auto* p = reinterpret_cast<T*>(&m_data[0]);
				return Option<T&>(*p);
			} else {
				return Option<T&>();
			}
		}

		MACH_ALWAYS_INLINE Option<T const&> as_const_ref() const {
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
		using Underlying = RemoveReference<T>;

		explicit Option() = default;
		MACH_ALWAYS_INLINE constexpr Option(NullOpt) : m_ptr(nullptr) {}
		MACH_ALWAYS_INLINE constexpr Option(T t) : m_ptr(&t) {}

		MACH_NO_DISCARD MACH_ALWAYS_INLINE bool is_set() const { return m_ptr != nullptr; }
		MACH_ALWAYS_INLINE operator bool() const { return is_set(); }

		MACH_ALWAYS_INLINE Underlying* operator->() { return m_ptr; }
		MACH_ALWAYS_INLINE Underlying* operator->() const { return m_ptr; }

		MACH_ALWAYS_INLINE T unwrap() {
			MACH_ASSERT(is_set());
			return *m_ptr;
		}

	private:
		Underlying* m_ptr = nullptr;
	};
} // namespace Mach::Core

namespace Mach {
	using Core::nullopt;
	using Core::Option;
} // namespace Mach
