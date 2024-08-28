/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include "core/math/matrix4.h"
#include "core/debug/test.h"

#if OP_ENABLE_TEST
OP_TEST_SUITE("math") {
	using namespace op;

	OP_TEST_CASE("Matrix4") {
		OP_SUBCASE("default initialize") {
			const Matrix4<f32> m;
			OP_CHECK(m.x.equals(0.f));
			OP_CHECK(m.y.equals(0.f));
			OP_CHECK(m.z.equals(0.f));
			OP_CHECK(m.w.equals(0.f));
		}

		OP_SUBCASE("from_columns") {
			const Vector4<f32> x{ 1, 2, 3, 4 };
			const Vector4<f32> y{ 5, 6, 7, 8 };
			const Vector4<f32> z{ 9, 10, 11, 12 };
			const Vector4<f32> w{ 13, 14, 15, 16 };

			const Matrix4<f32> m = Matrix4<f32>::from_columns(x, y, z, w);

			OP_CHECK(m.x.equals(x));
			OP_CHECK(m.y.equals(y));
			OP_CHECK(m.z.equals(z));
			OP_CHECK(m.w.equals(w));
		}

		OP_SUBCASE("from_rows") {
			const Vector4<f32> x{ 1, 2, 3, 4 };
			const Vector4<f32> y{ 5, 6, 7, 8 };
			const Vector4<f32> z{ 9, 10, 11, 12 };
			const Vector4<f32> w{ 13, 14, 15, 16 };

			const Matrix4<f32> m = Matrix4<f32>::from_rows(x, y, z, w);

			OP_CHECK(m.x.x == x.x);
			OP_CHECK(m.x.y == y.x);
			OP_CHECK(m.x.z == z.x);
			OP_CHECK(m.x.w == w.x);

			OP_CHECK(m.y.x == x.y);
			OP_CHECK(m.y.y == y.y);
			OP_CHECK(m.y.z == z.y);
			OP_CHECK(m.y.w == w.y);

			OP_CHECK(m.z.x == x.z);
			OP_CHECK(m.z.y == y.z);
			OP_CHECK(m.z.z == z.z);
			OP_CHECK(m.z.w == w.z);

			OP_CHECK(m.w.x == x.w);
			OP_CHECK(m.w.y == y.w);
			OP_CHECK(m.w.z == z.w);
			OP_CHECK(m.w.w == w.w);
		}

		OP_SUBCASE("identity") {
			const Matrix4<f32> m = Matrix4<f32>::identity();

			OP_CHECK(m.x.equals(Vector4<f32>{ 1, 0, 0, 0 }));
			OP_CHECK(m.y.equals(Vector4<f32>{ 0, 1, 0, 0 }));
			OP_CHECK(m.z.equals(Vector4<f32>{ 0, 0, 1, 0 }));
			OP_CHECK(m.w.equals(Vector4<f32>{ 0, 0, 0, 1 }));
		}

		OP_SUBCASE("inverse") {
			const Matrix4<f32> m = Matrix4<f32>::identity();
			const auto inv = m.inverse();

			OP_CHECK(inv.is_set());
			OP_CHECK(inv.unwrap().x.equals(m.x));
			OP_CHECK(inv.unwrap().y.equals(m.y));
			OP_CHECK(inv.unwrap().z.equals(m.z));
			OP_CHECK(inv.unwrap().w.equals(m.w));
		}

		OP_SUBCASE("row") {
			const Matrix4<f32> m = Matrix4<f32>::identity();

			OP_CHECK(m.row(0).equals(Vector4<f32>{ 1, 0, 0, 0 }));
			OP_CHECK(m.row(1).equals(Vector4<f32>{ 0, 1, 0, 0 }));
			OP_CHECK(m.row(2).equals(Vector4<f32>{ 0, 0, 1, 0 }));
			OP_CHECK(m.row(3).equals(Vector4<f32>{ 0, 0, 0, 1 }));
		}

		OP_SUBCASE("operator*") {
			const Matrix4<f32> m = Matrix4<f32>::identity();
			const Matrix4<f32> n = Matrix4<f32>::identity();
			const auto o = m * n;

			OP_CHECK(o.x.equals(m.x));
			OP_CHECK(o.y.equals(m.y));
			OP_CHECK(o.z.equals(m.z));
			OP_CHECK(o.w.equals(m.w));
		}

		OP_SUBCASE("operator* (Vector4)") {
			const Matrix4<f32> m = Matrix4<f32>::identity();
			const Vector4<f32> v{ 1, 2, 3, 4 };
			const auto o = m * v;

			OP_CHECK(o.equals(v));
		}
	}
}
#endif // OP_ENABLE_TEST
