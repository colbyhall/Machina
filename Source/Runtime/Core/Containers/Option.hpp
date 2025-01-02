/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
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
			return Grizzly::Core::nullopt;                                                                             \
		}                                                                                                              \
		_temp_result.unwrap();                                                                                         \
	})

namespace Grizzly::Core {
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
		GRIZZLY_ALWAYS_INLINE constexpr Option(NullOpt) : m_set(false), m_data() {}
		GRIZZLY_ALWAYS_INLINE Option(T&& t)
			requires MoveConstructible<T>
			: m_set(true)
			, m_data() {
			auto* p = m_data;
			Memory::emplace<T>(p, Grizzly::forward<T>(t));
		}
		GRIZZLY_ALWAYS_INLINE Option& operator=(T&& t)
			requires MoveConstructible<T>
		{
			if (m_set) {
				auto* p = reinterpret_cast<T*>(&m_data[0]);
				p->~T();
			}
			auto* p = m_data;
			m_set = true;
			Memory::emplace<T>(p, Grizzly::forward<T>(t));
			return *this;
		}

		GRIZZLY_ALWAYS_INLINE Option(const T& t)
			requires CopyConstructible<T>
			: m_set(true)
			, m_data() {
			auto* p = m_data;
			Memory::emplace<T>(p, t);
		}
		GRIZZLY_ALWAYS_INLINE Option& operator=(const T& t)
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

		GRIZZLY_ALWAYS_INLINE Option(const Option<T>& copy)
			requires CopyConstructible<T>
		{
			m_set = copy.m_set;
			if (m_set) {
				auto* p = m_data;
				Memory::emplace<T>(p, copy.as_const_ref().unwrap());
			}
		}

		GRIZZLY_ALWAYS_INLINE Option& operator=(const Option<T>& copy)
			requires CopyConstructible<T>
		{
			auto to_destroy = Grizzly::move(*this);
			GRIZZLY_UNUSED(to_destroy);

			m_set = copy.m_set;
			if (m_set) {
				auto* p = m_data;
				Memory::emplace<T>(p, copy.as_const_ref().unwrap());
			}

			return *this;
		}

		GRIZZLY_ALWAYS_INLINE Option(Option<T>&& move) noexcept : m_set(move.m_set) {
			if (m_set) {
				Memory::copy(m_data, move.m_data, sizeof(T));
				Memory::set(move.m_data, 0, sizeof(T));
			}
			move.m_set = false;
		}

		GRIZZLY_ALWAYS_INLINE Option& operator=(Option<T>&& move) noexcept {
			auto to_destroy = Grizzly::move(*this);
			GRIZZLY_UNUSED(to_destroy);

			m_set = move.m_set;
			move.m_set = false;

			if (m_set) {
				Memory::copy(m_data, move.m_data, sizeof(T));
#if GRIZZLY_BUILD == GRIZZLY_BUILD_DEBUG
				Memory::set(move.m_data, 0, sizeof(T));
#endif
			}
			return *this;
		}

		GRIZZLY_ALWAYS_INLINE ~Option() {
			if (m_set) {
				auto* p = reinterpret_cast<T*>(&m_data[0]);
				p->~T();
				m_set = false;
				Memory::set(m_data, 0, sizeof(T));
			}
		}

		GRIZZLY_NO_DISCARD GRIZZLY_ALWAYS_INLINE bool is_set() const { return m_set; }
		GRIZZLY_ALWAYS_INLINE explicit operator bool() const { return is_set(); }

		GRIZZLY_ALWAYS_INLINE T unwrap()
			requires Movable<T> || Copyable<T>
		{
			GRIZZLY_ASSERT(is_set(), "Value must be set to be unwrapped");
			m_set = false;

			auto* p = reinterpret_cast<T*>(&m_data[0]);

			if constexpr (Movable<T>) {
				return Grizzly::move(*p);
			} else {
				return *p;
			}
		}

