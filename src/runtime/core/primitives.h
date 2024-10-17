/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <cstddef>
#include <cstdint>

namespace grizzly::core {
	template <typename T>
	struct NumericLimits;

	using u8 = uint8_t;
	static_assert(sizeof(u8) == 1, "u8 should only be 1 byte");
	template <>
	struct NumericLimits<u8> {
		static constexpr u8 min() { return 0; }
		static constexpr u8 max() { return 0xff; }
	};

	using u16 = uint16_t;
	static_assert(sizeof(u16) == 2, "u16 should only be 2 bytes");
	template <>
	struct NumericLimits<u16> {
		static constexpr u16 min() { return 0; }
		static constexpr u16 max() { return 0xffff; }
	};

	using u32 = uint32_t;
	static_assert(sizeof(u32) == 4, "u32 should only be 4 bytes");
	template <>
	struct NumericLimits<u32> {
		static constexpr u32 min() { return 0; }
		static constexpr u32 max() { return 0xffffffff; }
	};

	using u64 = uint64_t;
	static_assert(sizeof(u64) == 8, "u64 should only be 8 bytes");
	using usize = size_t;
	static_assert(sizeof(usize) == 8, "usize should only be 8 bytes");
	template <>
	struct NumericLimits<u64> {
		static constexpr u64 min() { return 0; }
		static constexpr u64 max() { return 0xffffffffffffffff; }
	};

	using i8 = int8_t;
	static_assert(sizeof(i8) == 1, "i8 should only be 1 byte");
	template <>
	struct NumericLimits<i8> {
		static constexpr i8 min() { return (-127 - 1); }
		static constexpr i8 max() { return 127; }
	};

	using i16 = int16_t;
	static_assert(sizeof(i16) == 2, "i16 should only be 2 bytes");
	template <>
	struct NumericLimits<i16> {
		static constexpr i16 min() { return (-32767 - 1); }
		static constexpr i16 max() { return 32767; }
	};

	using i32 = int32_t;
	static_assert(sizeof(i32) == 4, "i32 should only be 4 bytes");
	template <>
	struct NumericLimits<i32> {
		static constexpr i32 min() { return (-2147483647 - 1); }
		static constexpr i32 max() { return 2147483647; }
	};

	using i64 = int64_t;
	static_assert(sizeof(i64) == 8, "i64 should only be 8 bytes");
	using isize = ptrdiff_t;
	static_assert(sizeof(isize) == 8, "isize should only be 8 bytes");
	template <>
	struct NumericLimits<i64> {
		static constexpr i64 min() { return (-9223372036854775807 - 1); }
		static constexpr i64 max() { return 9223372036854775807; }
	};

	using f32 = float;
	static_assert(sizeof(f32) == 4, "f32 should only be 4 bytes");
	template <>
	struct NumericLimits<f32> {
		static constexpr f32 min() { return 1.17549435e-38F; }
		static constexpr f32 max() { return 3.40282347e+38F; }
	};

	using f64 = double;
	static_assert(sizeof(f64) == 8, "f64 should only be 8 bytes");

	using NullPtr = decltype(nullptr);
} // namespace grizzly::core

namespace grizzly {
	// Export core primitives to the op namespace
	using core::f32;
	using core::f64;
	using core::i16;
	using core::i32;
	using core::i64;
	using core::i8;
	using core::isize;
	using core::NullPtr;
	using core::u16;
	using core::u32;
	using core::u64;
	using core::u8;
	using core::usize;
} // namespace grizzly
