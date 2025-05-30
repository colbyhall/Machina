/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Concepts.hpp>
#include <Core/Core.hpp>
#include <Core/Primitives.hpp>

#include <limits>

namespace Mach::Core::Math {
	template <typename T>
	concept Number = FloatingPoint<T> || Integral<T>;

	template <Number T>
	constexpr MACH_ALWAYS_INLINE T min(T a, T b) {
		return a < b ? a : b;
	}

	template <Number T>
	constexpr MACH_ALWAYS_INLINE T max(T a, T b) {
		return a > b ? a : b;
	}

	template <Number T>
	constexpr MACH_ALWAYS_INLINE T clamp(T value, T min, T max) {
		return Math::min<T>(Math::max<T>(value, min), max);
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

	MACH_NO_DISCARD f32 cos(f32 x);
	MACH_NO_DISCARD f64 cos(f64 x);

	MACH_NO_DISCARD f32 sin(f32 x);
	MACH_NO_DISCARD f64 sin(f64 x);

	MACH_NO_DISCARD f32 tan(f32 x);
	MACH_NO_DISCARD f64 tan(f64 x);

	MACH_NO_DISCARD f32 acos(f32 x);
	MACH_NO_DISCARD f64 acos(f64 x);

	MACH_NO_DISCARD f32 asin(f32 x);
	MACH_NO_DISCARD f64 asin(f64 x);

	MACH_NO_DISCARD f32 atan(f32 x);
	MACH_NO_DISCARD f64 atan(f64 x);

	MACH_NO_DISCARD f32 atan2(f32 y, f32 x);
	MACH_NO_DISCARD f64 atan2(f64 y, f64 x);

	MACH_NO_DISCARD f32 sqrt(f32 x);
	MACH_NO_DISCARD f64 sqrt(f64 x);

	MACH_NO_DISCARD f32 fmod(f32 numerator, f32 denominator);
	MACH_NO_DISCARD f64 fmod(f64 numerator, f64 denominator);

	MACH_NO_DISCARD f32 powf(f32 x, f32 y);
	MACH_NO_DISCARD f64 pow(f64 x, f64 y);

	template <Number T>
	constexpr MACH_ALWAYS_INLINE T abs(T x) {
		return x < 0 ? -x : x;
	}

	template <FloatingPoint T>
	constexpr MACH_ALWAYS_INLINE bool is_near_zero(T a) {
		return abs<T>(a) <= small_number<T>;
	}

	template <FloatingPoint T>
	constexpr MACH_ALWAYS_INLINE T lerp(T a, T b, T t) {
		return (T{ 1 } - t) * a + t * b;
	}

	template <Number T>
	constexpr MACH_ALWAYS_INLINE T square(T value) {
		return value * value;
	}

	template <Number T>
	constexpr MACH_ALWAYS_INLINE T equals(T a, T b, T tolerance = kinda_small_number<T>) {
		return abs<T>(a - b) <= tolerance;
	}
} // namespace Mach::Core::Math

namespace Mach::Math {
	using Core::Math::Number;

	using Core::Math::abs;
	using Core::Math::acos;
	using Core::Math::asin;
	using Core::Math::atan;
	using Core::Math::atan2;
	using Core::Math::clamp;
	using Core::Math::cos;
	using Core::Math::equals;
	using Core::Math::fmod;
	using Core::Math::lerp;
	using Core::Math::max;
	using Core::Math::min;
	using Core::Math::sin;
	using Core::Math::sqrt;
	using Core::Math::square;
	using Core::Math::tan;

	template <Number T>
	inline constexpr T pi = Core::Math::pi<T>;

	template <Number T>
	inline constexpr T tau = Core::Math::tau<T>;

	template <Number T>
	inline constexpr T deg_to_rad = Core::Math::deg_to_rad<T>;

	template <Number T>
	inline constexpr T rad_to_deg = Core::Math::rad_to_deg<T>;

	template <Number T>
	inline constexpr T infinity = Core::Math::infinity<T>;

	template <Number T>
	inline constexpr T epsilon = Core::Math::epsilon<T>;

	template <Number T>
	inline constexpr T nan = Core::Math::nan<T>;
} // namespace Mach::Math
