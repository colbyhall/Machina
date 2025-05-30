/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Math/Vector2.hpp>

namespace Mach::Core::Math {
	template <Number T>
	struct alignas(16) Vector3 {
		T x, y, z;

		constexpr MACH_ALWAYS_INLINE Vector3() : x{ 0 }, y{ 0 }, z{ 0 } {}
		constexpr MACH_ALWAYS_INLINE Vector3(T xyz) : x{ xyz }, y{ xyz }, z{ xyz } {}
		constexpr MACH_ALWAYS_INLINE Vector3(T _x, T _y, T _z) : x{ _x }, y{ _y }, z{ _z } {}
		constexpr MACH_ALWAYS_INLINE Vector3(const Vector2<T>& xy, T _z) : x{ xy.x }, y{ xy.y }, z{ _z } {}

		MACH_ALWAYS_INLINE Vector3 operator+(const Vector3& rhs) const { return { x + rhs.x, y + rhs.y, z + rhs.z }; }
		MACH_ALWAYS_INLINE Vector3 operator-(const Vector3& rhs) const { return { x - rhs.x, y - rhs.y, z - rhs.z }; }
		MACH_ALWAYS_INLINE Vector3 operator*(const Vector3& rhs) const { return { x * rhs.x, y * rhs.y, z * rhs.z }; }
		MACH_ALWAYS_INLINE Vector3 operator/(const Vector3& rhs) const { return { x / rhs.x, y / rhs.y, z / rhs.z }; }
		MACH_ALWAYS_INLINE void operator+=(const Vector3& rhs) { *this = *this + rhs; }
		MACH_ALWAYS_INLINE void operator-=(const Vector3& rhs) { *this = *this - rhs; }
		MACH_ALWAYS_INLINE void operator*=(const Vector3& rhs) { *this = *this * rhs; }
		MACH_ALWAYS_INLINE void operator/=(const Vector3& rhs) { *this = *this / rhs; }
		MACH_ALWAYS_INLINE Vector3 operator-() const { return { -x, -y, -z }; }

		/**
		 * @returns the cos of the angle between the two vectors.
		 */
		MACH_ALWAYS_INLINE T dot(const Vector3& rhs) const { return x * rhs.x + y * rhs.y; }

		/**
		 * @returns the perpendicular vector to the two vectors.
		 */
		MACH_ALWAYS_INLINE Vector3 cross(const Vector3& rhs) const {
			return { y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x };
		}

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
		MACH_ALWAYS_INLINE Option<Vector3> normalized() const
			requires FloatingPoint<T>
		{
			if (len_sq() >= kinda_small_number<T>) {
				return *this / len();
			}
			return nullopt;
		}

		MACH_NO_DISCARD MACH_ALWAYS_INLINE bool contains(T value) const {
			return x == value || y == value || z == value;
		}
		MACH_NO_DISCARD MACH_ALWAYS_INLINE bool equals(const Vector3& rhs, T tolerance = kinda_small_number<T>) const {
			return Math::equals(x, rhs.x, tolerance) && Math::equals(y, rhs.y, tolerance) &&
				   Math::equals(z, rhs.z, tolerance);
		}

		MACH_NO_DISCARD MACH_ALWAYS_INLINE Vector3 min(const Vector3& rhs) const {
			return { Math::min(x, rhs.x), Math::min(y, rhs.y), Math::min(z, rhs.z) };
		}

		MACH_NO_DISCARD MACH_ALWAYS_INLINE Vector3 max(const Vector3& rhs) const {
			return { Math::max(x, rhs.x), Math::max(y, rhs.y), Math::max(z, rhs.z) };
		}

		MACH_NO_DISCARD MACH_ALWAYS_INLINE Vector2<T> xy() const { return { x, y }; }

		/**
		 * Cast the vector to another type.
		 *
		 * @tparam D The type to cast to.
		 * @return A new vector with the casted type.
		 */
		template <typename D>
		MACH_NO_DISCARD MACH_ALWAYS_INLINE Vector3<D> as() const {
			return Vector3<D>{ static_cast<D>(x), static_cast<D>(y), static_cast<D>(z) };
		}
	};
} // namespace Mach::Core::Math

namespace Mach {
	using Core::Math::Vector3;
} // namespace Mach
