/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Containers/Shared.hpp>
#include <Core/Debug/Test.hpp>

#if FORGE_ENABLE_TEST
FORGE_TEST_SUITE("Containers") {
	using namespace Forge;

	FORGE_TEST_CASE("Shared") {
		FORGE_SUBCASE("create") {
			const auto shared = Rc<int>::create(100);
			FORGE_CHECK(*shared == 100);
		}

		FORGE_SUBCASE("copy constructor") {
			const auto shared = Rc<int>::create(100);
			FORGE_REQUIRE(*shared == 100);
			FORGE_CHECK(shared.strong() == 1);
			const auto copy = shared;
			FORGE_CHECK(*copy == 100);
			FORGE_CHECK(shared.strong() == 2);
			FORGE_CHECK(copy.strong() == 2);
			FORGE_CHECK(copy.strong() == shared.strong());
			FORGE_CHECK(&*shared == &*copy);
		}

		FORGE_SUBCASE("copy assignment") {
			const auto shared = Rc<int>::create(100);
			FORGE_REQUIRE(*shared == 100);
			FORGE_CHECK(shared.strong() == 1);
			auto copy_assignment = Rc<int>::create(0);
			FORGE_CHECK(*copy_assignment == 0);
			FORGE_CHECK(copy_assignment.strong() == 1);
			copy_assignment = shared;
			FORGE_CHECK(*copy_assignment == 100);
			FORGE_CHECK(shared.strong() == 2);
			FORGE_CHECK(copy_assignment.strong() == 2);
			FORGE_CHECK(copy_assignment.strong() == shared.strong());
			FORGE_CHECK(&*shared == &*copy_assignment);
		}

		FORGE_SUBCASE("move constructor") {
			auto shared = Rc<int>::create(100);
			FORGE_REQUIRE(*shared == 100);
			FORGE_CHECK(shared.strong() == 1);
			auto move = Forge::move(shared);
			FORGE_CHECK(*move == 100);
			FORGE_CHECK(static_cast<int*>(shared) == nullptr);
			FORGE_CHECK(shared.strong() == 0);
			FORGE_CHECK(move.strong() == 1);
		}

		FORGE_SUBCASE("move assignment") {
			auto shared = Rc<int>::create(100);
			FORGE_REQUIRE(*shared == 100);
			FORGE_CHECK(shared.strong() == 1);
			auto move_assignment = Rc<int>::create(0);
			FORGE_CHECK(*move_assignment == 0);
			FORGE_CHECK(move_assignment.strong() == 1);
			move_assignment = Forge::move(shared);
			FORGE_CHECK(move_assignment.strong() == 1);
			FORGE_CHECK(shared.strong() == 0);
			FORGE_CHECK(*move_assignment == 100);
			FORGE_CHECK(static_cast<int*>(shared) == nullptr);
		}

		FORGE_SUBCASE("destructor") {
			auto shared = Rc<int>::create();
			FORGE_CHECK(shared.strong() == 1);
			{
				const auto copy = shared;
				FORGE_CHECK(shared.strong() == 2);
			}
			FORGE_CHECK(shared.strong() == 1);
		}

		FORGE_SUBCASE("downgrade") {
			const auto shared = Rc<int>::create(100);
			FORGE_REQUIRE(*shared == 100);
			FORGE_CHECK(shared.strong() == 1);
			FORGE_CHECK(shared.weak() == 0);
			{
				const auto weak = shared.downgrade();
				FORGE_CHECK(shared.strong() == 1);
				FORGE_CHECK(shared.weak() == 1);
				FORGE_CHECK(shared.strong() == weak.strong());
				FORGE_CHECK(shared.weak() == weak.weak());
			}
			FORGE_CHECK(shared.strong() == 1);
			FORGE_CHECK(shared.weak() == 0);
		}

		FORGE_SUBCASE("upgrade") {
			const auto shared = Rc<int>::create(100);
			FORGE_REQUIRE(*shared == 100);
			FORGE_CHECK(shared.strong() == 1);
			FORGE_CHECK(shared.weak() == 0);
			{
				const auto weak = shared.downgrade();
				FORGE_CHECK(shared.strong() == 1);
				FORGE_CHECK(shared.weak() == 1);
				FORGE_CHECK(shared.strong() == weak.strong());
				FORGE_CHECK(shared.weak() == weak.weak());
				const auto upgrade = weak.upgrade();
				FORGE_REQUIRE(upgrade.is_set());
				FORGE_CHECK(shared.strong() == 2);
				FORGE_CHECK(shared.weak() == 1);
				FORGE_CHECK(shared.strong() == upgrade.as_const_ref().unwrap().strong());
				FORGE_CHECK(shared.weak() == upgrade.as_const_ref().unwrap().weak());
			}
			FORGE_CHECK(shared.strong() == 1);
			FORGE_CHECK(shared.weak() == 0);
		}

		FORGE_SUBCASE("RcFromThis") {
			class Test : public RcFromThis<Test> {
			public:
				int a = 120;
			};

			const auto test = Rc<Test>::create();
			FORGE_CHECK(test->a == 120);
			const auto& test_ref = *test;
			FORGE_CHECK(test_ref.a == 120);
			const auto shared = test_ref.to_shared();
			FORGE_CHECK(shared.strong() == 2);
			FORGE_CHECK(shared->a == 120);
		}
	}

	FORGE_TEST_CASE("Arc") {
		FORGE_SUBCASE("default constructor") {
			const auto shared = Arc<int>::create(0);
			FORGE_CHECK(*shared == 0);
		}

		FORGE_SUBCASE("create") {
			const auto shared = Arc<int>::create(100);
			FORGE_CHECK(*shared == 100);
		}

		FORGE_SUBCASE("copy constructor") {
			const auto shared = Arc<int>::create(100);
			FORGE_REQUIRE(*shared == 100);
			FORGE_CHECK(shared.strong() == 1);
			const auto copy = shared;
			FORGE_CHECK(*copy == 100);
			FORGE_CHECK(shared.strong() == 2);
			FORGE_CHECK(copy.strong() == 2);
			FORGE_CHECK(copy.strong() == shared.strong());
			FORGE_CHECK(&*shared == &*copy);
		}

		FORGE_SUBCASE("copy assignment") {
			const auto shared = Arc<int>::create(100);
			FORGE_REQUIRE(*shared == 100);
			FORGE_CHECK(shared.strong() == 1);
			auto copy_assignment = Arc<int>::create(0);
			FORGE_CHECK(*copy_assignment == 0);
			FORGE_CHECK(copy_assignment.strong() == 1);
			copy_assignment = shared;
			FORGE_CHECK(*copy_assignment == 100);
			FORGE_CHECK(shared.strong() == 2);
			FORGE_CHECK(copy_assignment.strong() == 2);
			FORGE_CHECK(copy_assignment.strong() == shared.strong());
			FORGE_CHECK(&*shared == &*copy_assignment);
		}

		FORGE_SUBCASE("move constructor") {
			auto shared = Arc<int>::create(100);
			FORGE_REQUIRE(*shared == 100);
			FORGE_CHECK(shared.strong() == 1);
			auto move = Forge::move(shared);
			FORGE_CHECK(*move == 100);
			FORGE_CHECK(shared.strong() == 0);
			FORGE_CHECK(move.strong() == 1);
		}

		FORGE_SUBCASE("move assignment") {
			auto shared = Arc<int>::create(100);
			FORGE_REQUIRE(*shared == 100);
			FORGE_CHECK(shared.strong() == 1);
			auto move_assignment = Arc<int>::create(0);
			FORGE_CHECK(*move_assignment == 0);
			FORGE_CHECK(move_assignment.strong() == 1);
			move_assignment = Forge::move(shared);
			FORGE_CHECK(move_assignment.strong() == 1);
			FORGE_CHECK(shared.strong() == 0);
			FORGE_CHECK(*move_assignment == 100);
		}

		FORGE_SUBCASE("destructor") {
			auto shared = Arc<int>::create(0);
			FORGE_CHECK(shared.strong() == 1);
			{
				const auto copy = shared;
				FORGE_CHECK(shared.strong() == 2);
			}
			FORGE_CHECK(shared.strong() == 1);
		}

		FORGE_SUBCASE("downgrade") {
			const auto shared = Arc<int>::create(100);
			FORGE_REQUIRE(*shared == 100);
			FORGE_CHECK(shared.strong() == 1);
			FORGE_CHECK(shared.weak() == 0);
			{
				const auto weak = shared.downgrade();
				FORGE_CHECK(shared.strong() == 1);
				FORGE_CHECK(shared.weak() == 1);
				FORGE_CHECK(shared.strong() == weak.strong());
				FORGE_CHECK(shared.weak() == weak.weak());
			}
			FORGE_CHECK(shared.strong() == 1);
			FORGE_CHECK(shared.weak() == 0);
		}

		FORGE_SUBCASE("upgrade") {
			const auto shared = Arc<int>::create(100);
			FORGE_REQUIRE(*shared == 100);
			FORGE_CHECK(shared.strong() == 1);
			FORGE_CHECK(shared.weak() == 0);
			{
				const auto weak = shared.downgrade();
				FORGE_CHECK(shared.strong() == 1);
				FORGE_CHECK(shared.weak() == 1);
				FORGE_CHECK(shared.strong() == weak.strong());
				FORGE_CHECK(shared.weak() == weak.weak());
				const auto upgrade = weak.upgrade();
				FORGE_REQUIRE(upgrade.is_set());
				FORGE_CHECK(shared.strong() == 2);
				FORGE_CHECK(shared.weak() == 1);
				FORGE_CHECK(shared.strong() == upgrade.as_const_ref().unwrap().strong());
				FORGE_CHECK(shared.weak() == upgrade.as_const_ref().unwrap().weak());
			}
			FORGE_CHECK(shared.strong() == 1);
			FORGE_CHECK(shared.weak() == 0);
		}

		FORGE_SUBCASE("ArcFromThis") {
			class Test : public ArcFromThis<Test> {
			public:
				int a = 120;
			};

			const auto test = Arc<Test>::create();
			FORGE_CHECK(test->a == 120);
			const auto& test_ref = *test;
			FORGE_CHECK(test_ref.a == 120);
			const auto shared = test_ref.to_shared();
			FORGE_CHECK(shared.strong() == 2);
			FORGE_CHECK(shared->a == 120);
		}
	}
}
#endif // FORGE_ENABLE_TEST
