/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Math/Quaternion.h>
#include <Core/Math/Vector4.h>

namespace Grizzly::Core::Math {
	/**
	 * @class Matrix4
	 * @brief A template class for creating and manipulating column major 4x4 matrices.
	 *
	 * @tparam T A floating-point type specifying the precision of the matrix elements.
	 */
	template <FloatingPoint T>
	struct Matrix4 {
		Vector4<T> x, y, z, w;

		/**
		 * @brief Default constructor. Initializes the matrix to the 0 matrix.
		 */
		GRIZZLY_ALWAYS_INLINE constexpr Matrix4() : x{}, y{}, z{}, w{} {}

		/**
		 * @brief Constructs a matrix from its columns.
		 * @param x The first column of the matrix.
		 * @param y The second column of the matrix.
		 * @param z The third column of the matrix.
		 * @param w The fourth column of the matrix.
		 * @return A new Matrix4 instance constructed from the given columns.
		 */
		static constexpr Matrix4
		from_columns(const Vector4<T>& x, const Vector4<T>& y, const Vector4<T>& z, const Vector4<T>& w);

		/**
		 * @brief Constructs a matrix from its rows.
		 * @param x The first row of the matrix.
		 * @param y The second row of the matrix.
		 * @param z The third row of the matrix.
		 * @param w The fourth row of the matrix.
		 * @returns A new Matrix4 instance where the input rows are converted to columns.
		 */
		static constexpr Matrix4
		from_rows(const Vector4<T>& x, const Vector4<T>& y, const Vector4<T>& z, const Vector4<T>& w);

		/**
		 * @brief Creates an identity matrix.
		 * @return An identity matrix of type Matrix4.
		 */
		static constexpr GRIZZLY_ALWAYS_INLINE Matrix4 identity();

		/**
		 * @brief Creates an orthographic projection matrix.
		 * @param width Width of the viewing volume.
		 * @param height Height of the viewing volume.
		 * @param near Near clipping plane distance.
		 * @param far Far clipping plane distance.
		 * @return An orthographic projection matrix.
		 */
		static constexpr Matrix4 orthographic(T width, T height, T near, T far);

		/**
		 * @brief Creates a perspective projection matrix.
		 * @param fov Field of view angle in degrees.
		 * @param aspect_ratio Aspect ratio of the viewing volume.
		 * @param near Near clipping plane distance.
		 * @param far Far clipping plane distance.
		 * @return A perspective projection matrix.
		 */
		static Matrix4 perspective(T fov, T aspect_ratio, T near, T far);

		/**
		 * @brief Creates a transformation matrix from position, rotation, and scale.
		 * @param position The position vector.
		 * @param rotation The rotation quaternion.
		 * @param scale The scale vector.
		 * @return A transformation matrix.
		 */
		static Matrix4 transform(const Vector3<T>& position, const Quaternion<T>& rotation, const Vector3<T>& scale);

		/**
		 * @brief Attempts to calculate the inverse of the matrix.
		 * @return An optional Matrix4 containing the inverse if it exists, otherwise an empty option.
		 */
		GRIZZLY_NO_DISCARD Option<Matrix4> inverse() const;

		/**
		 * @brief Retrieves a row of the matrix as a Vector4.
		 * @param index Index of the row to retrieve.
		 * @return The requested row as a Vector4.
		 */
		GRIZZLY_NO_DISCARD Vector4<T> row(usize index) const;

		/**
		 * @brief Multiplies this matrix by another matrix.
		 * @param rhs The right-hand side matrix to multiply with.
		 * @return The result of the matrix multiplication.
		 */
		Matrix4 operator*(const Matrix4& rhs) const;

		/**
		 * @brief Multiplies this matrix by a vector.
		 * @param rhs The vector to multiply with.
		 * @return The result of the multiplication as a Vector4.
		 */
		Vector4<T> operator*(const Vector4<T>& rhs) const;

	private:
		GRIZZLY_ALWAYS_INLINE constexpr explicit Matrix4(
			const Vector4<T>& _x,
			const Vector4<T>& _y,
			const Vector4<T>& _z,
			const Vector4<T>& _w)
			: x{ _x }
			, y{ _y }
			, z{ _z }
			, w{ _w } {}
	};

