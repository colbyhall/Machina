/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include "core/containers/unique.h"
#include "core/test.h"

using namespace op::core;

OP_TEST_SUITE("containers") {
	OP_TEST_CASE("Unique") {
		struct Foo {
			int a = 0;
		};

		OP_SUBCASE("default constructor") {
			const Unique<Foo> unique;
			OP_CHECK(unique->a == 0);
		}

		OP_SUBCASE("create") {
			const auto unique = Unique<Foo>::create(Foo{ .a = 100 });
			OP_CHECK(unique->a == 100);
		}

		OP_SUBCASE("copy constructor") {
			const auto unique = Unique<Foo>::create(Foo{ .a = 100 });
			OP_REQUIRE(unique->a == 100);
			const auto copy = unique;
			OP_CHECK(copy->a == 100);
		}

		OP_SUBCASE("copy assignment") {
			const auto unique = Unique<Foo>::create(Foo{ .a = 100 });
			OP_REQUIRE(unique->a == 100);
			Unique<Foo> copy_assignment;
			OP_CHECK(copy_assignment->a == 0);
			copy_assignment = unique;
			OP_CHECK(copy_assignment->a == 100);
		}

		OP_SUBCASE("move constructor") {
			auto unique = Unique<Foo>::create(Foo{ .a = 100 });
			OP_REQUIRE(unique->a == 100);
			const auto move = op::move(unique);
			OP_CHECK(move->a == 100);
			OP_CHECK(static_cast<Foo*>(unique) == nullptr);
		}

		OP_SUBCASE("move assignment") {
			auto unique = Unique<Foo>::create(Foo{ .a = 100 });
			OP_REQUIRE(unique->a == 100);
			Unique<Foo> move_assignment;
			OP_CHECK(move_assignment->a == 0);
			move_assignment = op::move(unique);
			OP_CHECK(move_assignment->a == 100);
			OP_CHECK(static_cast<Foo*>(unique) == nullptr);
		}

		struct Bar : Foo {
			int b = 0;
		};

		OP_SUBCASE("derived move constructor") {
			auto unique = Unique<Bar>::create(Bar{ .b = 200 });
			OP_REQUIRE(unique->a == 0);
			OP_REQUIRE(unique->b == 200);
			const Unique<Foo> move = op::move(unique);
			OP_CHECK(move->a == 0);
			OP_CHECK(static_cast<Bar*>(unique) == nullptr);
			OP_CHECK(static_cast<const Bar*>(static_cast<const Foo*>(move))->b == 200);
		}

		OP_SUBCASE("derived move assignment") {
			auto unique = Unique<Bar>::create(Bar{ .b = 200 });
			OP_REQUIRE(unique->a == 0);
			OP_REQUIRE(unique->b == 200);
			Unique<Foo> move_assignment;
			OP_CHECK(move_assignment->a == 0);
			move_assignment = op::move(unique);
			OP_CHECK(move_assignment->a == 0);
			OP_CHECK(static_cast<Bar*>(unique) == nullptr);
			OP_CHECK(static_cast<const Bar*>(static_cast<const Foo*>(move_assignment))->b == 200);
		}
	}
}