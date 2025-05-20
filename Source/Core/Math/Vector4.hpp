/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Math/Vector3.hpp>

namespace Forge::Core::Math {
	template <typename T>
	struct Vector4 {
		T x, y, z, w;

		constexpr FORGE_ALWAYS_INLINE Vector4() : x{ 0 }, y{ 0 }, z{ 0 }, w{ 0 } {}
		constexpr FORGE_ALWAYS_INLINE Vector4(T xyzw) : x{ xyzw }, y{ xyzw }, z{ xyzw }, w{ xyzw } {}
		constexpr FORGE_ALWAYS_INLINE Vector4(T _x, T _y, T _z, T _w) : x{ _x }, y{ _y }, z{ _z }, w{ _w } {}
		constexpr FORGE_ALWAYS_INLINE Vector4(const Vector3<T>& xyz, T _w)
			: x{ xyz.x }
			, y{ xyz.y }
			, z{ xyz.z }
			, w{ _w } {}

		FORGE_ALWAYS_INLINE Vector4 operator+(const Vector4& rhs) const {
			return { x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w };
		}
		FORGE_ALWAYS_INLINE Vector4 operator-(const Vector4& rhs) const {
			return { x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w };
		}
		FORGE_ALWAYS_INLINE Vector4 operator*(const Vector4& rhs) const {
			return { x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w };
		}
		FORGE_ALWAYS_INLINE Vector4 operator/(const Vector4& rhs) const {
			return { x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.w };
		}
		FORGE_ALWAYS_INLINE void operator+=(const Vector4& rhs) { *this = *this + rhs; }
		FORGE_ALWAYS_INLINE void operator-=(const Vector4& rhs) { *this = *this - rhs; }
		FORGE_ALWAYS_INLINE void operator*=(const Vector4& rhs) { *this = *this * rhs; }
		FORGE_ALWAYS_INLINE void operator/=(const Vector4& rhs) { *this = *this / rhs; }
		FORGE_ALWAYS_INLINE Vector4 operator-() const { return { -x, -y, -z, -w }; }

		/**
		 * @returns the cos of the angle between the two vectors.
		 */
		FORGE_ALWAYS_INLINE T dot(const Vector4& rhs) const { return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w; }

		/**
		 * @returns the length of the vector squared.
		 */
		FORGE_ALWAYS_INLINE T len_sq() const { return dot(*this); }

		/**
		 * @returns the length of the vector.
		 */
		FORGE_ALWAYS_INLINE T len() const { return sqrt(len_sq()); }

		/**
		 * @returns an option containing the normalized vector if the length is not zero.
		 */
		FORGE_ALWAYS_INLINE Option<Vector4> normalized() const
			requires FloatingPoint<T>
		{
			if (len_sq() >= kinda_small_number<T>) {
				return *this / len();
			}
			return nullopt;
		}

		FORGE_NO_DISCARD FORGE_ALWAYS_INLINE bool contains(T value) const {
			return x == value || y == value || z == value || w == value;
		}
		FORGE_NO_DISCARD FORGE_ALWAYS_INLINE bool
		equals(const Vector4& rhs, T tolerance = kinda_small_number<T>) const {
			return Math::equals(x, rhs.x, tolerance) && Math::equals(y, rhs.y, tolerance) &&
				   Math::equals(z, rhs.z, tolerance) && Math::equals(w, rhs.w, tolerance);
		}

		FORGE_NO_DISCARD FORGE_ALWAYS_INLINE Vector4 min(const Vector4& rhs) const {
			return { Math::min(x, rhs.x), Math::min(y, rhs.y), Math::min(z, rhs.z), Math::min(w, rhs.w) };
		}

		FORGE_NO_DISCARD FORGE_ALWAYS_INLINE Vector4 max(const Vector4& rhs) const {
			return { Math::max(x, rhs.x), Math::max(y, rhs.y), Math::max(z, rhs.z), Math::max(w, rhs.w) };
		}

		FORGE_NO_DISCARD FORGE_ALWAYS_INLINE Vector3<T> xyz() const { return { x, y, z }; }

		/**
		 * Cast the vector to another type.
		 *
		 * @tparam D The type to cast to.
		 * @return A new vector with the casted type.
		 */
		template <typename D>
		FORGE_NO_DISCARD FORGE_ALWAYS_INLINE Vector4<D> as() const {
			return Vector4<D>{ static_cast<D>(x), static_cast<D>(y), static_cast<D>(z), static_cast<D>(w) };
		}
	};
} // namespace Forge::Core::Math

namespace Forge {
	using Core::Math::Vector4;
} // namespace Forge