	template <FloatingPoint T>
	constexpr Matrix4<T>
	Matrix4<T>::from_columns(const Vector4<T>& x, const Vector4<T>& y, const Vector4<T>& z, const Vector4<T>& w) {
		return Matrix4{ x, y, z, w };
	}

	template <FloatingPoint T>
	constexpr Matrix4<T>
	Matrix4<T>::from_rows(const Vector4<T>& x, const Vector4<T>& y, const Vector4<T>& z, const Vector4<T>& w) {
		const Vector4<T> x_col = { x.x, y.x, z.x, w.x };
		const Vector4<T> y_col = { x.y, y.y, z.y, w.y };
		const Vector4<T> z_col = { x.z, y.z, z.z, w.z };
		const Vector4<T> w_col = { x.w, y.w, z.w, w.w };
		return Matrix4{ x_col, y_col, z_col, w_col };
	}

	template <FloatingPoint T>
	constexpr GRIZZLY_ALWAYS_INLINE Matrix4<T> Matrix4<T>::identity() {
		return Matrix4{ Vector4<T>{ 1, 0, 0, 0 },
						Vector4<T>{ 0, 1, 0, 0 },
						Vector4<T>{ 0, 0, 1, 0 },
						Vector4<T>{ 0, 0, 0, 1 } };
	}

	template <FloatingPoint T>
	constexpr Matrix4<T> Matrix4<T>::orthographic(T width, T height, T near, T far) {
		// 0 - 1 z clipping
		auto result = Matrix4::identity();
		result.x.x = T{ 2 } / width;
		result.y.y = T{ 2 } / height;
		result.z.z = T{ 1 } / (far - near);

		result.w.z = near / (far - near);

		return result;
	}

	template <FloatingPoint T>
	Matrix4<T> Matrix4<T>::perspective(T fov, T aspect_ratio, T near, T far) {
		const auto cotan = (T)1 / Math::tan((fov * Math::deg_to_rad<T>) / (T)2);

		auto result = Matrix4::identity;
		result.x.x = cotan / aspect_ratio;
		result.y.y = cotan;
		result.z.w = -1;

		result.z.z = far / (near - far);
		result.w.z = -(far * near) / (far - near);

		result.w.w = 0;

		return result;
	}

	template <FloatingPoint T>
	Matrix4<T>
	Matrix4<T>::transform(const Vector3<T>& position, const Quaternion<T>& rotation, const Vector3<T>& scale) {
		Matrix4 result;
		result.x = { rotation.rotate(Vector3<T>::forward) * scale.x, T{ 0 } };
		result.y = { rotation.rotate(Vector3<T>::right) * scale.y, T{ 0 } };
		result.z = { rotation.rotate(Vector3<T>::up) * scale.z, T{ 0 } };
		result.w = { position, T{ 1 } };
		return result;
	}

