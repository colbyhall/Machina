/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Debug/Test.hpp>
#include <Core/Math/Vector4.hpp>

#if GRIZZLY_ENABLE_TEST
GRIZZLY_TEST_SUITE("Math") {
	using namespace Grizzly;

	GRIZZLY_TEST_CASE("Vector4") {
		GRIZZLY_SUBCASE("default initialize") {
			Vector4<i32> zero;
			GRIZZLY_CHECK(zero.x == 0);
			GRIZZLY_CHECK(zero.y == 0);
			GRIZZLY_CHECK(zero.z == 0);
			GRIZZLY_CHECK(zero.w == 0);
		}

		GRIZZLY_SUBCASE("scalar initialize") {
			Vector4<f32> foo = 1.f;
			GRIZZLY_CHECK(foo.equals(1.f));
		}

		GRIZZLY_SUBCASE("x y z initializer") {
			Vector4<u32> foo{ 0, 1, 0, 0 };
			GRIZZLY_CHECK(foo.x == 0);
			GRIZZLY_CHECK(foo.y == 1);
			GRIZZLY_CHECK(foo.z == 0);
			GRIZZLY_CHECK(foo.w == 0);
		}

		GRIZZLY_SUBCASE("operator+") {
			const Vector4<i32> a = 0;
			const Vector4<i32> b = 1;
			GRIZZLY_CHECK((a + b).equals(b));
		}

		GRIZZLY_SUBCASE("operator-") {
			const Vector4<i32> a = 0;
			const Vector4<i32> b = 1;
			GRIZZLY_CHECK((b - a).equals(b));
		}

		GRIZZLY_SUBCASE("operator*") {
			const Vector4<i32> a = 1;
			const Vector4<i32> b = 5;
			GRIZZLY_CHECK((a * b).equals(b));
		}

		GRIZZLY_SUBCASE("operator/") {
			const Vector4<i32> a = 1;
			const Vector4<i32> b = 5;
			GRIZZLY_CHECK((b / a).equals(b));
		}

		GRIZZLY_SUBCASE("operator+=") {
			Vector4<i32> a = 0;
			const Vector4<i32> b = 1;
			a += b;
			GRIZZLY_CHECK(a.equals(b));
		}

		GRIZZLY_SUBCASE("operator-=") {
			const Vector4<i32> a = 0;
			Vector4<i32> b = 1;
			b -= a;
			GRIZZLY_CHECK(b.equals(1));
		}

		GRIZZLY_SUBCASE("operator*=") {
			Vector4<i32> a = 1;
			const Vector4<i32> b = 5;
			a *= b;
			GRIZZLY_CHECK(a.equals(b));
		}

		GRIZZLY_SUBCASE("operator/=") {
			const Vector4<i32> a = 1;
			Vector4<i32> b = 5;
			b /= a;
			GRIZZLY_CHECK(b.equals(5));
		}

		GRIZZLY_SUBCASE("operator- (negate)") {
			const Vector4<i32> a = 1;
			GRIZZLY_CHECK((-a).equals(-1));
		}

		GRIZZLY_SUBCASE("dot") {
			const Vector4<f32> foo{ 1.f, 0.f, 0.f, 0.f };
			const Vector4<f32> bar{ 0.f, 1.f, 0.f, 0.f };
			GRIZZLY_CHECK(Math::equals(foo.dot(foo), 1.f));
			GRIZZLY_CHECK(Math::equals(foo.dot(bar), 0.f));
		}

		GRIZZLY_SUBCASE("len_sq") {
			const Vector4<f32> foo{ 1.f, 0.f, 0.f, 0.f };
			GRIZZLY_CHECK(Math::equals(foo.len_sq(), 1.f));
		}

		GRIZZLY_SUBCASE("len") {
			const Vector4<f32> foo = 1.f;
			GRIZZLY_CHECK(Math::equals(foo.len(), 2.f));
		}

		GRIZZLY_SUBCASE("normalized") {
			const Vector4<f32> a = 1.f;
			const auto a_normalized = a.normalized();
			GRIZZLY_CHECK(a_normalized.is_set());

			const Vector4<f32> b = 0.f;
			GRIZZLY_CHECK(!b.normalized().is_set());
		}

		GRIZZLY_SUBCASE("contains") {
			const Vector4<i32> foo{ 0, 5, 0, 0 };
			GRIZZLY_CHECK(foo.contains(5));

			// Compiler will throw error if there is a divide by 0 in floating point literal
			f32 a = 1;
			a -= 1;
			const Vector4<f32> bar{ 1.f / a, 0.f, 0.f, 0.f };
			GRIZZLY_CHECK(bar.contains(Math::nan<f32>));
		}

		GRIZZLY_SUBCASE("equals") {
			const Vector4<f32> a = 1.f;
			GRIZZLY_CHECK(a.equals(1.f));

			const Vector4<f32> b = 10.f;
			GRIZZLY_CHECK(b.equals(7.f, 5.f));
		}

		GRIZZLY_SUBCASE("min") {
			const Vector4<f32> a = 0.f;
			const Vector4<f32> b = 10.f;
			GRIZZLY_CHECK(a.min(b).equals(a));

			const Vector4<f32> c{ 0.f, 10.f };
			const Vector4<f32> d{ 10.f, 0.f };
			GRIZZLY_CHECK(c.min(d).equals(a));
		}

		GRIZZLY_SUBCASE("max") {
			const Vector4<f32> a = 0.f;
			const Vector4<f32> b = 10.f;
			GRIZZLY_CHECK(a.max(b).equals(b));

			const Vector4<f32> c{ 0.f, 10.f };
			const Vector4<f32> d{ 10.f, 0.f };
			GRIZZLY_CHECK(c.max(d).equals(b));
		}

		GRIZZLY_SUBCASE("as") {
			const Vector4<f32> a = 0.5f;
			const auto b = a.as<i32>();
			GRIZZLY_CHECK(b.equals(0));
		}
	}
}
#endif // GRIZZLY_ENABLE_TEST
