/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Containers/Unique.h>
#include <Core/Debug/Test.h>

GRIZZLY_TEST_SUITE("Containers") {
	using namespace Grizzly::Core;

	GRIZZLY_TEST_CASE("Unique") {
		struct Foo {
			int a = 0;
		};

		GRIZZLY_SUBCASE("default constructor") {
			const Unique<Foo> unique;
			GRIZZLY_CHECK(unique->a == 0);
		}

		GRIZZLY_SUBCASE("create") {
			const auto unique = Unique<Foo>::create(Foo{ .a = 100 });
			GRIZZLY_CHECK(unique->a == 100);
		}

		GRIZZLY_SUBCASE("copy constructor") {
			const auto unique = Unique<Foo>::create(Foo{ .a = 100 });
			GRIZZLY_REQUIRE(unique->a == 100);
			const auto copy = unique;
			GRIZZLY_CHECK(copy->a == 100);
		}

		GRIZZLY_SUBCASE("copy assignment") {
			const auto unique = Unique<Foo>::create(Foo{ .a = 100 });
			GRIZZLY_REQUIRE(unique->a == 100);
			Unique<Foo> copy_assignment;
			GRIZZLY_CHECK(copy_assignment->a == 0);
			copy_assignment = unique;
			GRIZZLY_CHECK(copy_assignment->a == 100);
		}

		GRIZZLY_SUBCASE("move constructor") {
			auto unique = Unique<Foo>::create(Foo{ .a = 100 });
			GRIZZLY_REQUIRE(unique->a == 100);
			const auto move = Grizzly::move(unique);
			GRIZZLY_CHECK(move->a == 100);
			GRIZZLY_CHECK(static_cast<Foo*>(unique) == nullptr);
		}

		GRIZZLY_SUBCASE("move assignment") {
			auto unique = Unique<Foo>::create(Foo{ .a = 100 });
			GRIZZLY_REQUIRE(unique->a == 100);
			Unique<Foo> move_assignment;
			GRIZZLY_CHECK(move_assignment->a == 0);
			move_assignment = Grizzly::move(unique);
			GRIZZLY_CHECK(move_assignment->a == 100);
			GRIZZLY_CHECK(static_cast<Foo*>(unique) == nullptr);
		}

		struct Bar : Foo {
			int b = 0;
		};

		GRIZZLY_SUBCASE("derived move constructor") {
			auto unique = Unique<Bar>::create(Bar{ .b = 200 });
			GRIZZLY_REQUIRE(unique->a == 0);
			GRIZZLY_REQUIRE(unique->b == 200);
			const Unique<Foo> move = Grizzly::move(unique);
			GRIZZLY_CHECK(move->a == 0);
			GRIZZLY_CHECK(static_cast<Bar*>(unique) == nullptr);
			GRIZZLY_CHECK(static_cast<const Bar*>(static_cast<const Foo*>(move))->b == 200);
		}

		GRIZZLY_SUBCASE("derived move assignment") {
			auto unique = Unique<Bar>::create(Bar{ .b = 200 });
			GRIZZLY_REQUIRE(unique->a == 0);
			GRIZZLY_REQUIRE(unique->b == 200);
			Unique<Foo> move_assignment;
			GRIZZLY_CHECK(move_assignment->a == 0);
			move_assignment = Grizzly::move(unique);
			GRIZZLY_CHECK(move_assignment->a == 0);
			GRIZZLY_CHECK(static_cast<Bar*>(unique) == nullptr);
			GRIZZLY_CHECK(static_cast<const Bar*>(static_cast<const Foo*>(move_assignment))->b == 200);
		}
	}
}