		GRIZZLY_ALWAYS_INLINE T unwrap_or_default()
			requires is_default_constructible<T>
		{
			if (is_set()) {
				return unwrap();
			}
			return T{};
		}

		GRIZZLY_ALWAYS_INLINE T unwrap_or(T&& t)
			requires Movable<T>
		{
			if (is_set()) {
				return unwrap();
			}
			return Grizzly::move(t);
		}

		GRIZZLY_ALWAYS_INLINE T unwrap_or(const T& t)
			requires Copyable<T>
		{
			if (is_set()) {
				return unwrap();
			}
			return t;
		}

		GRIZZLY_ALWAYS_INLINE Option<T&> as_ref() {
			if (is_set()) {
				auto* p = reinterpret_cast<T*>(&m_data[0]);
				return Option<T&>{ *p };
			}

			return nullopt;
		}

		GRIZZLY_ALWAYS_INLINE Option<T const&> as_const_ref() const {
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
		GRIZZLY_ALWAYS_INLINE constexpr Option(NullOpt) : m_set(false), m_data() {}
		GRIZZLY_ALWAYS_INLINE Option(const T& t) : m_set(true), m_data() {
			auto* p = m_data;
			Memory::emplace<T>(p, t);
		}

		GRIZZLY_ALWAYS_INLINE Option& operator=(const T& t) {
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

		GRIZZLY_ALWAYS_INLINE Option(const Option<T>& copy) {
			Memory::copy(m_data, copy.m_data, sizeof(m_data));
			m_set = copy.m_set;
		}

		GRIZZLY_ALWAYS_INLINE Option& operator=(const Option<T>& copy) {
			Memory::copy(m_data, copy.m_data, sizeof(m_data));
			m_set = copy.m_set;

			return *this;
		}

		GRIZZLY_ALWAYS_INLINE ~Option() {
			if (m_set) {
				auto* p = reinterpret_cast<T*>(&m_data[0]);
				p->~T();
				m_set = false;
			}
		}

		GRIZZLY_NO_DISCARD GRIZZLY_ALWAYS_INLINE bool is_set() const { return m_set; }
		GRIZZLY_ALWAYS_INLINE explicit operator bool() const { return is_set(); }

		GRIZZLY_ALWAYS_INLINE T unwrap() const {
			GRIZZLY_ASSERT(is_set(), "Value must be set to be unwrapped");

			// Do not reset m_set for trivially copyable types

			auto* p = reinterpret_cast<const T*>(&m_data[0]);
			return *p;
		}

		GRIZZLY_ALWAYS_INLINE T unwrap_or_default() const
			requires is_default_constructible<T>
		{
			if (is_set()) {
				return unwrap();
			}
			return T{};
		}

		GRIZZLY_ALWAYS_INLINE T unwrap_or(const T& t) const {
			if (is_set()) {
				return unwrap();
			}
			return t;
		}

		GRIZZLY_ALWAYS_INLINE Option<T&> as_ref() {
			if (is_set()) {
				auto* p = reinterpret_cast<T*>(&m_data[0]);
				return Option<T&>(*p);
			} else {
				return Option<T&>();
			}
		}

		GRIZZLY_ALWAYS_INLINE Option<T const&> as_const_ref() const {
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
		GRIZZLY_ALWAYS_INLINE constexpr Option(NullOpt) : m_ptr(nullptr) {}
		GRIZZLY_ALWAYS_INLINE constexpr Option(T t) : m_ptr(&t) {}

		GRIZZLY_NO_DISCARD GRIZZLY_ALWAYS_INLINE bool is_set() const { return m_ptr != nullptr; }
		GRIZZLY_ALWAYS_INLINE operator bool() const { return is_set(); }

		GRIZZLY_ALWAYS_INLINE T& unwrap() {
			GRIZZLY_ASSERT(is_set());
			return *m_ptr;
		}

	private:
		RemoveReference<T>* m_ptr = nullptr;
	};
} // namespace Grizzly::Core

namespace Grizzly {
	using Core::nullopt;
	using Core::Option;
} // namespace Grizzly
