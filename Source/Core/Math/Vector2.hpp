/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Containers/Option.hpp>
#include <Core/Math/Math.hpp>

namespace Mach::Core::Math {
	template <Number T>
	struct Vector2 {
		T x, y;

		constexpr MACH_ALWAYS_INLINE Vector2() : x{ 0 }, y{ 0 } {}
		constexpr MACH_ALWAYS_INLINE Vector2(T xy) : x{ xy }, y{ xy } {}
		constexpr MACH_ALWAYS_INLINE Vector2(T _x, T _y) : x{ _x }, y{ _y } {}

		MACH_ALWAYS_INLINE static Vector2<T> from_rad(T theta)
			requires FloatingPoint<T>
		{
			return Vector2<T>{ cos(theta), sin(theta) };
		}

		MACH_ALWAYS_INLINE Vector2 operator+(const Vector2& rhs) const { return { x + rhs.x, y + rhs.y }; }
		MACH_ALWAYS_INLINE Vector2 operator-(const Vector2& rhs) const { return { x - rhs.x, y - rhs.y }; }
		MACH_ALWAYS_INLINE Vector2 operator*(const Vector2& rhs) const { return { x * rhs.x, y * rhs.y }; }
		MACH_ALWAYS_INLINE Vector2 operator/(const Vector2& rhs) const { return { x / rhs.x, y / rhs.y }; }
		MACH_ALWAYS_INLINE void operator+=(const Vector2& rhs) { *this = *this + rhs; }
		MACH_ALWAYS_INLINE void operator-=(const Vector2& rhs) { *this = *this - rhs; }
		MACH_ALWAYS_INLINE void operator*=(const Vector2& rhs) { *this = *this * rhs; }
		MACH_ALWAYS_INLINE void operator/=(const Vector2& rhs) { *this = *this / rhs; }
		MACH_ALWAYS_INLINE Vector2 operator-() const { return { -x, -y }; }

		/**
		 * @returns the cos of the angle between the two vectors.
		 */
		MACH_ALWAYS_INLINE T dot(const Vector2& rhs) const { return x * rhs.x + y * rhs.y; }

		/**
		 * @return the sin of the angle between the two vectors.
		 */
		MACH_ALWAYS_INLINE T cross(const Vector2& rhs) const { return x * rhs.y - y * rhs.x; }

		/**
		 * @returns a new vector that is perpendicular to this vector.
		 */
		MACH_ALWAYS_INLINE Vector2 perp() const { return { y, -x }; }

		/**
		 * @returns the length of the vector squared.
		 */
		MACH_ALWAYS_INLINE T len_sq() const { return dot(*this); }

		/**
		 * @returns the length of the vector.
		 */
		MACH_ALWAYS_INLINE T len() const { return sqrt(len_sq()); }

		/**
		 * @returns an option containing the normalized vector if the length is not zero.
		 */
		MACH_ALWAYS_INLINE Option<Vector2> normalized() const
			requires FloatingPoint<T>
		{
			if (len_sq() >= kinda_small_number<T>) {
				return *this / len();
			}
			return nullopt;
		}

		MACH_NO_DISCARD MACH_ALWAYS_INLINE bool contains(T value) const { return x == value || y == value; }
		MACH_NO_DISCARD MACH_ALWAYS_INLINE bool equals(const Vector2& rhs, T tolerance = kinda_small_number<T>) const {
			return Math::equals(x, rhs.x, tolerance) && Math::equals(y, rhs.y, tolerance);
		}

		MACH_NO_DISCARD Vector2 min(const Vector2& rhs) const { return { Math::min(x, rhs.x), Math::min(y, rhs.y) }; }

		MACH_NO_DISCARD Vector2 max(const Vector2& rhs) const { return { Math::max(x, rhs.x), Math::max(y, rhs.y) }; }

		/**
		 * Cast the vector to another type.
		 *
		 * @tparam D The type to cast to.
		 * @return A new vector with the casted type.
		 */
		template <typename D>
		MACH_NO_DISCARD MACH_ALWAYS_INLINE Vector2<D> as() const {
			return Vector2<D>{ static_cast<D>(x), static_cast<D>(y) };
		}
	};
} // namespace Mach::Core::Math

namespace Mach {
	using Core::Math::Vector2;
} // namespace Mach
