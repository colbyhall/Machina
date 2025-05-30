/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Debug/Test.hpp>
#include <Core/Math/Vector2.hpp>

#if MACH_ENABLE_TEST
MACH_TEST_SUITE("Math") {
	using namespace Mach;

	MACH_TEST_CASE("Vector2") {
		MACH_SUBCASE("default initialize") {
			Vector2<i32> zero;
			MACH_CHECK(zero.x == 0);
			MACH_CHECK(zero.y == 0);
		}

		MACH_SUBCASE("scalar initialize") {
			Vector2<f32> foo = 1.f;
			MACH_CHECK(foo.equals(1.f));
		}

		MACH_SUBCASE("x y initializer") {
			Vector2<u32> foo{ 0, 1 };
			MACH_CHECK(foo.x == 0);
			MACH_CHECK(foo.y == 1);
		}

		MACH_SUBCASE("from_rad") {
			const auto foo = Vector2<f32>::from_rad(0.f);
			MACH_CHECK(Math::equals(foo.x, 1.f));
			MACH_CHECK(Math::equals(foo.y, 0.f));

			const auto bar = Vector2<f32>::from_rad(Math::pi<f32> / 2.f);
			MACH_CHECK(Math::equals(bar.x, 0.f));
			MACH_CHECK(Math::equals(bar.y, 1.f));
		}

		MACH_SUBCASE("operator+") {
			const Vector2<i32> a = 0;
			const Vector2<i32> b = 1;
			MACH_CHECK((a + b).equals(b));
		}

		MACH_SUBCASE("operator-") {
			const Vector2<i32> a = 0;
			const Vector2<i32> b = 1;
			MACH_CHECK((b - a).equals(b));
		}

		MACH_SUBCASE("operator*") {
			const Vector2<i32> a = 1;
			const Vector2<i32> b = 5;
			MACH_CHECK((a * b).equals(b));
		}

		MACH_SUBCASE("operator/") {
			const Vector2<i32> a = 1;
			const Vector2<i32> b = 5;
			MACH_CHECK((b / a).equals(b));
		}

		MACH_SUBCASE("operator+=") {
			Vector2<i32> a = 0;
			const Vector2<i32> b = 1;
			a += b;
			MACH_CHECK(a.equals(b));
		}

		MACH_SUBCASE("operator-=") {
			const Vector2<i32> a = 0;
			Vector2<i32> b = 1;
			b -= a;
			MACH_CHECK(b.equals(1));
		}

		MACH_SUBCASE("operator*=") {
			Vector2<i32> a = 1;
			const Vector2<i32> b = 5;
			a *= b;
			MACH_CHECK(a.equals(b));
		}

		MACH_SUBCASE("operator/=") {
			const Vector2<i32> a = 1;
			Vector2<i32> b = 5;
			b /= a;
			MACH_CHECK(b.equals(5));
		}

		MACH_SUBCASE("operator- (negate)") {
			const Vector2<i32> a = 1;
			MACH_CHECK((-a).equals(-1));
		}

		MACH_SUBCASE("dot") {
			const Vector2<f32> foo{ 1.f, 0.f };
			const Vector2<f32> bar{ 0.f, 1.f };
			MACH_CHECK(Math::equals(foo.dot(foo), 1.f));
			MACH_CHECK(Math::equals(foo.dot(bar), 0.f));
		}

		MACH_SUBCASE("cross") {
			const Vector2<f32> foo{ 1.f, 0.f };
			const Vector2<f32> bar{ 0.f, 1.f };
			MACH_CHECK(Math::equals(foo.cross(foo), 0.f));
			MACH_CHECK(Math::equals(foo.cross(bar), 1.f));
		}

		MACH_SUBCASE("perp") {
			const Vector2<f32> foo{ 1.f, 0.f };
			MACH_CHECK(foo.perp().equals(Vector2<f32>{ 0.f, -1.f }));
		}

		MACH_SUBCASE("len_sq") {
			const Vector2<f32> foo{ 1.f, 0.f };
			MACH_CHECK(Math::equals(foo.len_sq(), 1.f));
		}

		MACH_SUBCASE("len") {
			const Vector2<f32> foo = 1.f;
			MACH_CHECK(Math::equals(foo.len(), Math::sqrt(2.f)));
		}

		MACH_SUBCASE("normalized") {
			const Vector2<f32> a = 1.f;
			const auto a_normalized = a.normalized();
			MACH_CHECK(a_normalized.is_set());
			MACH_CHECK(a.normalized().unwrap().equals(Math::sqrt(2.f) * 0.5f));

			const Vector2<f32> b = 0.f;
			MACH_CHECK(!b.normalized().is_set());
		}

		MACH_SUBCASE("contains") {
			const Vector2<i32> foo{ 0, 5 };
			MACH_CHECK(foo.contains(5));

			// Compiler will throw error if there is a divide by 0 in floating point literal
			f32 a = 1;
			a -= 1;
			const Vector2<f32> bar{ 1.f / a, 0.f };
			MACH_CHECK(bar.contains(Math::nan<f32>));
		}

		MACH_SUBCASE("equals") {
			const Vector2<f32> a = 1.f;
			MACH_CHECK(a.equals(1.f));

			const Vector2<f32> b = 10.f;
			MACH_CHECK(b.equals(7.f, 5.f));
		}

		MACH_SUBCASE("min") {
			const Vector2<f32> a = 0.f;
			const Vector2<f32> b = 10.f;
			MACH_CHECK(a.min(b).equals(a));

			const Vector2<f32> c{ 0.f, 10.f };
			const Vector2<f32> d{ 10.f, 0.f };
			MACH_CHECK(c.min(d).equals(a));
		}

		MACH_SUBCASE("max") {
			const Vector2<f32> a = 0.f;
			const Vector2<f32> b = 10.f;
			MACH_CHECK(a.max(b).equals(b));

			const Vector2<f32> c{ 0.f, 10.f };
			const Vector2<f32> d{ 10.f, 0.f };
			MACH_CHECK(c.max(d).equals(b));
		}

		MACH_SUBCASE("as") {
			const Vector2<f32> a = 0.5f;
			const auto b = a.as<i32>();
			MACH_CHECK(b.equals(0));
		}
	}
}
#endif // MACH_ENABLE_TEST
