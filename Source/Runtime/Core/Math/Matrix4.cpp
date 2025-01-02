/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Debug/Test.hpp>
#include <Core/Math/Matrix4.hpp>

#if GRIZZLY_ENABLE_TEST
GRIZZLY_TEST_SUITE("Math") {
	using namespace Grizzly;

	GRIZZLY_TEST_CASE("Matrix4") {
		GRIZZLY_SUBCASE("default initialize") {
			const Matrix4<f32> m;
			GRIZZLY_CHECK(m.x.equals(0.f));
			GRIZZLY_CHECK(m.y.equals(0.f));
			GRIZZLY_CHECK(m.z.equals(0.f));
			GRIZZLY_CHECK(m.w.equals(0.f));
		}

		GRIZZLY_SUBCASE("from_columns") {
			const Vector4<f32> x{ 1, 2, 3, 4 };
			const Vector4<f32> y{ 5, 6, 7, 8 };
			const Vector4<f32> z{ 9, 10, 11, 12 };
			const Vector4<f32> w{ 13, 14, 15, 16 };

			const Matrix4<f32> m = Matrix4<f32>::from_columns(x, y, z, w);

			GRIZZLY_CHECK(m.x.equals(x));
			GRIZZLY_CHECK(m.y.equals(y));
			GRIZZLY_CHECK(m.z.equals(z));
			GRIZZLY_CHECK(m.w.equals(w));
		}

		GRIZZLY_SUBCASE("from_rows") {
			const Vector4<f32> x{ 1, 2, 3, 4 };
			const Vector4<f32> y{ 5, 6, 7, 8 };
			const Vector4<f32> z{ 9, 10, 11, 12 };
			const Vector4<f32> w{ 13, 14, 15, 16 };

			const Matrix4<f32> m = Matrix4<f32>::from_rows(x, y, z, w);

			GRIZZLY_CHECK(m.x.x == x.x);
			GRIZZLY_CHECK(m.x.y == y.x);
			GRIZZLY_CHECK(m.x.z == z.x);
			GRIZZLY_CHECK(m.x.w == w.x);

			GRIZZLY_CHECK(m.y.x == x.y);
			GRIZZLY_CHECK(m.y.y == y.y);
			GRIZZLY_CHECK(m.y.z == z.y);
			GRIZZLY_CHECK(m.y.w == w.y);

			GRIZZLY_CHECK(m.z.x == x.z);
			GRIZZLY_CHECK(m.z.y == y.z);
			GRIZZLY_CHECK(m.z.z == z.z);
			GRIZZLY_CHECK(m.z.w == w.z);

			GRIZZLY_CHECK(m.w.x == x.w);
			GRIZZLY_CHECK(m.w.y == y.w);
			GRIZZLY_CHECK(m.w.z == z.w);
			GRIZZLY_CHECK(m.w.w == w.w);
		}

		GRIZZLY_SUBCASE("identity") {
			const Matrix4<f32> m = Matrix4<f32>::identity();

			GRIZZLY_CHECK(m.x.equals(Vector4<f32>{ 1, 0, 0, 0 }));
			GRIZZLY_CHECK(m.y.equals(Vector4<f32>{ 0, 1, 0, 0 }));
			GRIZZLY_CHECK(m.z.equals(Vector4<f32>{ 0, 0, 1, 0 }));
			GRIZZLY_CHECK(m.w.equals(Vector4<f32>{ 0, 0, 0, 1 }));
		}

		GRIZZLY_SUBCASE("inverse") {
			const Matrix4<f32> m = Matrix4<f32>::identity();
			const auto inv = m.inverse();

			GRIZZLY_CHECK(inv.is_set());
			GRIZZLY_CHECK(inv.unwrap().x.equals(m.x));
			GRIZZLY_CHECK(inv.unwrap().y.equals(m.y));
			GRIZZLY_CHECK(inv.unwrap().z.equals(m.z));
			GRIZZLY_CHECK(inv.unwrap().w.equals(m.w));
		}

		GRIZZLY_SUBCASE("row") {
			const Matrix4<f32> m = Matrix4<f32>::identity();

			GRIZZLY_CHECK(m.row(0).equals(Vector4<f32>{ 1, 0, 0, 0 }));
			GRIZZLY_CHECK(m.row(1).equals(Vector4<f32>{ 0, 1, 0, 0 }));
			GRIZZLY_CHECK(m.row(2).equals(Vector4<f32>{ 0, 0, 1, 0 }));
			GRIZZLY_CHECK(m.row(3).equals(Vector4<f32>{ 0, 0, 0, 1 }));
		}

		GRIZZLY_SUBCASE("operator*") {
			const Matrix4<f32> m = Matrix4<f32>::identity();
			const Matrix4<f32> n = Matrix4<f32>::identity();
			const auto o = m * n;

			GRIZZLY_CHECK(o.x.equals(m.x));
			GRIZZLY_CHECK(o.y.equals(m.y));
			GRIZZLY_CHECK(o.z.equals(m.z));
			GRIZZLY_CHECK(o.w.equals(m.w));
		}

		GRIZZLY_SUBCASE("operator* (Vector4)") {
			const Matrix4<f32> m = Matrix4<f32>::identity();
			const Vector4<f32> v{ 1, 2, 3, 4 };
			const auto o = m * v;

			GRIZZLY_CHECK(o.equals(v));
		}
	}
}
#endif // GRIZZLY_ENABLE_TEST
