/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Containers/Option.hpp>
#include <Core/Math/Math.hpp>

namespace Grizzly::Core::Math {
	template <Number T>
	struct Vector2 {
		T x, y;

		constexpr GRIZZLY_ALWAYS_INLINE Vector2() : x{ 0 }, y{ 0 } {}
		constexpr GRIZZLY_ALWAYS_INLINE Vector2(T xy) : x{ xy }, y{ xy } {}
		constexpr GRIZZLY_ALWAYS_INLINE Vector2(T _x, T _y) : x{ _x }, y{ _y } {}

		GRIZZLY_ALWAYS_INLINE static Vector2<T> from_rad(T theta)
			requires FloatingPoint<T>
		{
			return Vector2<T>{ cos(theta), sin(theta) };
		}

		GRIZZLY_ALWAYS_INLINE Vector2 operator+(const Vector2& rhs) const { return { x + rhs.x, y + rhs.y }; }
		GRIZZLY_ALWAYS_INLINE Vector2 operator-(const Vector2& rhs) const { return { x - rhs.x, y - rhs.y }; }
		GRIZZLY_ALWAYS_INLINE Vector2 operator*(const Vector2& rhs) const { return { x * rhs.x, y * rhs.y }; }
		GRIZZLY_ALWAYS_INLINE Vector2 operator/(const Vector2& rhs) const { return { x / rhs.x, y / rhs.y }; }
		GRIZZLY_ALWAYS_INLINE void operator+=(const Vector2& rhs) { *this = *this + rhs; }
		GRIZZLY_ALWAYS_INLINE void operator-=(const Vector2& rhs) { *this = *this - rhs; }
		GRIZZLY_ALWAYS_INLINE void operator*=(const Vector2& rhs) { *this = *this * rhs; }
		GRIZZLY_ALWAYS_INLINE void operator/=(const Vector2& rhs) { *this = *this / rhs; }
		GRIZZLY_ALWAYS_INLINE Vector2 operator-() const { return { -x, -y }; }

		/**
		 * @returns the cos of the angle between the two vectors.
		 */
		GRIZZLY_ALWAYS_INLINE T dot(const Vector2& rhs) const { return x * rhs.x + y * rhs.y; }

		/**
		 * @return the sin of the angle between the two vectors.
		 */
		GRIZZLY_ALWAYS_INLINE T cross(const Vector2& rhs) const { return x * rhs.y - y * rhs.x; }

		/**
		 * @returns a new vector that is perpendicular to this vector.
		 */
		GRIZZLY_ALWAYS_INLINE Vector2 perp() const { return { y, -x }; }

		/**
		 * @returns the length of the vector squared.
		 */
		GRIZZLY_ALWAYS_INLINE T len_sq() const { return dot(*this); }

		/**
		 * @returns the length of the vector.
		 */
		GRIZZLY_ALWAYS_INLINE T len() const { return sqrt(len_sq()); }

		/**
		 * @returns an option containing the normalized vector if the length is not zero.
		 */
		GRIZZLY_ALWAYS_INLINE Option<Vector2> normalized() const
			requires FloatingPoint<T>
		{
			if (len_sq() >= kinda_small_number<T>) {
				return *this / len();
			}
			return nullopt;
		}

		GRIZZLY_NO_DISCARD GRIZZLY_ALWAYS_INLINE bool contains(T value) const { return x == value || y == value; }
		GRIZZLY_NO_DISCARD GRIZZLY_ALWAYS_INLINE bool
		equals(const Vector2& rhs, T tolerance = kinda_small_number<T>) const {
			return Math::equals(x, rhs.x, tolerance) && Math::equals(y, rhs.y, tolerance);
		}

		GRIZZLY_NO_DISCARD Vector2 min(const Vector2& rhs) const {
			return { Math::min(x, rhs.x), Math::min(y, rhs.y) };
		}

		GRIZZLY_NO_DISCARD Vector2 max(const Vector2& rhs) const {
			return { Math::max(x, rhs.x), Math::max(y, rhs.y) };
		}

		/**
		 * Cast the vector to another type.
		 *
		 * @tparam D The type to cast to.
		 * @return A new vector with the casted type.
		 */
		template <typename D>
		GRIZZLY_NO_DISCARD GRIZZLY_ALWAYS_INLINE Vector2<D> as() const {
			return Vector2<D>{ static_cast<D>(x), static_cast<D>(y) };
		}
	};
} // namespace Grizzly::Core::Math

namespace Grizzly {
	using Core::Math::Vector2;
} // namespace Grizzly
