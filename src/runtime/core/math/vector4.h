/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include "core/math/vector3.h"

namespace op::core::math {
	template <typename T>
	struct Vector4 {
		T x, y, z, w;

		constexpr OP_ALWAYS_INLINE Vector4() : x{ 0 }, y{ 0 }, z{ 0 }, w{ 0 } {}
		constexpr OP_ALWAYS_INLINE Vector4(T xyzw) : x{ xyzw }, y{ xyzw }, z{ xyzw }, w{ xyzw } {}
		constexpr OP_ALWAYS_INLINE Vector4(T _x, T _y, T _z, T _w) : x{ _x }, y{ _y }, z{ _z }, w{ _w } {}
		constexpr OP_ALWAYS_INLINE Vector4(const Vector3<T>& xyz, T _w) : x{ xyz.x }, y{ xyz.y }, z{ xyz.z }, w{ _w } {}

		OP_ALWAYS_INLINE Vector4 operator+(const Vector4& rhs) const {
			return { x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w };
		}
		OP_ALWAYS_INLINE Vector4 operator-(const Vector4& rhs) const {
			return { x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w };
		}
		OP_ALWAYS_INLINE Vector4 operator*(const Vector4& rhs) const {
			return { x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w };
		}
		OP_ALWAYS_INLINE Vector4 operator/(const Vector4& rhs) const {
			return { x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.w };
		}
		OP_ALWAYS_INLINE void operator+=(const Vector4& rhs) { *this = *this + rhs; }
		OP_ALWAYS_INLINE void operator-=(const Vector4& rhs) { *this = *this - rhs; }
		OP_ALWAYS_INLINE void operator*=(const Vector4& rhs) { *this = *this * rhs; }
		OP_ALWAYS_INLINE void operator/=(const Vector4& rhs) { *this = *this / rhs; }
		OP_ALWAYS_INLINE Vector4 operator-() const { return { -x, -y, -z, -w }; }

		/**
		 * @returns the cos of the angle between the two vectors.
		 */
		OP_ALWAYS_INLINE T dot(const Vector4& rhs) const { return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w; }

		/**
		 * @returns the length of the vector squared.
		 */
		OP_ALWAYS_INLINE T len_sq() const { return dot(*this); }

		/**
		 * @returns the length of the vector.
		 */
		OP_ALWAYS_INLINE T len() const { return sqrt(len_sq()); }

		/**
		 * @returns an option containing the normalized vector if the length is not zero.
		 */
		OP_ALWAYS_INLINE Option<Vector4> normalized() const
			requires FloatingPoint<T>
		{
			if (len_sq() >= kinda_small_number<T>) {
				return *this / len();
			}
			return nullopt;
		}

		OP_NO_DISCARD OP_ALWAYS_INLINE bool contains(T value) const {
			return x == value || y == value || z == value || w == value;
		}
		OP_NO_DISCARD OP_ALWAYS_INLINE bool equals(const Vector4& rhs, T tolerance = kinda_small_number<T>) const {
			return math::equals(x, rhs.x, tolerance) && math::equals(y, rhs.y, tolerance) &&
				   math::equals(z, rhs.z, tolerance) && math::equals(w, rhs.w, tolerance);
		}

		OP_NO_DISCARD OP_ALWAYS_INLINE Vector4 min(const Vector4& rhs) const {
			return { math::min(x, rhs.x), math::min(y, rhs.y), math::min(z, rhs.z), math::min(w, rhs.w) };
		}

		OP_NO_DISCARD OP_ALWAYS_INLINE Vector4 max(const Vector4& rhs) const {
			return { math::max(x, rhs.x), math::max(y, rhs.y), math::max(z, rhs.z), math::max(w, rhs.w) };
		}

		OP_NO_DISCARD OP_ALWAYS_INLINE Vector3<T> xyz() const { return { x, y, z }; }

		/**
		 * Cast the vector to another type.
		 *
		 * @tparam D The type to cast to.
		 * @return A new vector with the casted type.
		 */
		template <typename D>
		OP_NO_DISCARD OP_ALWAYS_INLINE Vector4<D> as() const {
			return Vector4<D>{ static_cast<D>(x), static_cast<D>(y), static_cast<D>(z), static_cast<D>(w) };
		}
	};
} // namespace op::core::math

namespace op {
	using core::math::Vector4;
} // namespace op