	template <FloatingPoint T>
	Option<Matrix4<T>> Matrix4<T>::inverse() const {
		const auto a = x.x;
		const auto b = x.y;
		const auto c = x.z;
		const auto d = x.w;
		const auto e = y.x;
		const auto f = y.y;
		const auto g = y.z;
		const auto h = y.w;
		const auto i = z.x;
		const auto j = z.y;
		const auto k = z.z;
		const auto l = z.w;
		const auto m = w.x;
		const auto n = w.y;
		const auto o = w.z;
		const auto p = w.w;

		const auto kp_lo = k * p - l * o;
		const auto jp_ln = j * p - l * n;
		const auto jo_kn = j * o - k * n;
		const auto ip_lm = i * p - l * m;
		const auto io_km = i * o - k * m;
		const auto in_jm = i * n - j * m;

		const auto a11 = +(f * kp_lo - g * jp_ln + h * jo_kn);
		const auto a12 = -(e * kp_lo - g * ip_lm + h * io_km);
		const auto a13 = +(e * jp_ln - f * ip_lm + h * in_jm);
		const auto a14 = -(e * jo_kn - f * io_km + g * in_jm);

		const auto det = a * a11 + b * a12 + c * a13 + d * a14;
		if (Math::equals(det, 0.f)) {
			return nullopt;
		}

		const auto inv_det = 1.f / det;

		const auto a21 = -(b * kp_lo - c * jp_ln + d * jo_kn) * inv_det;
		const auto a22 = +(a * kp_lo - c * ip_lm + d * io_km) * inv_det;
		const auto a23 = -(a * jp_ln - b * ip_lm + d * in_jm) * inv_det;
		const auto a24 = +(a * jo_kn - b * io_km + c * in_jm) * inv_det;

		const auto gp_ho = g * p - h * o;
		const auto fp_hn = f * p - h * n;
		const auto fo_gn = f * o - g * n;
		const auto ep_hm = e * p - h * m;

		const auto eo_gm = e * o - g * m;
		const auto en_fm = e * n - f * m;

		const auto a31 = +(b * gp_ho - c * fp_hn + d * fo_gn) * inv_det;
		const auto a32 = -(a * gp_ho - c * ep_hm + d * eo_gm) * inv_det;
		const auto a33 = +(a * fp_hn - b * ep_hm + d * en_fm) * inv_det;
		const auto a34 = -(a * fo_gn - b * eo_gm + c * en_fm) * inv_det;

		const auto gl_hk = g * l - h * k;
		const auto fl_hj = f * l - h * j;
		const auto fk_gj = f * k - g * j;
		const auto el_hi = e * l - h * i;

		const auto ek_gi = e * k - g * i;
		const auto ej_fi = e * j - f * i;

		const auto a41 = -(b * gl_hk - c * fl_hj + d * fk_gj) * inv_det;
		const auto a42 = +(a * gl_hk - c * el_hi + d * ek_gi) * inv_det;
		const auto a43 = -(a * fl_hj - b * el_hi + d * ej_fi) * inv_det;
		const auto a44 = +(a * fk_gj - b * ek_gi + c * ej_fi) * inv_det;

		return Matrix4::from_columns(
			Vector4<T>{ a11, a21, a31, a41 },
			Vector4<T>{ a12, a22, a32, a42 },
			Vector4<T>{ a13, a23, a33, a43 },
			Vector4<T>{ a14, a24, a34, a44 });
	}

	template <FloatingPoint T>
	Vector4<T> Matrix4<T>::row(usize index) const {
		GRIZZLY_ASSERT(index < 4);
		switch (index) {
		case 0:
			return { x.x, y.x, z.x, w.x };
		case 1:
			return { x.y, y.y, z.y, w.y };
		case 2:
			return { x.z, y.z, z.z, w.z };
		case 3:
			return { x.w, y.w, z.w, w.w };
		}
		return {};
	}

	template <FloatingPoint T>
	Matrix4<T> Matrix4<T>::operator*(const Matrix4<T>& rhs) const {
		Vector4<T> row_x = {};
		const auto row0 = row(0);
		row_x.x = row0.dot(rhs.x);
		row_x.y = row0.dot(rhs.y);
		row_x.z = row0.dot(rhs.z);
		row_x.w = row0.dot(rhs.w);

		Vector4<T> row_y = {};
		const auto row1 = row(1);
		row_y.x = row1.dot(rhs.x);
		row_y.y = row1.dot(rhs.y);
		row_y.z = row1.dot(rhs.z);
		row_y.w = row1.dot(rhs.w);

		Vector4<T> row_z = {};
		const auto row2 = row(2);
		row_z.x = row2.dot(rhs.x);
		row_z.y = row2.dot(rhs.y);
		row_z.z = row2.dot(rhs.z);
		row_z.w = row2.dot(rhs.w);

		Vector4<T> row_w = {};
		const auto row3 = row(3);
		row_w.x = row3.dot(rhs.x);
		row_w.y = row3.dot(rhs.y);
		row_w.z = row3.dot(rhs.z);
		row_w.w = row3.dot(rhs.w);

		return Matrix4::from_rows(row_x, row_y, row_z, row_w);
	}

	template <FloatingPoint T>
	Vector4<T> Matrix4<T>::operator*(const Vector4<T>& rhs) const {
		const auto _x = row(0).dot(rhs);
		const auto _y = row(1).dot(rhs);
		const auto _z = row(2).dot(rhs);
		const auto _w = row(3).dot(rhs);

		return { _x, _y, _z, _w };
	}
} // namespace Grizzly::Core::Math

namespace Grizzly {
	using Core::Math::Matrix4;
} // namespace Grizzly
