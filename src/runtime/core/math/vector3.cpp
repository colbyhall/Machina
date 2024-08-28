/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include "core/math/vector3.h"
#include "core/debug/test.h"

#if OP_ENABLE_TEST
OP_TEST_SUITE("math") {
	using namespace op;

	OP_TEST_CASE("Vector3") {
		OP_SUBCASE("default initialize") {
			Vector3<i32> zero;
			OP_CHECK(zero.x == 0);
			OP_CHECK(zero.y == 0);
			OP_CHECK(zero.z == 0);
		}

		OP_SUBCASE("scalar initialize") {
			Vector3<f32> foo = 1.f;
			OP_CHECK(foo.equals(1.f));
		}

		OP_SUBCASE("x y z initializer") {
			Vector3<u32> foo{ 0, 1, 0 };
			OP_CHECK(foo.x == 0);
			OP_CHECK(foo.y == 1);
			OP_CHECK(foo.z == 0);
		}

		OP_SUBCASE("operator+") {
			const Vector3<i32> a = 0;
			const Vector3<i32> b = 1;
			OP_CHECK((a + b).equals(b));
		}

		OP_SUBCASE("operator-") {
			const Vector3<i32> a = 0;
			const Vector3<i32> b = 1;
			OP_CHECK((b - a).equals(b));
		}

		OP_SUBCASE("operator*") {
			const Vector3<i32> a = 1;
			const Vector3<i32> b = 5;
			OP_CHECK((a * b).equals(b));
		}

		OP_SUBCASE("operator/") {
			const Vector3<i32> a = 1;
			const Vector3<i32> b = 5;
			OP_CHECK((b / a).equals(b));
		}

		OP_SUBCASE("operator+=") {
			Vector3<i32> a = 0;
			const Vector3<i32> b = 1;
			a += b;
			OP_CHECK(a.equals(b));
		}

		OP_SUBCASE("operator-=") {
			const Vector3<i32> a = 0;
			Vector3<i32> b = 1;
			b -= a;
			OP_CHECK(b.equals(1));
		}

		OP_SUBCASE("operator*=") {
			Vector3<i32> a = 1;
			const Vector3<i32> b = 5;
			a *= b;
			OP_CHECK(a.equals(b));
		}

		OP_SUBCASE("operator/=") {
			const Vector3<i32> a = 1;
			Vector3<i32> b = 5;
			b /= a;
			OP_CHECK(b.equals(5));
		}

		OP_SUBCASE("operator- (negate)") {
			const Vector3<i32> a = 1;
			OP_CHECK((-a).equals(-1));
		}

		OP_SUBCASE("dot") {
			const Vector3<f32> foo{ 1.f, 0.f, 0.f };
			const Vector3<f32> bar{ 0.f, 1.f, 0.f };
			OP_CHECK(math::equals(foo.dot(foo), 1.f));
			OP_CHECK(math::equals(foo.dot(bar), 0.f));
		}

		OP_SUBCASE("cross") {
			const Vector3<f32> foo{ 1.f, 0.f, 0.f };
			const Vector3<f32> bar{ 0.f, 1.f, 0.f };
			OP_CHECK(foo.cross(bar).equals({ 0.f, 0.f, 1.f }));
		}

		OP_SUBCASE("len_sq") {
			const Vector3<f32> foo{ 1.f, 0.f, 0.f };
			OP_CHECK(math::equals(foo.len_sq(), 1.f));
		}

		OP_SUBCASE("len") {
			const Vector3<f32> foo = 1.f;
			OP_CHECK(math::equals(foo.len(), math::sqrt(2.f)));
		}

		OP_SUBCASE("normalized") {
			const Vector3<f32> a = 1.f;
			const auto a_normalized = a.normalized();
			OP_CHECK(a_normalized.is_set());
			OP_CHECK(a.normalized().unwrap().equals(math::sqrt(2.f) * 0.5f));

			const Vector3<f32> b = 0.f;
			OP_CHECK(!b.normalized().is_set());
		}

		OP_SUBCASE("contains") {
			const Vector3<i32> foo{ 0, 5, 0 };
			OP_CHECK(foo.contains(5));

			// Compiler will throw error if there is a divide by 0 in floating point literal
			f32 a = 1;
			a -= 1;
			const Vector3<f32> bar{ 1.f / a, 0.f, 0.f };
			OP_CHECK(bar.contains(math::nan<f32>));
		}

		OP_SUBCASE("equals") {
			const Vector3<f32> a = 1.f;
			OP_CHECK(a.equals(1.f));

			const Vector3<f32> b = 10.f;
			OP_CHECK(b.equals(7.f, 5.f));
		}

		OP_SUBCASE("min") {
			const Vector3<f32> a = 0.f;
			const Vector3<f32> b = 10.f;
			OP_CHECK(a.min(b).equals(a));

			const Vector3<f32> c{ 0.f, 10.f };
			const Vector3<f32> d{ 10.f, 0.f };
			OP_CHECK(c.min(d).equals(a));
		}

		OP_SUBCASE("max") {
			const Vector3<f32> a = 0.f;
			const Vector3<f32> b = 10.f;
			OP_CHECK(a.max(b).equals(b));

			const Vector3<f32> c{ 0.f, 10.f };
			const Vector3<f32> d{ 10.f, 0.f };
			OP_CHECK(c.max(d).equals(b));
		}

		OP_SUBCASE("as") {
			const Vector3<f32> a = 0.5f;
			const auto b = a.as<i32>();
			OP_CHECK(b.equals(0));
		}
	}
}
#endif // OP_ENABLE_TEST
