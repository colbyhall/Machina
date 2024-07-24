/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include "core/math/vector3.h"

namespace op::core::math {
	template <FloatingPoint T>
	struct Quaternion {
		T x, y, z, w;

		OP_ALWAYS_INLINE constexpr Quaternion() : x(0), y(0), z(0), w(1) {}
		OP_ALWAYS_INLINE explicit Quaternion(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) {}
		static Quaternion from_axis_angle(const Vector3<T>& axis, T theta) {
			const auto half_theta = theta / 2;
			const auto s = math::sin(half_theta);
			const auto c = math::cos(half_theta);
			return Quaternion{ axis.x * s, axis.y * s, axis.z * s, c };
		}
		static Quaternion from_euler(T pitch, T yaw, T roll) {
			const auto rads_div_by_2 = math::deg_to_rad<T> / T{ 2 };

			const auto pitch1 = math::fmod(pitch, T{ 360 });
			const auto yaw1 = math::fmod(yaw, T{ 360 });
			const auto roll1 = math::fmod(roll, T{ 360 });

			const auto sp = math::sin(pitch1 * rads_div_by_2);
			const auto cp = math::cos(pitch1 * rads_div_by_2);

			const auto sy = math::sin(yaw1 * rads_div_by_2);
			const auto cy = math::cos(yaw1 * rads_div_by_2);

			const auto sr = math::sin(roll1 * rads_div_by_2);
			const auto cr = math::cos(roll1 * rads_div_by_2);

			return Quaternion{ cr * sp * sy - sr * cp * cy,
							   -cr * sp * cy - sr * cp * sy,
							   cr * cp * sy - sr * sp * cy,
							   cr * cp * cy + sr * sp * sy };
		}
		static constexpr Quaternion identity() { return Quaternion{}; }

		OP_NO_DISCARD OP_ALWAYS_INLINE T len_sq() const { return x * x + y * y + z * z + w * w; }
		OP_NO_DISCARD OP_ALWAYS_INLINE T len() const { return math::sqrt(len_sq()); }

		OP_NO_DISCARD OP_ALWAYS_INLINE Option<Quaternion> normalized() const {
			if (len_sq() >= kinda_small_number<T>) {
				const auto inverse_len = T{ 1 } / len();
				return Quaternion{ x * inverse_len, y * inverse_len, z * inverse_len, w * inverse_len };
			}
			return nullopt;
		}
		OP_NO_DISCARD OP_ALWAYS_INLINE Quaternion inverse() const { return Quaternion{ -x, -y, -z, w }; }
		OP_NO_DISCARD Vector3<T> rotate(const Vector3<T>& xyz) const {
			const Vector3<T> this_xyz = { x, y, z };
			const auto t = this_xyz.cross(xyz) * T{ 2 };
			return xyz + (t * w) + this_xyz.cross(t);
		}

		OP_NO_DISCARD OP_ALWAYS_INLINE Quaternion operator*(const Quaternion& rhs) const {
			return Quaternion{ w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y,
							   w * rhs.y + y * rhs.w + z * rhs.x - x * rhs.z,
							   w * rhs.z + z * rhs.w + x * rhs.y - y * rhs.x,
							   w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z };
		}
	};
} // namespace op::core::math

namespace op {
	using core::math::Quaternion;
} // namespace op