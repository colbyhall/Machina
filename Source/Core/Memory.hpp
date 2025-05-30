/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Containers/NonNull.hpp>
#include <Core/Containers/Slice.hpp>
#include <Core/TypeTraits.hpp>

#include <new>
#undef stdin
#undef stdout
#undef stderr

namespace Mach::Core::Memory {
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
		return new (memory) T{ Mach::forward<Args>(args)... };
	}

	MACH_NO_DISCARD NonNull<void> alloc(const Layout& layout);

	template <typename T>
	MACH_ALWAYS_INLINE NonNull<T> alloc(usize len = 1) {
		static_assert(Mach::Core::is_trivial<T>, "Value must be a trivial type to malloc");
		return Memory::alloc(Layout::array<T>(len)).template as<T>();
	}

	void free(NonNull<void> ptr);

	NonNull<void> copy(NonNull<void> dst, NonNull<void const> src, usize count);
	NonNull<void> move(NonNull<void> dst, NonNull<void const> src, usize count);
	NonNull<void> set(NonNull<void> ptr, u8 value, usize count);

	u8 count_ones(u8 byte);

	template <typename T>
	MACH_ALWAYS_INLINE u32 count_ones(T t) {
		const usize size = sizeof(T);
		void* ptr = &t;
		u8 const* u8_casted = (u8 const*)ptr;

		u32 result = 0;
		for (usize i = 0; i < size; ++i) {
			result += count_ones(u8_casted[i]);
		}

		return result;
	}

	template <typename T>
	Slice<u8 const> as_slice_of_bytes(const T& t) {
		return Slice<T const>(t).as_bytes();
	}
} // namespace Mach::Core::Memory

namespace Mach::Memory {
	using namespace Mach::Core::Memory;
} // namespace Mach::Memory
