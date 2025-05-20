/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Debug/Test.hpp>
#include <Core/Math/Matrix4.hpp>

#if FORGE_ENABLE_TEST
FORGE_TEST_SUITE("Math") {
	using namespace Forge;

	FORGE_TEST_CASE("Matrix4") {
		FORGE_SUBCASE("default initialize") {
			const Matrix4<f32> m;
			FORGE_CHECK(m.x.equals(0.f));
			FORGE_CHECK(m.y.equals(0.f));
			FORGE_CHECK(m.z.equals(0.f));
			FORGE_CHECK(m.w.equals(0.f));
		}

		FORGE_SUBCASE("from_columns") {
			const Vector4<f32> x{ 1, 2, 3, 4 };
			const Vector4<f32> y{ 5, 6, 7, 8 };
			const Vector4<f32> z{ 9, 10, 11, 12 };
			const Vector4<f32> w{ 13, 14, 15, 16 };

			const Matrix4<f32> m = Matrix4<f32>::from_columns(x, y, z, w);

			FORGE_CHECK(m.x.equals(x));
			FORGE_CHECK(m.y.equals(y));
			FORGE_CHECK(m.z.equals(z));
			FORGE_CHECK(m.w.equals(w));
		}

		FORGE_SUBCASE("from_rows") {
			const Vector4<f32> x{ 1, 2, 3, 4 };
			const Vector4<f32> y{ 5, 6, 7, 8 };
			const Vector4<f32> z{ 9, 10, 11, 12 };
			const Vector4<f32> w{ 13, 14, 15, 16 };

			const Matrix4<f32> m = Matrix4<f32>::from_rows(x, y, z, w);

			FORGE_CHECK(m.x.x == x.x);
			FORGE_CHECK(m.x.y == y.x);
			FORGE_CHECK(m.x.z == z.x);
			FORGE_CHECK(m.x.w == w.x);

			FORGE_CHECK(m.y.x == x.y);
			FORGE_CHECK(m.y.y == y.y);
			FORGE_CHECK(m.y.z == z.y);
			FORGE_CHECK(m.y.w == w.y);

			FORGE_CHECK(m.z.x == x.z);
			FORGE_CHECK(m.z.y == y.z);
			FORGE_CHECK(m.z.z == z.z);
			FORGE_CHECK(m.z.w == w.z);

			FORGE_CHECK(m.w.x == x.w);
			FORGE_CHECK(m.w.y == y.w);
			FORGE_CHECK(m.w.z == z.w);
			FORGE_CHECK(m.w.w == w.w);
		}

		FORGE_SUBCASE("identity") {
			const Matrix4<f32> m = Matrix4<f32>::identity();

			FORGE_CHECK(m.x.equals(Vector4<f32>{ 1, 0, 0, 0 }));
			FORGE_CHECK(m.y.equals(Vector4<f32>{ 0, 1, 0, 0 }));
			FORGE_CHECK(m.z.equals(Vector4<f32>{ 0, 0, 1, 0 }));
			FORGE_CHECK(m.w.equals(Vector4<f32>{ 0, 0, 0, 1 }));
		}

		FORGE_SUBCASE("inverse") {
			const Matrix4<f32> m = Matrix4<f32>::identity();
			const auto inv = m.inverse();

			FORGE_CHECK(inv.is_set());
			FORGE_CHECK(inv.unwrap().x.equals(m.x));
			FORGE_CHECK(inv.unwrap().y.equals(m.y));
			FORGE_CHECK(inv.unwrap().z.equals(m.z));
			FORGE_CHECK(inv.unwrap().w.equals(m.w));
		}

		FORGE_SUBCASE("row") {
			const Matrix4<f32> m = Matrix4<f32>::identity();

			FORGE_CHECK(m.row(0).equals(Vector4<f32>{ 1, 0, 0, 0 }));
			FORGE_CHECK(m.row(1).equals(Vector4<f32>{ 0, 1, 0, 0 }));
			FORGE_CHECK(m.row(2).equals(Vector4<f32>{ 0, 0, 1, 0 }));
			FORGE_CHECK(m.row(3).equals(Vector4<f32>{ 0, 0, 0, 1 }));
		}

		FORGE_SUBCASE("operator*") {
			const Matrix4<f32> m = Matrix4<f32>::identity();
			const Matrix4<f32> n = Matrix4<f32>::identity();
			const auto o = m * n;

			FORGE_CHECK(o.x.equals(m.x));
			FORGE_CHECK(o.y.equals(m.y));
			FORGE_CHECK(o.z.equals(m.z));
			FORGE_CHECK(o.w.equals(m.w));
		}

		FORGE_SUBCASE("operator* (Vector4)") {
			const Matrix4<f32> m = Matrix4<f32>::identity();
			const Vector4<f32> v{ 1, 2, 3, 4 };
			const auto o = m * v;

			FORGE_CHECK(o.equals(v));
		}
	}
}
#endif // FORGE_ENABLE_TEST
