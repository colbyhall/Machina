/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Debug/Test.hpp>
#include <Core/Math/Vector3.hpp>

#if FORGE_ENABLE_TEST
FORGE_TEST_SUITE("Math") {
	using namespace Forge;

	FORGE_TEST_CASE("Vector3") {
		FORGE_SUBCASE("default initialize") {
			Vector3<i32> zero;
			FORGE_CHECK(zero.x == 0);
			FORGE_CHECK(zero.y == 0);
			FORGE_CHECK(zero.z == 0);
		}

		FORGE_SUBCASE("scalar initialize") {
			Vector3<f32> foo = 1.f;
			FORGE_CHECK(foo.equals(1.f));
		}

		FORGE_SUBCASE("x y z initializer") {
			Vector3<u32> foo{ 0, 1, 0 };
			FORGE_CHECK(foo.x == 0);
			FORGE_CHECK(foo.y == 1);
			FORGE_CHECK(foo.z == 0);
		}

		FORGE_SUBCASE("operator+") {
			const Vector3<i32> a = 0;
			const Vector3<i32> b = 1;
			FORGE_CHECK((a + b).equals(b));
		}

		FORGE_SUBCASE("operator-") {
			const Vector3<i32> a = 0;
			const Vector3<i32> b = 1;
			FORGE_CHECK((b - a).equals(b));
		}

		FORGE_SUBCASE("operator*") {
			const Vector3<i32> a = 1;
			const Vector3<i32> b = 5;
			FORGE_CHECK((a * b).equals(b));
		}

		FORGE_SUBCASE("operator/") {
			const Vector3<i32> a = 1;
			const Vector3<i32> b = 5;
			FORGE_CHECK((b / a).equals(b));
		}

		FORGE_SUBCASE("operator+=") {
			Vector3<i32> a = 0;
			const Vector3<i32> b = 1;
			a += b;
			FORGE_CHECK(a.equals(b));
		}

		FORGE_SUBCASE("operator-=") {
			const Vector3<i32> a = 0;
			Vector3<i32> b = 1;
			b -= a;
			FORGE_CHECK(b.equals(1));
		}

		FORGE_SUBCASE("operator*=") {
			Vector3<i32> a = 1;
			const Vector3<i32> b = 5;
			a *= b;
			FORGE_CHECK(a.equals(b));
		}

		FORGE_SUBCASE("operator/=") {
			const Vector3<i32> a = 1;
			Vector3<i32> b = 5;
			b /= a;
			FORGE_CHECK(b.equals(5));
		}

		FORGE_SUBCASE("operator- (negate)") {
			const Vector3<i32> a = 1;
			FORGE_CHECK((-a).equals(-1));
		}

		FORGE_SUBCASE("dot") {
			const Vector3<f32> foo{ 1.f, 0.f, 0.f };
			const Vector3<f32> bar{ 0.f, 1.f, 0.f };
			FORGE_CHECK(Math::equals(foo.dot(foo), 1.f));
			FORGE_CHECK(Math::equals(foo.dot(bar), 0.f));
		}

		FORGE_SUBCASE("cross") {
			const Vector3<f32> foo{ 1.f, 0.f, 0.f };
			const Vector3<f32> bar{ 0.f, 1.f, 0.f };
			FORGE_CHECK(foo.cross(bar).equals({ 0.f, 0.f, 1.f }));
		}

		FORGE_SUBCASE("len_sq") {
			const Vector3<f32> foo{ 1.f, 0.f, 0.f };
			FORGE_CHECK(Math::equals(foo.len_sq(), 1.f));
		}

		FORGE_SUBCASE("len") {
			const Vector3<f32> foo = 1.f;
			FORGE_CHECK(Math::equals(foo.len(), Math::sqrt(2.f)));
		}

		FORGE_SUBCASE("normalized") {
			const Vector3<f32> a = 1.f;
			const auto a_normalized = a.normalized();
			FORGE_CHECK(a_normalized.is_set());
			FORGE_CHECK(a.normalized().unwrap().equals(Math::sqrt(2.f) * 0.5f));

			const Vector3<f32> b = 0.f;
			FORGE_CHECK(!b.normalized().is_set());
		}

		FORGE_SUBCASE("contains") {
			const Vector3<i32> foo{ 0, 5, 0 };
			FORGE_CHECK(foo.contains(5));

			// Compiler will throw error if there is a divide by 0 in floating point literal
			f32 a = 1;
			a -= 1;
			const Vector3<f32> bar{ 1.f / a, 0.f, 0.f };
			FORGE_CHECK(bar.contains(Math::nan<f32>));
		}

		FORGE_SUBCASE("equals") {
			const Vector3<f32> a = 1.f;
			FORGE_CHECK(a.equals(1.f));

			const Vector3<f32> b = 10.f;
			FORGE_CHECK(b.equals(7.f, 5.f));
		}

		FORGE_SUBCASE("min") {
			const Vector3<f32> a = 0.f;
			const Vector3<f32> b = 10.f;
			FORGE_CHECK(a.min(b).equals(a));

			const Vector3<f32> c{ 0.f, 10.f };
			const Vector3<f32> d{ 10.f, 0.f };
			FORGE_CHECK(c.min(d).equals(a));
		}

		FORGE_SUBCASE("max") {
			const Vector3<f32> a = 0.f;
			const Vector3<f32> b = 10.f;
			FORGE_CHECK(a.max(b).equals(b));

			const Vector3<f32> c{ 0.f, 10.f };
			const Vector3<f32> d{ 10.f, 0.f };
			FORGE_CHECK(c.max(d).equals(b));
		}

		FORGE_SUBCASE("as") {
			const Vector3<f32> a = 0.5f;
			const auto b = a.as<i32>();
			FORGE_CHECK(b.equals(0));
		}
	}
}
#endif // FORGE_ENABLE_TEST
