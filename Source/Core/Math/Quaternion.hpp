/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Math/Vector3.hpp>

namespace Forge::Core::Math {
	/**
	 * @class Quaternion
	 * @brief A template class for representing and manipulating quaternions.
	 *
	 * @tparam T A floating-point type specifying the precision of the quaternion components.
	 */
	template <FloatingPoint T>
	struct Quaternion {
		T x, y, z, w;

		/**
		 * @brief Default constructor. Initializes the quaternion to the identity rotation (no rotation).
		 */
		FORGE_ALWAYS_INLINE constexpr Quaternion() : x(0), y(0), z(0), w(1) {}

		/**
		 * @brief Parameterized constructor for directly setting the quaternion's components.
		 * @param _x The x component of the quaternion.
		 * @param _y The y component of the quaternion.
		 * @param _z The z component of the quaternion.
		 * @param _w The w component of the quaternion (real part).
		 */
		FORGE_ALWAYS_INLINE explicit Quaternion(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) {}

		/**
		 * @brief Creates a quaternion from an axis and an angle.
		 * @param axis The axis of rotation.
		 * @param theta The angle of rotation in radians.
		 * @return A quaternion representing the specified rotation.
		 */
		static Quaternion from_axis_angle(const Vector3<T>& axis, T theta);

		/**
		 * @brief Creates a quaternion from Euler angles.
		 * @param pitch The pitch angle in degrees.
		 * @param yaw The yaw angle in degrees.
		 * @param roll The roll angle in degrees.
		 * @return A quaternion representing the specified rotation.
		 */
		static Quaternion from_euler(T pitch, T yaw, T roll);

		/**
		 * @brief Returns the identity quaternion (no rotation).
		 * @return The identity quaternion.
		 */
		static constexpr Quaternion identity();

		/**
		 * @brief Calculates the squared length of the quaternion.
		 * @return The squared length of the quaternion.
		 */
		FORGE_NO_DISCARD FORGE_ALWAYS_INLINE T len_sq() const;

		/**
		 * @brief Calculates the length of the quaternion.
		 * @return The length of the quaternion.
		 */
		FORGE_NO_DISCARD FORGE_ALWAYS_INLINE T len() const;

		/**
		 * @brief Normalizes the quaternion.
		 * @return An optional containing the normalized quaternion if it has a non-zero length, otherwise an empty
		 * optional.
		 */
		FORGE_NO_DISCARD FORGE_ALWAYS_INLINE Option<Quaternion> normalized() const;

		/**
		 * @brief Calculates the inverse of the quaternion.
		 * @return The inverse of the quaternion.
		 */
		FORGE_NO_DISCARD FORGE_ALWAYS_INLINE Quaternion inverse() const;

		/**
		 * @brief Rotates a vector by the quaternion.
		 * @param xyz The vector to rotate.
		 * @return The rotated vector.
		 */
		FORGE_NO_DISCARD Vector3<T> rotate(const Vector3<T>& xyz) const;

		/**
		 * @brief Multiplies this quaternion by another quaternion.
		 * @param rhs The right-hand side quaternion to multiply with.
		 * @return The result of the quaternion multiplication.
		 */
		FORGE_NO_DISCARD FORGE_ALWAYS_INLINE Quaternion operator*(const Quaternion& rhs) const;
	};

	template <FloatingPoint T>
	Quaternion<T> Quaternion<T>::from_axis_angle(const Vector3<T>& axis, T theta) {
		const auto half_theta = theta / 2;
		const auto s = Math::sin(half_theta);
		const auto c = Math::cos(half_theta);
		return Quaternion{ axis.x * s, axis.y * s, axis.z * s, c };
	}

	template <FloatingPoint T>
	Quaternion<T> Quaternion<T>::from_euler(T pitch, T yaw, T roll) {
		const auto rads_div_by_2 = Math::deg_to_rad<T> / T{ 2 };

		const auto pitch1 = Math::fmod(pitch, T{ 360 });
		const auto yaw1 = Math::fmod(yaw, T{ 360 });
		const auto roll1 = Math::fmod(roll, T{ 360 });

		const auto sp = Math::sin(pitch1 * rads_div_by_2);
		const auto cp = Math::cos(pitch1 * rads_div_by_2);

		const auto sy = Math::sin(yaw1 * rads_div_by_2);
		const auto cy = Math::cos(yaw1 * rads_div_by_2);

		const auto sr = Math::sin(roll1 * rads_div_by_2);
		const auto cr = Math::cos(roll1 * rads_div_by_2);

		return Quaternion{ cr * sp * sy - sr * cp * cy,
						   -cr * sp * cy - sr * cp * sy,
						   cr * cp * sy - sr * sp * cy,
						   cr * cp * cy + sr * sp * sy };
	}

	template <FloatingPoint T>
	constexpr Quaternion<T> Quaternion<T>::identity() {
		return Quaternion{};
	}

	template <FloatingPoint T>
	FORGE_NO_DISCARD FORGE_ALWAYS_INLINE T Quaternion<T>::len_sq() const {
		return x * x + y * y + z * z + w * w;
	}

	template <FloatingPoint T>
	FORGE_NO_DISCARD FORGE_ALWAYS_INLINE T Quaternion<T>::len() const {
		return Math::sqrt(len_sq());
	}

	template <FloatingPoint T>
	FORGE_NO_DISCARD FORGE_ALWAYS_INLINE Option<Quaternion<T>> Quaternion<T>::normalized() const {
		if (len_sq() >= kinda_small_number<T>) {
			const auto len_reciprocal = T{ 1 } / len();
			return Quaternion{ x * len_reciprocal, y * len_reciprocal, z * len_reciprocal, w * len_reciprocal };
		}
		return nullopt;
	}

	template <FloatingPoint T>
	FORGE_NO_DISCARD FORGE_ALWAYS_INLINE Quaternion<T> Quaternion<T>::inverse() const {
		return Quaternion{ -x, -y, -z, w };
	}

	template <FloatingPoint T>
	FORGE_NO_DISCARD Vector3<T> Quaternion<T>::rotate(const Vector3<T>& xyz) const {
		const Vector3<T> this_xyz = { x, y, z };
		const auto t = this_xyz.cross(xyz) * T{ 2 };
		return xyz + (t * w) + this_xyz.cross(t);
	}

	template <FloatingPoint T>
	FORGE_NO_DISCARD FORGE_ALWAYS_INLINE Quaternion<T> Quaternion<T>::operator*(const Quaternion<T>& rhs) const {
		return Quaternion{ w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y,
						   w * rhs.y + y * rhs.w + z * rhs.x - x * rhs.z,
						   w * rhs.z + z * rhs.w + x * rhs.y - y * rhs.x,
						   w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z };
	}
} // namespace Forge::Core::Math

namespace Forge {
	using Core::Math::Quaternion;
} // namespace Forge
