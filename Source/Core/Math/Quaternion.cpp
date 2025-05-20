/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Debug/Test.hpp>
#include <Core/Math/Quaternion.hpp>

#if FORGE_ENABLE_TEST
FORGE_TEST_SUITE("Math") {
	using namespace Forge;

	FORGE_TEST_CASE("Quaternion") {
		FORGE_SUBCASE("default initialize") {
			Quaternion<f32> q;
			FORGE_CHECK(q.x == 0.f);
			FORGE_CHECK(q.y == 0.f);
			FORGE_CHECK(q.z == 0.f);
			FORGE_CHECK(q.w == 1.f);
		}

		FORGE_SUBCASE("x, y, z, w initialize") {
			Quaternion<f32> q{ 1.f, 2.f, 3.f, 4.f };
			FORGE_CHECK(q.x == 1.f);
			FORGE_CHECK(q.y == 2.f);
			FORGE_CHECK(q.z == 3.f);
			FORGE_CHECK(q.w == 4.f);
		}

		FORGE_SUBCASE("from_axis_angle") {
			const auto q = Quaternion<f32>::from_axis_angle({ 1.f, 0.f, 0.f }, Math::deg_to_rad<f32> * 90.f);
			FORGE_CHECK(q.x == 0.70710677f);
			FORGE_CHECK(q.y == 0.f);
			FORGE_CHECK(q.z == 0.f);
			FORGE_CHECK(q.w == 0.70710677f);
		}

		FORGE_SUBCASE("from_euler") {
			const auto q = Quaternion<f32>::from_euler(90.f, 0.f, 0.f);
			FORGE_CHECK(q.x == 0.f);
			FORGE_CHECK(Math::equals(q.y, -0.70710677f));
			FORGE_CHECK(q.z == 0.f);
			FORGE_CHECK(Math::equals(q.w, 0.70710677f));
		}

		FORGE_SUBCASE("identity") {
			const auto q = Quaternion<f32>::identity();
			FORGE_CHECK(q.x == 0.f);
			FORGE_CHECK(q.y == 0.f);
			FORGE_CHECK(q.z == 0.f);
			FORGE_CHECK(q.w == 1.f);
		}

		FORGE_SUBCASE("len_sq") {
			const auto q = Quaternion<f32>{ 1.f, 2.f, 3.f, 4.f };
			FORGE_CHECK(q.len_sq() == 30.f);
		}

		FORGE_SUBCASE("len") {
			const auto q = Quaternion<f32>{ 1.f, 2.f, 3.f, 4.f };
			FORGE_CHECK(q.len() == 5.47722578f);
		}

		FORGE_SUBCASE("normalized") {
			const auto q = Quaternion<f32>{ 1.f, 2.f, 3.f, 4.f };
			const auto n = q.normalized();
			FORGE_CHECK(n.is_set());
			FORGE_CHECK(Math::equals(n.unwrap().x, 0.18257418f));
			FORGE_CHECK(Math::equals(n.unwrap().y, 0.36514837f));
			FORGE_CHECK(Math::equals(n.unwrap().z, 0.54772251f));
			FORGE_CHECK(Math::equals(n.unwrap().w, 0.73029673f));
		}

		FORGE_SUBCASE("inverse") {
			const auto q = Quaternion<f32>{ 1.f, 2.f, 3.f, 4.f };
			const auto i = q.inverse();
			FORGE_CHECK(i.x == -1.f);
			FORGE_CHECK(i.y == -2.f);
			FORGE_CHECK(i.z == -3.f);
			FORGE_CHECK(i.w == 4.f);
		}

		FORGE_SUBCASE("rotate") {
			const auto q = Quaternion<f32>::from_axis_angle({ 1.f, 0.f, 0.f }, Math::deg_to_rad<f32> * 90.f);
			const auto v = q.rotate({ 0.f, 1.f, 0.f });
			FORGE_CHECK(Math::equals(v.x, 0.f));
			FORGE_CHECK(Math::equals(v.y, 0.f));
			FORGE_CHECK(Math::equals(v.z, 1.f));
		}

		FORGE_SUBCASE("operator*") {
			const auto q1 = Quaternion<f32>{ 1.f, 2.f, 3.f, 4.f };
			const auto q2 = Quaternion<f32>{ 5.f, 6.f, 7.f, 8.f };
			const auto q3 = q1 * q2;
			FORGE_CHECK(q3.x == 24.f);
			FORGE_CHECK(q3.y == 48.f);
			FORGE_CHECK(q3.z == 48.f);
			FORGE_CHECK(q3.w == -6.f);
		}
	}
}
#endif // FORGE_ENABLE_TEST
