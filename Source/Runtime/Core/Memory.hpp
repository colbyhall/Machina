/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Containers/NonNull.hpp>
#include <Core/TypeTraits.hpp>
#include <new>

namespace Grizzly::Core::Memory {
	struct Layout {
		usize size;
		usize alignment;

		template <typename T>
		static inline constexpr Layout single() {
			return Layout{ sizeof(T), alignof(T) };
		}

		template <typename T>
		static inline constexpr Layout array(usize len) {
			return Layout{ sizeof(T) * len, alignof(T) };
		}
	};

	template <typename T, typename... Args>
	NonNull<T> emplace(void* memory, Args&&... args) {
		return new (memory) T{ Grizzly::forward<Args>(args)... };
	}

	GRIZZLY_NO_DISCARD NonNull<void> alloc(const Layout& layout);

	template <typename T>
	GRIZZLY_ALWAYS_INLINE NonNull<T> alloc(usize len = 1) {
		static_assert(Grizzly::Core::is_trivial<T>, "Value must be a trivial type to malloc");
		return Memory::alloc(Layout::array<T>(len)).template as<T>();
	}

	void free(NonNull<void> ptr);

	NonNull<void> copy(NonNull<void> dst, NonNull<void const> src, usize count);
	NonNull<void> move(NonNull<void> dst, NonNull<void const> src, usize count);
	NonNull<void> set(NonNull<void> ptr, u8 value, usize count);

	u8 count_ones(u8 byte);

	template <typename T>
	GRIZZLY_ALWAYS_INLINE u32 count_ones(T t) {
		const usize size = sizeof(T);
		void* ptr = &t;
		u8 const* u8_casted = (u8 const*)ptr;

		u32 result = 0;
		for (usize i = 0; i < size; ++i) {
			result += count_ones(u8_casted[i]);
		}

		return result;
	}
} // namespace Grizzly::Core::Memory

namespace Grizzly::Memory {
	using namespace Grizzly::Core::Memory;
} // namespace Grizzly::Memory
