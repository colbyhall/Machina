/**
 * Copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Concepts.hpp>
#include <Core/Containers/Option.hpp>
#include <Core/Memory.hpp>

namespace Forge::Core {
	template <typename T, typename... Types>
	struct TypeIndex;

	// Base case: If the first type matches T, return index 0
	template <typename T, typename... Types>
	struct TypeIndex<T, T, Types...> {
		static constexpr usize value = 0;
	};

	// Recursive case: If the first type does not match T, increment the index and recurse
	template <typename T, typename U, typename... Types>
	struct TypeIndex<T, U, Types...> {
		static constexpr usize value = 1 + TypeIndex<T, Types...>::value;
	};

	// Helper alias template to make usage easier
	template <typename T, typename... Types>
	inline constexpr usize type_index = TypeIndex<T, Types...>::value;

	template <typename T>
	constexpr const T& max(const T& a, const T& b) {
		return (b < a) ? a : b;
	}

	template <typename T, typename... Ts>
	constexpr T max(T a, Ts... args) {
		return (sizeof...(args) == 0) ? a : max(a, max(args...));
	}

	template <typename... Ts>
	constexpr usize max_sizeof() {
		return max<usize>(sizeof(Ts)...);
	}

	template <typename... Ts>
	class Variant {
	public:
		static constexpr usize Count = sizeof...(Ts);
		using Tag = u8;

		template <typename T>
			requires(Core::is_same<T, Ts> || ...)
		Variant(T&& value) {
			new (m_data) T(Forge::forward<T>(value));
			m_tag = static_cast<u8>(type_index<T, Ts...>);
		}
		Variant(const Variant&) = delete;
		Variant& operator=(const Variant&) = delete;
		Variant(Variant&& m) { (move<Ts>(m), ...); }
		Variant& operator=(Variant&& m) {
			(move<Ts>(m), ...);
			return *this;
		}
		~Variant() { (destroy<Ts>(), ...); }

		template <typename T>
			requires(Core::is_same<T, Ts> || ...)
		bool is() const {
			return m_tag == type_index<T, Ts...>;
		}

		template <typename T>
			requires(Core::is_same<T, Ts> || ...)
		Option<T&> get() {
			if (!is<T>()) {
				return Forge::nullopt;
			}
			return *reinterpret_cast<T*>(m_data);
		}

		template <typename T>
			requires(Core::is_same<T, Ts> || ...)
		Option<T const&> get() const {
			if (!is<T>()) {
				return Forge::nullopt;
			}
			return *reinterpret_cast<const T*>(m_data);
		}

		template <typename T>
			requires(Core::is_same<T, Ts> || ...)
		T& get_unchecked() {
			return *reinterpret_cast<T*>(m_data);
		}

		template <typename T>
			requires(Core::is_same<T, Ts> || ...)
		T const& get_unchecked() const {
			return *reinterpret_cast<const T*>(m_data);
		}

	private:
		template <typename T>
			requires(Core::is_same<T, Ts> || ...)
		void destroy() {
			if (is<T>()) {
				reinterpret_cast<T*>(m_data)->~T();
			}
		}

		template <typename T>
			requires(Core::is_same<T, Ts> || ...)
		void move(Variant&& m) {
			if (m.is<T>()) {
				m_tag = m.m_tag;

				auto* from = reinterpret_cast<T*>(m.m_data);
				auto* to = reinterpret_cast<T*>(m_data);
				*to = Forge::move(*from);

				m.m_tag = 0;
			}
		}

		static constexpr usize MaxSizeOf = Core::max_sizeof<Ts...>();

		u8 m_tag = 0;
		u8 m_data[MaxSizeOf];
	};
} // namespace Forge::Core
