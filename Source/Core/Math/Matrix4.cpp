/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Debug/Test.hpp>
#include <Core/Math/Matrix4.hpp>

#if MACH_ENABLE_TEST
MACH_TEST_SUITE("Math") {
	using namespace Mach;

	MACH_TEST_CASE("Matrix4") {
		MACH_SUBCASE("default initialize") {
			const Matrix4<f32> m;
			MACH_CHECK(m.x.equals(0.f));
			MACH_CHECK(m.y.equals(0.f));
			MACH_CHECK(m.z.equals(0.f));
			MACH_CHECK(m.w.equals(0.f));
		}

		MACH_SUBCASE("from_columns") {
			const Vector4<f32> x{ 1, 2, 3, 4 };
			const Vector4<f32> y{ 5, 6, 7, 8 };
			const Vector4<f32> z{ 9, 10, 11, 12 };
			const Vector4<f32> w{ 13, 14, 15, 16 };

			const Matrix4<f32> m = Matrix4<f32>::from_columns(x, y, z, w);

			MACH_CHECK(m.x.equals(x));
			MACH_CHECK(m.y.equals(y));
			MACH_CHECK(m.z.equals(z));
			MACH_CHECK(m.w.equals(w));
		}

		MACH_SUBCASE("from_rows") {
			const Vector4<f32> x{ 1, 2, 3, 4 };
			const Vector4<f32> y{ 5, 6, 7, 8 };
			const Vector4<f32> z{ 9, 10, 11, 12 };
			const Vector4<f32> w{ 13, 14, 15, 16 };

			const Matrix4<f32> m = Matrix4<f32>::from_rows(x, y, z, w);

			MACH_CHECK(m.x.x == x.x);
			MACH_CHECK(m.x.y == y.x);
			MACH_CHECK(m.x.z == z.x);
			MACH_CHECK(m.x.w == w.x);

			MACH_CHECK(m.y.x == x.y);
			MACH_CHECK(m.y.y == y.y);
			MACH_CHECK(m.y.z == z.y);
			MACH_CHECK(m.y.w == w.y);

			MACH_CHECK(m.z.x == x.z);
			MACH_CHECK(m.z.y == y.z);
			MACH_CHECK(m.z.z == z.z);
			MACH_CHECK(m.z.w == w.z);

			MACH_CHECK(m.w.x == x.w);
			MACH_CHECK(m.w.y == y.w);
			MACH_CHECK(m.w.z == z.w);
			MACH_CHECK(m.w.w == w.w);
		}

		MACH_SUBCASE("identity") {
			const Matrix4<f32> m = Matrix4<f32>::identity();

			MACH_CHECK(m.x.equals(Vector4<f32>{ 1, 0, 0, 0 }));
			MACH_CHECK(m.y.equals(Vector4<f32>{ 0, 1, 0, 0 }));
			MACH_CHECK(m.z.equals(Vector4<f32>{ 0, 0, 1, 0 }));
			MACH_CHECK(m.w.equals(Vector4<f32>{ 0, 0, 0, 1 }));
		}

		MACH_SUBCASE("inverse") {
			const Matrix4<f32> m = Matrix4<f32>::identity();
			const auto inv = m.inverse();

			MACH_CHECK(inv.is_set());
			MACH_CHECK(inv.unwrap().x.equals(m.x));
			MACH_CHECK(inv.unwrap().y.equals(m.y));
			MACH_CHECK(inv.unwrap().z.equals(m.z));
			MACH_CHECK(inv.unwrap().w.equals(m.w));
		}

		MACH_SUBCASE("row") {
			const Matrix4<f32> m = Matrix4<f32>::identity();

			MACH_CHECK(m.row(0).equals(Vector4<f32>{ 1, 0, 0, 0 }));
			MACH_CHECK(m.row(1).equals(Vector4<f32>{ 0, 1, 0, 0 }));
			MACH_CHECK(m.row(2).equals(Vector4<f32>{ 0, 0, 1, 0 }));
			MACH_CHECK(m.row(3).equals(Vector4<f32>{ 0, 0, 0, 1 }));
		}

		MACH_SUBCASE("operator*") {
			const Matrix4<f32> m = Matrix4<f32>::identity();
			const Matrix4<f32> n = Matrix4<f32>::identity();
			const auto o = m * n;

			MACH_CHECK(o.x.equals(m.x));
			MACH_CHECK(o.y.equals(m.y));
			MACH_CHECK(o.z.equals(m.z));
			MACH_CHECK(o.w.equals(m.w));
		}

		MACH_SUBCASE("operator* (Vector4)") {
			const Matrix4<f32> m = Matrix4<f32>::identity();
			const Vector4<f32> v{ 1, 2, 3, 4 };
			const auto o = m * v;

			MACH_CHECK(o.equals(v));
		}
	}
}
#endif // MACH_ENABLE_TEST
