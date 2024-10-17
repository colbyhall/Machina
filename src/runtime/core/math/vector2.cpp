/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <core/debug/test.h>
#include <core/math/vector2.h>

#if GRIZZLY_ENABLE_TEST
GRIZZLY_TEST_SUITE("math") {
	using namespace grizzly;

	GRIZZLY_TEST_CASE("Vector2") {
		GRIZZLY_SUBCASE("default initialize") {
			Vector2<i32> zero;
			GRIZZLY_CHECK(zero.x == 0);
			GRIZZLY_CHECK(zero.y == 0);
		}

		GRIZZLY_SUBCASE("scalar initialize") {
			Vector2<f32> foo = 1.f;
			GRIZZLY_CHECK(foo.equals(1.f));
		}

		GRIZZLY_SUBCASE("x y initializer") {
			Vector2<u32> foo{ 0, 1 };
			GRIZZLY_CHECK(foo.x == 0);
			GRIZZLY_CHECK(foo.y == 1);
		}

		GRIZZLY_SUBCASE("from_rad") {
			const auto foo = Vector2<f32>::from_rad(0.f);
			GRIZZLY_CHECK(math::equals(foo.x, 1.f));
			GRIZZLY_CHECK(math::equals(foo.y, 0.f));

			const auto bar = Vector2<f32>::from_rad(math::pi<f32> / 2.f);
			GRIZZLY_CHECK(math::equals(bar.x, 0.f));
			GRIZZLY_CHECK(math::equals(bar.y, 1.f));
		}

		GRIZZLY_SUBCASE("operator+") {
			const Vector2<i32> a = 0;
			const Vector2<i32> b = 1;
			GRIZZLY_CHECK((a + b).equals(b));
		}

		GRIZZLY_SUBCASE("operator-") {
			const Vector2<i32> a = 0;
			const Vector2<i32> b = 1;
			GRIZZLY_CHECK((b - a).equals(b));
		}

		GRIZZLY_SUBCASE("operator*") {
			const Vector2<i32> a = 1;
			const Vector2<i32> b = 5;
			GRIZZLY_CHECK((a * b).equals(b));
		}

		GRIZZLY_SUBCASE("operator/") {
			const Vector2<i32> a = 1;
			const Vector2<i32> b = 5;
			GRIZZLY_CHECK((b / a).equals(b));
		}

		GRIZZLY_SUBCASE("operator+=") {
			Vector2<i32> a = 0;
			const Vector2<i32> b = 1;
			a += b;
			GRIZZLY_CHECK(a.equals(b));
		}

		GRIZZLY_SUBCASE("operator-=") {
			const Vector2<i32> a = 0;
			Vector2<i32> b = 1;
			b -= a;
			GRIZZLY_CHECK(b.equals(1));
		}

		GRIZZLY_SUBCASE("operator*=") {
			Vector2<i32> a = 1;
			const Vector2<i32> b = 5;
			a *= b;
			GRIZZLY_CHECK(a.equals(b));
		}

		GRIZZLY_SUBCASE("operator/=") {
			const Vector2<i32> a = 1;
			Vector2<i32> b = 5;
			b /= a;
			GRIZZLY_CHECK(b.equals(5));
		}

		GRIZZLY_SUBCASE("operator- (negate)") {
			const Vector2<i32> a = 1;
			GRIZZLY_CHECK((-a).equals(-1));
		}

		GRIZZLY_SUBCASE("dot") {
			const Vector2<f32> foo{ 1.f, 0.f };
			const Vector2<f32> bar{ 0.f, 1.f };
			GRIZZLY_CHECK(math::equals(foo.dot(foo), 1.f));
			GRIZZLY_CHECK(math::equals(foo.dot(bar), 0.f));
		}

		GRIZZLY_SUBCASE("cross") {
			const Vector2<f32> foo{ 1.f, 0.f };
			const Vector2<f32> bar{ 0.f, 1.f };
			GRIZZLY_CHECK(math::equals(foo.cross(foo), 0.f));
			GRIZZLY_CHECK(math::equals(foo.cross(bar), 1.f));
		}

		GRIZZLY_SUBCASE("perp") {
			const Vector2<f32> foo{ 1.f, 0.f };
			GRIZZLY_CHECK(foo.perp().equals(Vector2<f32>{ 0.f, -1.f }));
		}

		GRIZZLY_SUBCASE("len_sq") {
			const Vector2<f32> foo{ 1.f, 0.f };
			GRIZZLY_CHECK(math::equals(foo.len_sq(), 1.f));
		}

		GRIZZLY_SUBCASE("len") {
			const Vector2<f32> foo = 1.f;
			GRIZZLY_CHECK(math::equals(foo.len(), math::sqrt(2.f)));
		}

		GRIZZLY_SUBCASE("normalized") {
			const Vector2<f32> a = 1.f;
			const auto a_normalized = a.normalized();
			GRIZZLY_CHECK(a_normalized.is_set());
			GRIZZLY_CHECK(a.normalized().unwrap().equals(math::sqrt(2.f) * 0.5f));

			const Vector2<f32> b = 0.f;
			GRIZZLY_CHECK(!b.normalized().is_set());
		}

		GRIZZLY_SUBCASE("contains") {
			const Vector2<i32> foo{ 0, 5 };
			GRIZZLY_CHECK(foo.contains(5));

			// Compiler will throw error if there is a divide by 0 in floating point literal
			f32 a = 1;
			a -= 1;
			const Vector2<f32> bar{ 1.f / a, 0.f };
			GRIZZLY_CHECK(bar.contains(math::nan<f32>));
		}

		GRIZZLY_SUBCASE("equals") {
			const Vector2<f32> a = 1.f;
			GRIZZLY_CHECK(a.equals(1.f));

			const Vector2<f32> b = 10.f;
			GRIZZLY_CHECK(b.equals(7.f, 5.f));
		}

		GRIZZLY_SUBCASE("min") {
			const Vector2<f32> a = 0.f;
			const Vector2<f32> b = 10.f;
			GRIZZLY_CHECK(a.min(b).equals(a));

			const Vector2<f32> c{ 0.f, 10.f };
			const Vector2<f32> d{ 10.f, 0.f };
			GRIZZLY_CHECK(c.min(d).equals(a));
		}

		GRIZZLY_SUBCASE("max") {
			const Vector2<f32> a = 0.f;
			const Vector2<f32> b = 10.f;
			GRIZZLY_CHECK(a.max(b).equals(b));

			const Vector2<f32> c{ 0.f, 10.f };
			const Vector2<f32> d{ 10.f, 0.f };
			GRIZZLY_CHECK(c.max(d).equals(b));
		}

		GRIZZLY_SUBCASE("as") {
			const Vector2<f32> a = 0.5f;
			const auto b = a.as<i32>();
			GRIZZLY_CHECK(b.equals(0));
		}
	}
}
#endif // GRIZZLY_ENABLE_TEST
