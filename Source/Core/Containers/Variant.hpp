/**
 * Copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Concepts.hpp>
#include <Core/Containers/Option.hpp>
#include <Core/Memory.hpp>

namespace Mach::Core {
	template <typename T, typename... Ts>
	struct TypeIndex;

	template <typename T, typename... Ts>
	struct TypeIndex<T, T, Ts...> {
		static constexpr usize value = 0;
	};

	template <typename T, typename U, typename... Ts>
	struct TypeIndex<T, U, Ts...> {
		static constexpr usize value = 1 + TypeIndex<T, Ts...>::value;
	};

	// Helper alias template to make usage easier
	template <typename T, typename... Types>
	inline constexpr usize type_index = TypeIndex<T, Types...>::value;

	template <usize Index, typename T, typename... Ts>
	struct NthType {
		using type = typename NthType<Index - 1, Ts...>::type;
	};

	template <typename T, typename... Ts>
	struct NthType<0, T, Ts...> {
		using type = T;
	};

	template <usize Index, typename... Ts>
	using Nth = typename NthType<Index, Ts...>::type;

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

	template <typename T, typename... Ts>
	concept Contains = (Core::is_same<T, Ts> || ...);

	template <typename... Ts>
	class Variant {
	public:
		using Tag = u8;
		static constexpr usize Count = sizeof...(Ts);
		static constexpr usize MaxSizeOf = Core::max_sizeof<Ts...>();

		template <typename T>
			requires Contains<T, Ts...>
		Variant(T&& value) {
			static_assert(Contains<T, Ts...>);
			Memory::emplace<T>(m_data, T(Mach::forward<T>(value)));
			m_tag = static_cast<Tag>(type_index<T, Ts...>);
		}

		Variant(const Variant& other) { copy_by_index(other); }

		Variant& operator=(const Variant& other) {
			if (this != &other) {
				destroy_by_index();
				copy_by_index(other);
			}
			return *this;
		}

		Variant(Variant&& other) noexcept {
			move_by_index(Mach::move(other));
			other.m_tag = 0;
		}

		Variant& operator=(Variant&& other) noexcept {
			if (this != &other) {
				destroy_by_index();
				move_by_index(Mach::move(other));
				other.m_tag = 0;
			}
			return *this;
		}

		~Variant() { destroy_by_index(); }

		template <typename T>
			requires Contains<T, Ts...>
		bool is() const {
			return m_tag == type_index<T, Ts...>;
		}

		template <typename T>
			requires Contains<T, Ts...>
		Option<T&> get() {
			if (!is<T>()) return nullopt;
			return *reinterpret_cast<T*>(m_data);
		}

		template <typename T>
			requires Contains<T, Ts...>
		Option<const T&> get() const {
			if (!is<T>()) return nullopt;
			return *reinterpret_cast<const T*>(m_data);
		}

		template <typename T>
			requires Contains<T, Ts...>
		T& get_unchecked() {
			return *reinterpret_cast<T*>(m_data);
		}

		template <typename T>
			requires Contains<T, Ts...>
		const T& get_unchecked() const {
			return *reinterpret_cast<const T*>(m_data);
		}

	private:
		alignas(u64) u8 m_data[MaxSizeOf];
		Tag m_tag = 0;

		// --- Indexed Dispatch ---

		template <usize Index = 0>
		void destroy_by_index() {
			if constexpr (Index < Count) {
				if (m_tag == Index) {
					using T = Nth<Index, Ts...>;
					reinterpret_cast<T*>(m_data)->~T();
				} else {
					destroy_by_index<Index + 1>();
				}
			}
		}

		template <usize Index = 0>
		void copy_by_index(const Variant& other) {
			if constexpr (Index < Count) {
				if (other.m_tag == Index) {
					using T = Nth<Index, Ts...>;
					auto* from = reinterpret_cast<const T*>(other.m_data);
					Memory::emplace<T>(m_data, T(*from));
					m_tag = other.m_tag;
				} else {
					copy_by_index<Index + 1>(other);
				}
			}
		}

		template <usize Index = 0>
		void move_by_index(Variant&& other) {
			if constexpr (Index < Count) {
				if (other.m_tag == Index) {
					using T = Nth<Index, Ts...>;
					auto* from = reinterpret_cast<T*>(other.m_data);
					Memory::emplace<T>(m_data, T(Mach::move(*from)));
					m_tag = other.m_tag;
				} else {
					move_by_index<Index + 1>(Mach::move(other));
				}
			}
		}
	};

} // namespace Mach::Core

namespace Mach {
	using Core::Variant;
}
