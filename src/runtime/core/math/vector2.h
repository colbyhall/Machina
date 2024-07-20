/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include "core/containers/option.h"
#include "core/math/math.h"

namespace op::core {
	template <Number T>
	struct Vector2 {
		T x, y;

		constexpr OP_ALWAYS_INLINE Vector2() : x{ 0 }, y{ 0 } {}
		constexpr OP_ALWAYS_INLINE Vector2(T xy) : x{ xy }, y{ xy } {}
		constexpr OP_ALWAYS_INLINE explicit Vector2(T _x, T _y) : x{ _x }, y{ _y } {}

		OP_ALWAYS_INLINE static Vector2<T> from_rad(T theta)
			requires FloatingPoint<T>
		{
			return Vector2<T>{ op::cos(theta), op::sin(theta) };
		}

		OP_ALWAYS_INLINE Vector2 operator+(const Vector2& rhs) const { return Vector2{ x + rhs.x, y + rhs.y }; }
		OP_ALWAYS_INLINE Vector2 operator-(const Vector2& rhs) const { return Vector2{ x - rhs.x, y - rhs.y }; }
		OP_ALWAYS_INLINE Vector2 operator*(const Vector2& rhs) const { return Vector2{ x * rhs.x, y * rhs.y }; }
		OP_ALWAYS_INLINE Vector2 operator/(const Vector2& rhs) const { return Vector2{ x / rhs.x, y / rhs.y }; }
		OP_ALWAYS_INLINE void operator+=(const Vector2& rhs) { *this = *this + rhs; }
		OP_ALWAYS_INLINE void operator-=(const Vector2& rhs) { *this = *this - rhs; }
		OP_ALWAYS_INLINE void operator*=(const Vector2& rhs) { *this = *this * rhs; }
		OP_ALWAYS_INLINE void operator/=(const Vector2& rhs) { *this = *this / rhs; }
		OP_ALWAYS_INLINE Vector2 operator-() const { return Vector2{ -x, -y }; }

		/**
		 * @return the cos of the angle between the two vectors.
		 */
		OP_ALWAYS_INLINE T dot(const Vector2<T>& rhs) const { return x * rhs.x + y * rhs.y; }

		/**
		 * @return the sin of the angle between the two vectors.
		 */
		OP_ALWAYS_INLINE T cross(const Vector2<T>& rhs) const { return x * rhs.y - y * rhs.x; }

		/**
		 * @returns a new vector that is perpendicular to this vector.
		 */
		OP_ALWAYS_INLINE Vector2<T> perp() const { return Vector2{ y, -x }; }

		/**
		 * @returns the length of the vector squared.
		 */
		OP_ALWAYS_INLINE T len_sq() const { return dot(*this); }

		/**
		 * @returns the length of the vector.
		 */
		OP_ALWAYS_INLINE T len() const { return op::sqrt(len_sq()); }

		/**
		 * @returns an option containing the normalized vector if the length is not zero.
		 */
		OP_ALWAYS_INLINE Option<Vector2<T>> normalized() const
			requires FloatingPoint<T>
		{
			if (len_sq() >= kinda_small_number<T>) {
				return *this / len();
			}
			return nullopt;
		}

		OP_NO_DISCARD OP_ALWAYS_INLINE bool contains(T value) const { return x == value || y == value; }
		OP_NO_DISCARD OP_ALWAYS_INLINE bool equals(const Vector2<T>& rhs, T tolerance = kinda_small_number<T>) const {
			return core::equals(x, rhs.x, tolerance) && core::equals(y, rhs.y, tolerance);
		}

		/**
		 * Returns the minimum elements of two vectors.
		 *
		 * @param rhs The other vector to compare against.
		 * @return The minimum elements of the two vectors.
		 */
		OP_NO_DISCARD Vector2<T> min(const Vector2<T>& rhs) const {
			return Vector2{ op::min(x, rhs.x), op::min(y, rhs.y) };
		}

		/**
		 * Returns the maximum elements of two vectors.
		 *
		 * @param rhs The other vector to compare against.
		 * @return The maximum elements of the two vectors.
		 */
		OP_NO_DISCARD Vector2<T> max(const Vector2<T>& rhs) const {
			return Vector2{ op::max(x, rhs.x), op::max(y, rhs.y) };
		}

		/**
		 * Cast the vector to another type.
		 *
		 * @tparam D The type to cast to.
		 * @return A new vector with the casted type.
		 */
		template <typename D>
		OP_NO_DISCARD OP_ALWAYS_INLINE Vector2<D> as() const {
			return Vector2<D>{ static_cast<D>(x), static_cast<D>(y) };
		}
	};
} // namespace op::core

namespace op {
	using core::Vector2;
} // namespace op