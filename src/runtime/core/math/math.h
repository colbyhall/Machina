/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include "core/concepts.h"
#include "core/primitives.h"

#include <limits>

namespace op::core::math {
	template <typename T>
	concept Number = FloatingPoint<T> || Integral<T>;

	template <Number T>
	constexpr OP_ALWAYS_INLINE T min(T a, T b) {
		return a < b ? a : b;
	}

	template <Number T>
	constexpr OP_ALWAYS_INLINE T max(T a, T b) {
		return a > b ? a : b;
	}

	template <Number T>
	constexpr OP_ALWAYS_INLINE T clamp(T value, T min, T max) {
		return math::min<T>(math::max<T>(value, min), max);
	}

	template <FloatingPoint T>
	inline constexpr T pi;

	template <>
	inline constexpr f32 pi<f32> = 3.14159265359f;

	template <>
	inline constexpr f64 pi<f64> = 3.14159265359;

	template <FloatingPoint T>
	inline constexpr T tau = pi<T> * T{ 2 };

	template <FloatingPoint T>
	inline constexpr T deg_to_rad = pi<T> / T{ 180 };

	template <FloatingPoint T>
	inline constexpr T rad_to_deg = T{ 180 } / pi<T>;

	template <Number T>
	inline constexpr T small_number = T{ 0 };

	template <>
	inline constexpr f32 small_number<f32> = 1.0e-8f;

	template <>
	inline constexpr f64 small_number<f64> = 1.0e-8;

	template <Number T>
	inline constexpr T kinda_small_number = T{ 0 };

	template <>
	inline constexpr f32 kinda_small_number<f32> = 1.0e-4f;

	template <>
	inline constexpr f64 kinda_small_number<f64> = 1.0e-4;

	template <FloatingPoint T>
	inline constexpr T infinity = std::numeric_limits<T>::infinity();

	template <FloatingPoint T>
	inline constexpr T epsilon = std::numeric_limits<T>::epsilon();

	template <FloatingPoint T>
	inline constexpr T nan = std::numeric_limits<T>::signaling_NaN();

	OP_NO_DISCARD f32 cos(f32 x);
	OP_NO_DISCARD f64 cos(f64 x);

	OP_NO_DISCARD f32 sin(f32 x);
	OP_NO_DISCARD f64 sin(f64 x);

	OP_NO_DISCARD f32 tan(f32 x);
	OP_NO_DISCARD f64 tan(f64 x);

	OP_NO_DISCARD f32 acos(f32 x);
	OP_NO_DISCARD f64 acos(f64 x);

	OP_NO_DISCARD f32 asin(f32 x);
	OP_NO_DISCARD f64 asin(f64 x);

	OP_NO_DISCARD f32 atan(f32 x);
	OP_NO_DISCARD f64 atan(f64 x);

	OP_NO_DISCARD f32 atan2(f32 y, f32 x);
	OP_NO_DISCARD f64 atan2(f64 y, f64 x);

	OP_NO_DISCARD f32 sqrt(f32 x);
	OP_NO_DISCARD f64 sqrt(f64 x);

	OP_NO_DISCARD f32 fmod(f32 numerator, f32 denominator);
	OP_NO_DISCARD f64 fmod(f64 numerator, f64 denominator);

	OP_NO_DISCARD f32 powf(f32 x, f32 y);
	OP_NO_DISCARD f64 pow(f64 x, f64 y);

	template <Number T>
	constexpr OP_ALWAYS_INLINE T abs(T x) {
		return x < 0 ? -x : x;
	}

	template <FloatingPoint T>
	constexpr OP_ALWAYS_INLINE bool is_near_zero(T a) {
		return abs<T>(a) <= small_number<T>;
	}

	template <FloatingPoint T>
	constexpr OP_ALWAYS_INLINE T lerp(T a, T b, T t) {
		return (T{ 1 } - t) * a + t * b;
	}

	template <Number T>
	constexpr OP_ALWAYS_INLINE T square(T value) {
		return value * value;
	}

	template <Number T>
	constexpr OP_ALWAYS_INLINE T equals(T a, T b, T tolerance = kinda_small_number<T>) {
		return abs<T>(a - b) <= tolerance;
	}
} // namespace op::core::math

namespace op::math {
	using core::math::Number;

	using core::math::abs;
	using core::math::acos;
	using core::math::asin;
	using core::math::atan;
	using core::math::atan2;
	using core::math::clamp;
	using core::math::cos;
	using core::math::equals;
	using core::math::fmod;
	using core::math::lerp;
	using core::math::max;
	using core::math::min;
	using core::math::sin;
	using core::math::sqrt;
	using core::math::square;
	using core::math::tan;

	template <Number T>
	inline constexpr T pi = core::math::pi<T>;

	template <Number T>
	inline constexpr T tau = core::math::tau<T>;

	template <Number T>
	inline constexpr T deg_to_rad = core::math::deg_to_rad<T>;

	template <Number T>
	inline constexpr T rad_to_deg = core::math::rad_to_deg<T>;

	template <Number T>
	inline constexpr T infinity = core::math::infinity<T>;

	template <Number T>
	inline constexpr T epsilon = core::math::epsilon<T>;

	template <Number T>
	inline constexpr T nan = core::math::nan<T>;
} // namespace op::math