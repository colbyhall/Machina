/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Containers/SharedPtr.hpp>
#include <Core/Debug/Test.hpp>

#if MACH_ENABLE_TEST
MACH_TEST_SUITE("Containers") {
	using namespace Mach;

	MACH_TEST_CASE("SharedPtr") {
		MACH_SUBCASE("default constructor") {
			const auto shared = SharedPtr<int>::create(0);
			MACH_CHECK(*shared == 0);
		}

		MACH_SUBCASE("create") {
			const auto shared = SharedPtr<int>::create(100);
			MACH_CHECK(*shared == 100);
		}

		MACH_SUBCASE("copy constructor") {
			const auto shared = SharedPtr<int>::create(100);
			MACH_REQUIRE(*shared == 100);
			MACH_CHECK(shared.strong() == 1);
			const auto copy = shared;
			MACH_CHECK(*copy == 100);
			MACH_CHECK(shared.strong() == 2);
			MACH_CHECK(copy.strong() == 2);
			MACH_CHECK(copy.strong() == shared.strong());
			MACH_CHECK(&*shared == &*copy);
		}

		MACH_SUBCASE("copy assignment") {
			const auto shared = SharedPtr<int>::create(100);
			MACH_REQUIRE(*shared == 100);
			MACH_CHECK(shared.strong() == 1);
			auto copy_assignment = SharedPtr<int>::create(0);
			MACH_CHECK(*copy_assignment == 0);
			MACH_CHECK(copy_assignment.strong() == 1);
			copy_assignment = shared;
			MACH_CHECK(*copy_assignment == 100);
			MACH_CHECK(shared.strong() == 2);
			MACH_CHECK(copy_assignment.strong() == 2);
			MACH_CHECK(copy_assignment.strong() == shared.strong());
			MACH_CHECK(&*shared == &*copy_assignment);
		}

		MACH_SUBCASE("move constructor") {
			auto shared = SharedPtr<int>::create(100);
			MACH_REQUIRE(*shared == 100);
			MACH_CHECK(shared.strong() == 1);
			auto move = Mach::move(shared);
			MACH_CHECK(*move == 100);
			MACH_CHECK(shared.strong() == 0);
			MACH_CHECK(move.strong() == 1);
		}

		MACH_SUBCASE("move assignment") {
			auto shared = SharedPtr<int>::create(100);
			MACH_REQUIRE(*shared == 100);
			MACH_CHECK(shared.strong() == 1);
			auto move_assignment = SharedPtr<int>::create(0);
			MACH_CHECK(*move_assignment == 0);
			MACH_CHECK(move_assignment.strong() == 1);
			move_assignment = Mach::move(shared);
			MACH_CHECK(move_assignment.strong() == 1);
			MACH_CHECK(shared.strong() == 0);
			MACH_CHECK(*move_assignment == 100);
		}

		MACH_SUBCASE("destructor") {
			auto shared = SharedPtr<int>::create(0);
			MACH_CHECK(shared.strong() == 1);
			{
				const auto copy = shared;
				MACH_CHECK(shared.strong() == 2);
			}
			MACH_CHECK(shared.strong() == 1);
		}

		MACH_SUBCASE("downgrade") {
			const auto shared = SharedPtr<int>::create(100);
			MACH_REQUIRE(*shared == 100);
			MACH_CHECK(shared.strong() == 1);
			MACH_CHECK(shared.weak() == 0);
			{
				const auto weak = shared.downgrade();
				MACH_CHECK(shared.strong() == 1);
				MACH_CHECK(shared.weak() == 1);
				MACH_CHECK(shared.strong() == weak.strong());
				MACH_CHECK(shared.weak() == weak.weak());
			}
			MACH_CHECK(shared.strong() == 1);
			MACH_CHECK(shared.weak() == 0);
		}

		MACH_SUBCASE("upgrade") {
			const auto shared = SharedPtr<int>::create(100);
			MACH_REQUIRE(*shared == 100);
			MACH_CHECK(shared.strong() == 1);
			MACH_CHECK(shared.weak() == 0);
			{
				const auto weak = shared.downgrade();
				MACH_CHECK(shared.strong() == 1);
				MACH_CHECK(shared.weak() == 1);
				MACH_CHECK(shared.strong() == weak.strong());
				MACH_CHECK(shared.weak() == weak.weak());
				const auto upgrade = weak.upgrade();
				MACH_REQUIRE(upgrade.is_set());
				MACH_CHECK(shared.strong() == 2);
				MACH_CHECK(shared.weak() == 1);
				MACH_CHECK(shared.strong() == upgrade.as_const_ref().unwrap().strong());
				MACH_CHECK(shared.weak() == upgrade.as_const_ref().unwrap().weak());
			}
			MACH_CHECK(shared.strong() == 1);
			MACH_CHECK(shared.weak() == 0);
		}

		MACH_SUBCASE("SharedPtrFromThis") {
			class Test : public SharedPtrFromThis<Test> {
			public:
				int a = 120;
			};

			const auto test = SharedPtr<Test>::create();
			MACH_CHECK(test->a == 120);
			const auto& test_ref = *test;
			MACH_CHECK(test_ref.a == 120);
			const auto shared = test_ref.to_shared();
			MACH_CHECK(shared.strong() == 2);
			MACH_CHECK(shared->a == 120);
		}
	}
}
#endif // MACH_ENABLE_TEST
