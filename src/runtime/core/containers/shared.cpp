/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include "core/containers/shared.h"
#include "core/debug/test.h"

using namespace op;

OP_TEST_SUITE("containers") {
	OP_TEST_CASE("Shared") {
		OP_SUBCASE("default constructor") {
			const Shared<int> shared;
			OP_CHECK(*shared == 0);
		}

		OP_SUBCASE("create") {
			const auto shared = Shared<int>::create(100);
			OP_CHECK(*shared == 100);
		}

		OP_SUBCASE("copy constructor") {
			const auto shared = Shared<int>::create(100);
			OP_REQUIRE(*shared == 100);
			OP_CHECK(shared.strong() == 1);
			const auto copy = shared;
			OP_CHECK(*copy == 100);
			OP_CHECK(shared.strong() == 2);
			OP_CHECK(copy.strong() == 2);
			OP_CHECK(copy.strong() == shared.strong());
			OP_CHECK(&*shared == &*copy);
		}

		OP_SUBCASE("copy assignment") {
			const auto shared = Shared<int>::create(100);
			OP_REQUIRE(*shared == 100);
			OP_CHECK(shared.strong() == 1);
			Shared<int> copy_assignment;
			OP_CHECK(*copy_assignment == 0);
			OP_CHECK(copy_assignment.strong() == 1);
			copy_assignment = shared;
			OP_CHECK(*copy_assignment == 100);
			OP_CHECK(shared.strong() == 2);
			OP_CHECK(copy_assignment.strong() == 2);
			OP_CHECK(copy_assignment.strong() == shared.strong());
			OP_CHECK(&*shared == &*copy_assignment);
		}

		OP_SUBCASE("move constructor") {
			auto shared = Shared<int>::create(100);
			OP_REQUIRE(*shared == 100);
			OP_CHECK(shared.strong() == 1);
			auto move = op::move(shared);
			OP_CHECK(*move == 100);
			OP_CHECK(static_cast<int*>(shared) == nullptr);
			OP_CHECK(shared.strong() == 0);
			OP_CHECK(move.strong() == 1);
		}

		OP_SUBCASE("move assignment") {
			auto shared = Shared<int>::create(100);
			OP_REQUIRE(*shared == 100);
			OP_CHECK(shared.strong() == 1);
			Shared<int> move_assignment;
			OP_CHECK(*move_assignment == 0);
			OP_CHECK(move_assignment.strong() == 1);
			move_assignment = op::move(shared);
			OP_CHECK(move_assignment.strong() == 1);
			OP_CHECK(shared.strong() == 0);
			OP_CHECK(*move_assignment == 100);
			OP_CHECK(static_cast<int*>(shared) == nullptr);
		}

		OP_SUBCASE("destructor") {
			Shared<int> shared;
			OP_CHECK(shared.strong() == 1);
			{
				const auto copy = shared;
				OP_CHECK(shared.strong() == 2);
			}
			OP_CHECK(shared.strong() == 1);
		}

		OP_SUBCASE("downgrade") {
			const auto shared = Shared<int>::create(100);
			OP_REQUIRE(*shared == 100);
			OP_CHECK(shared.strong() == 1);
			OP_CHECK(shared.weak() == 0);
			{
				const auto weak = shared.downgrade();
				OP_CHECK(shared.strong() == 1);
				OP_CHECK(shared.weak() == 1);
				OP_CHECK(shared.strong() == weak.strong());
				OP_CHECK(shared.weak() == weak.weak());
			}
			OP_CHECK(shared.strong() == 1);
			OP_CHECK(shared.weak() == 0);
		}

		OP_SUBCASE("upgrade") {
			const auto shared = Shared<int>::create(100);
			OP_REQUIRE(*shared == 100);
			OP_CHECK(shared.strong() == 1);
			OP_CHECK(shared.weak() == 0);
			{
				const auto weak = shared.downgrade();
				OP_CHECK(shared.strong() == 1);
				OP_CHECK(shared.weak() == 1);
				OP_CHECK(shared.strong() == weak.strong());
				OP_CHECK(shared.weak() == weak.weak());
				const auto upgrade = weak.upgrade();
				OP_REQUIRE(upgrade.is_set());
				OP_CHECK(shared.strong() == 2);
				OP_CHECK(shared.weak() == 1);
				OP_CHECK(shared.strong() == upgrade.as_const_ref().unwrap().strong());
				OP_CHECK(shared.weak() == upgrade.as_const_ref().unwrap().weak());
			}
			OP_CHECK(shared.strong() == 1);
			OP_CHECK(shared.weak() == 0);
		}

		OP_SUBCASE("SharedFromThis") {
			class Test : public SharedFromThis<Test> {
			public:
				int a = 120;
			};

			const auto test = Shared<Test>::create();
			OP_CHECK(test->a == 120);
			const auto& test_ref = *test;
			OP_CHECK(test_ref.a == 120);
			const auto shared = test_ref.to_shared();
			OP_CHECK(shared.strong() == 2);
			OP_CHECK(shared->a == 120);
		}
	}

	OP_TEST_CASE("AtomicShared") {
		OP_SUBCASE("default constructor") {
			const AtomicShared<int> shared;
			OP_CHECK(*shared == 0);
		}

		OP_SUBCASE("create") {
			const auto shared = AtomicShared<int>::create(100);
			OP_CHECK(*shared == 100);
		}

		OP_SUBCASE("copy constructor") {
			const auto shared = AtomicShared<int>::create(100);
			OP_REQUIRE(*shared == 100);
			OP_CHECK(shared.strong() == 1);
			const auto copy = shared;
			OP_CHECK(*copy == 100);
			OP_CHECK(shared.strong() == 2);
			OP_CHECK(copy.strong() == 2);
			OP_CHECK(copy.strong() == shared.strong());
			OP_CHECK(&*shared == &*copy);
		}

		OP_SUBCASE("copy assignment") {
			const auto shared = AtomicShared<int>::create(100);
			OP_REQUIRE(*shared == 100);
			OP_CHECK(shared.strong() == 1);
			AtomicShared<int> copy_assignment;
			OP_CHECK(*copy_assignment == 0);
			OP_CHECK(copy_assignment.strong() == 1);
			copy_assignment = shared;
			OP_CHECK(*copy_assignment == 100);
			OP_CHECK(shared.strong() == 2);
			OP_CHECK(copy_assignment.strong() == 2);
			OP_CHECK(copy_assignment.strong() == shared.strong());
			OP_CHECK(&*shared == &*copy_assignment);
		}

		OP_SUBCASE("move constructor") {
			auto shared = AtomicShared<int>::create(100);
			OP_REQUIRE(*shared == 100);
			OP_CHECK(shared.strong() == 1);
			auto move = op::move(shared);
			OP_CHECK(*move == 100);
			OP_CHECK(static_cast<int*>(shared) == nullptr);
			OP_CHECK(shared.strong() == 0);
			OP_CHECK(move.strong() == 1);
		}

		OP_SUBCASE("move assignment") {
			auto shared = AtomicShared<int>::create(100);
			OP_REQUIRE(*shared == 100);
			OP_CHECK(shared.strong() == 1);
			AtomicShared<int> move_assignment;
			OP_CHECK(*move_assignment == 0);
			OP_CHECK(move_assignment.strong() == 1);
			move_assignment = op::move(shared);
			OP_CHECK(move_assignment.strong() == 1);
			OP_CHECK(shared.strong() == 0);
			OP_CHECK(*move_assignment == 100);
			OP_CHECK(static_cast<int*>(shared) == nullptr);
		}

		OP_SUBCASE("destructor") {
			AtomicShared<int> shared;
			OP_CHECK(shared.strong() == 1);
			{
				const auto copy = shared;
				OP_CHECK(shared.strong() == 2);
			}
			OP_CHECK(shared.strong() == 1);
		}

		OP_SUBCASE("downgrade") {
			const auto shared = AtomicShared<int>::create(100);
			OP_REQUIRE(*shared == 100);
			OP_CHECK(shared.strong() == 1);
			OP_CHECK(shared.weak() == 0);
			{
				const auto weak = shared.downgrade();
				OP_CHECK(shared.strong() == 1);
				OP_CHECK(shared.weak() == 1);
				OP_CHECK(shared.strong() == weak.strong());
				OP_CHECK(shared.weak() == weak.weak());
			}
			OP_CHECK(shared.strong() == 1);
			OP_CHECK(shared.weak() == 0);
		}

		OP_SUBCASE("upgrade") {
			const auto shared = AtomicShared<int>::create(100);
			OP_REQUIRE(*shared == 100);
			OP_CHECK(shared.strong() == 1);
			OP_CHECK(shared.weak() == 0);
			{
				const auto weak = shared.downgrade();
				OP_CHECK(shared.strong() == 1);
				OP_CHECK(shared.weak() == 1);
				OP_CHECK(shared.strong() == weak.strong());
				OP_CHECK(shared.weak() == weak.weak());
				const auto upgrade = weak.upgrade();
				OP_REQUIRE(upgrade.is_set());
				OP_CHECK(shared.strong() == 2);
				OP_CHECK(shared.weak() == 1);
				OP_CHECK(shared.strong() == upgrade.as_const_ref().unwrap().strong());
				OP_CHECK(shared.weak() == upgrade.as_const_ref().unwrap().weak());
			}
			OP_CHECK(shared.strong() == 1);
			OP_CHECK(shared.weak() == 0);
		}

		OP_SUBCASE("AtomicSharedFromThis") {
			class Test : public AtomicSharedFromThis<Test> {
			public:
				int a = 120;
			};

			const auto test = AtomicShared<Test>::create();
			OP_CHECK(test->a == 120);
			const auto& test_ref = *test;
			OP_CHECK(test_ref.a == 120);
			const auto shared = test_ref.to_shared();
			OP_CHECK(shared.strong() == 2);
			OP_CHECK(shared->a == 120);
		}
	}
}