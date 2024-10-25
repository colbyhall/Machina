/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Containers/Shared.hpp>
#include <Core/Debug/Test.hpp>

#if GRIZZLY_ENABLE_TEST
GRIZZLY_TEST_SUITE("Containers") {
	using namespace Grizzly;

	GRIZZLY_TEST_CASE("Shared") {
		GRIZZLY_SUBCASE("default constructor") {
			const Shared<int> shared;
			GRIZZLY_CHECK(*shared == 0);
		}

		GRIZZLY_SUBCASE("create") {
			const auto shared = Shared<int>::create(100);
			GRIZZLY_CHECK(*shared == 100);
		}

		GRIZZLY_SUBCASE("copy constructor") {
			const auto shared = Shared<int>::create(100);
			GRIZZLY_REQUIRE(*shared == 100);
			GRIZZLY_CHECK(shared.strong() == 1);
			const auto copy = shared;
			GRIZZLY_CHECK(*copy == 100);
			GRIZZLY_CHECK(shared.strong() == 2);
			GRIZZLY_CHECK(copy.strong() == 2);
			GRIZZLY_CHECK(copy.strong() == shared.strong());
			GRIZZLY_CHECK(&*shared == &*copy);
		}

		GRIZZLY_SUBCASE("copy assignment") {
			const auto shared = Shared<int>::create(100);
			GRIZZLY_REQUIRE(*shared == 100);
			GRIZZLY_CHECK(shared.strong() == 1);
			Shared<int> copy_assignment;
			GRIZZLY_CHECK(*copy_assignment == 0);
			GRIZZLY_CHECK(copy_assignment.strong() == 1);
			copy_assignment = shared;
			GRIZZLY_CHECK(*copy_assignment == 100);
			GRIZZLY_CHECK(shared.strong() == 2);
			GRIZZLY_CHECK(copy_assignment.strong() == 2);
			GRIZZLY_CHECK(copy_assignment.strong() == shared.strong());
			GRIZZLY_CHECK(&*shared == &*copy_assignment);
		}

		GRIZZLY_SUBCASE("move constructor") {
			auto shared = Shared<int>::create(100);
			GRIZZLY_REQUIRE(*shared == 100);
			GRIZZLY_CHECK(shared.strong() == 1);
			auto move = Grizzly::move(shared);
			GRIZZLY_CHECK(*move == 100);
			GRIZZLY_CHECK(static_cast<int*>(shared) == nullptr);
			GRIZZLY_CHECK(shared.strong() == 0);
			GRIZZLY_CHECK(move.strong() == 1);
		}

		GRIZZLY_SUBCASE("move assignment") {
			auto shared = Shared<int>::create(100);
			GRIZZLY_REQUIRE(*shared == 100);
			GRIZZLY_CHECK(shared.strong() == 1);
			Shared<int> move_assignment;
			GRIZZLY_CHECK(*move_assignment == 0);
			GRIZZLY_CHECK(move_assignment.strong() == 1);
			move_assignment = Grizzly::move(shared);
			GRIZZLY_CHECK(move_assignment.strong() == 1);
			GRIZZLY_CHECK(shared.strong() == 0);
			GRIZZLY_CHECK(*move_assignment == 100);
			GRIZZLY_CHECK(static_cast<int*>(shared) == nullptr);
		}

		GRIZZLY_SUBCASE("destructor") {
			Shared<int> shared;
			GRIZZLY_CHECK(shared.strong() == 1);
			{
				const auto copy = shared;
				GRIZZLY_CHECK(shared.strong() == 2);
			}
			GRIZZLY_CHECK(shared.strong() == 1);
		}

		GRIZZLY_SUBCASE("downgrade") {
			const auto shared = Shared<int>::create(100);
			GRIZZLY_REQUIRE(*shared == 100);
			GRIZZLY_CHECK(shared.strong() == 1);
			GRIZZLY_CHECK(shared.weak() == 0);
			{
				const auto weak = shared.downgrade();
				GRIZZLY_CHECK(shared.strong() == 1);
				GRIZZLY_CHECK(shared.weak() == 1);
				GRIZZLY_CHECK(shared.strong() == weak.strong());
				GRIZZLY_CHECK(shared.weak() == weak.weak());
			}
			GRIZZLY_CHECK(shared.strong() == 1);
			GRIZZLY_CHECK(shared.weak() == 0);
		}

		GRIZZLY_SUBCASE("upgrade") {
			const auto shared = Shared<int>::create(100);
			GRIZZLY_REQUIRE(*shared == 100);
			GRIZZLY_CHECK(shared.strong() == 1);
			GRIZZLY_CHECK(shared.weak() == 0);
			{
				const auto weak = shared.downgrade();
				GRIZZLY_CHECK(shared.strong() == 1);
				GRIZZLY_CHECK(shared.weak() == 1);
				GRIZZLY_CHECK(shared.strong() == weak.strong());
				GRIZZLY_CHECK(shared.weak() == weak.weak());
				const auto upgrade = weak.upgrade();
				GRIZZLY_REQUIRE(upgrade.is_set());
				GRIZZLY_CHECK(shared.strong() == 2);
				GRIZZLY_CHECK(shared.weak() == 1);
				GRIZZLY_CHECK(shared.strong() == upgrade.as_const_ref().unwrap().strong());
				GRIZZLY_CHECK(shared.weak() == upgrade.as_const_ref().unwrap().weak());
			}
			GRIZZLY_CHECK(shared.strong() == 1);
			GRIZZLY_CHECK(shared.weak() == 0);
		}

		GRIZZLY_SUBCASE("SharedFromThis") {
			class Test : public SharedFromThis<Test> {
			public:
				int a = 120;
			};

			const auto test = Shared<Test>::create();
			GRIZZLY_CHECK(test->a == 120);
			const auto& test_ref = *test;
			GRIZZLY_CHECK(test_ref.a == 120);
			const auto shared = test_ref.to_shared();
			GRIZZLY_CHECK(shared.strong() == 2);
			GRIZZLY_CHECK(shared->a == 120);
		}
	}

	GRIZZLY_TEST_CASE("AtomicShared") {
		GRIZZLY_SUBCASE("default constructor") {
			const AtomicShared<int> shared;
			GRIZZLY_CHECK(*shared == 0);
		}

		GRIZZLY_SUBCASE("create") {
			const auto shared = AtomicShared<int>::create(100);
			GRIZZLY_CHECK(*shared == 100);
		}

		GRIZZLY_SUBCASE("copy constructor") {
			const auto shared = AtomicShared<int>::create(100);
			GRIZZLY_REQUIRE(*shared == 100);
			GRIZZLY_CHECK(shared.strong() == 1);
			const auto copy = shared;
			GRIZZLY_CHECK(*copy == 100);
			GRIZZLY_CHECK(shared.strong() == 2);
			GRIZZLY_CHECK(copy.strong() == 2);
			GRIZZLY_CHECK(copy.strong() == shared.strong());
			GRIZZLY_CHECK(&*shared == &*copy);
		}

		GRIZZLY_SUBCASE("copy assignment") {
			const auto shared = AtomicShared<int>::create(100);
			GRIZZLY_REQUIRE(*shared == 100);
			GRIZZLY_CHECK(shared.strong() == 1);
			AtomicShared<int> copy_assignment;
			GRIZZLY_CHECK(*copy_assignment == 0);
			GRIZZLY_CHECK(copy_assignment.strong() == 1);
			copy_assignment = shared;
			GRIZZLY_CHECK(*copy_assignment == 100);
			GRIZZLY_CHECK(shared.strong() == 2);
			GRIZZLY_CHECK(copy_assignment.strong() == 2);
			GRIZZLY_CHECK(copy_assignment.strong() == shared.strong());
			GRIZZLY_CHECK(&*shared == &*copy_assignment);
		}

		GRIZZLY_SUBCASE("move constructor") {
			auto shared = AtomicShared<int>::create(100);
			GRIZZLY_REQUIRE(*shared == 100);
			GRIZZLY_CHECK(shared.strong() == 1);
			auto move = Grizzly::move(shared);
			GRIZZLY_CHECK(*move == 100);
			GRIZZLY_CHECK(static_cast<int*>(shared) == nullptr);
			GRIZZLY_CHECK(shared.strong() == 0);
			GRIZZLY_CHECK(move.strong() == 1);
		}

		GRIZZLY_SUBCASE("move assignment") {
			auto shared = AtomicShared<int>::create(100);
			GRIZZLY_REQUIRE(*shared == 100);
			GRIZZLY_CHECK(shared.strong() == 1);
			AtomicShared<int> move_assignment;
			GRIZZLY_CHECK(*move_assignment == 0);
			GRIZZLY_CHECK(move_assignment.strong() == 1);
			move_assignment = Grizzly::move(shared);
			GRIZZLY_CHECK(move_assignment.strong() == 1);
			GRIZZLY_CHECK(shared.strong() == 0);
			GRIZZLY_CHECK(*move_assignment == 100);
			GRIZZLY_CHECK(static_cast<int*>(shared) == nullptr);
		}

		GRIZZLY_SUBCASE("destructor") {
			AtomicShared<int> shared;
			GRIZZLY_CHECK(shared.strong() == 1);
			{
				const auto copy = shared;
				GRIZZLY_CHECK(shared.strong() == 2);
			}
			GRIZZLY_CHECK(shared.strong() == 1);
		}

		GRIZZLY_SUBCASE("downgrade") {
			const auto shared = AtomicShared<int>::create(100);
			GRIZZLY_REQUIRE(*shared == 100);
			GRIZZLY_CHECK(shared.strong() == 1);
			GRIZZLY_CHECK(shared.weak() == 0);
			{
				const auto weak = shared.downgrade();
				GRIZZLY_CHECK(shared.strong() == 1);
				GRIZZLY_CHECK(shared.weak() == 1);
				GRIZZLY_CHECK(shared.strong() == weak.strong());
				GRIZZLY_CHECK(shared.weak() == weak.weak());
			}
			GRIZZLY_CHECK(shared.strong() == 1);
			GRIZZLY_CHECK(shared.weak() == 0);
		}

		GRIZZLY_SUBCASE("upgrade") {
			const auto shared = AtomicShared<int>::create(100);
			GRIZZLY_REQUIRE(*shared == 100);
			GRIZZLY_CHECK(shared.strong() == 1);
			GRIZZLY_CHECK(shared.weak() == 0);
			{
				const auto weak = shared.downgrade();
				GRIZZLY_CHECK(shared.strong() == 1);
				GRIZZLY_CHECK(shared.weak() == 1);
				GRIZZLY_CHECK(shared.strong() == weak.strong());
				GRIZZLY_CHECK(shared.weak() == weak.weak());
				const auto upgrade = weak.upgrade();
				GRIZZLY_REQUIRE(upgrade.is_set());
				GRIZZLY_CHECK(shared.strong() == 2);
				GRIZZLY_CHECK(shared.weak() == 1);
				GRIZZLY_CHECK(shared.strong() == upgrade.as_const_ref().unwrap().strong());
				GRIZZLY_CHECK(shared.weak() == upgrade.as_const_ref().unwrap().weak());
			}
			GRIZZLY_CHECK(shared.strong() == 1);
			GRIZZLY_CHECK(shared.weak() == 0);
		}

		GRIZZLY_SUBCASE("AtomicSharedFromThis") {
			class Test : public AtomicSharedFromThis<Test> {
			public:
				int a = 120;
			};

			const auto test = AtomicShared<Test>::create();
			GRIZZLY_CHECK(test->a == 120);
			const auto& test_ref = *test;
			GRIZZLY_CHECK(test_ref.a == 120);
			const auto shared = test_ref.to_shared();
			GRIZZLY_CHECK(shared.strong() == 2);
			GRIZZLY_CHECK(shared->a == 120);
		}
	}
}
#endif // GRIZZLY_ENABLE_TEST
