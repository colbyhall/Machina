/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Containers/Unique.hpp>
#include <Core/Debug/Test.hpp>

FORGE_TEST_SUITE("Containers") {
	using namespace Forge::Core;

	FORGE_TEST_CASE("Unique") {
		struct Foo {
			int a = 0;
		};

		FORGE_SUBCASE("default constructor") {
			const Unique<Foo> unique;
			FORGE_CHECK(unique->a == 0);
		}

		FORGE_SUBCASE("create") {
			const auto unique = Unique<Foo>::create(Foo{ .a = 100 });
			FORGE_CHECK(unique->a == 100);
		}

		FORGE_SUBCASE("copy constructor") {
			const auto unique = Unique<Foo>::create(Foo{ .a = 100 });
			FORGE_REQUIRE(unique->a == 100);
			const auto copy = unique;
			FORGE_CHECK(copy->a == 100);
		}

		FORGE_SUBCASE("copy assignment") {
			const auto unique = Unique<Foo>::create(Foo{ .a = 100 });
			FORGE_REQUIRE(unique->a == 100);
			Unique<Foo> copy_assignment;
			FORGE_CHECK(copy_assignment->a == 0);
			copy_assignment = unique;
			FORGE_CHECK(copy_assignment->a == 100);
		}

		FORGE_SUBCASE("move constructor") {
			auto unique = Unique<Foo>::create(Foo{ .a = 100 });
			FORGE_REQUIRE(unique->a == 100);
			const auto move = Forge::move(unique);
			FORGE_CHECK(move->a == 100);
			FORGE_CHECK(static_cast<Foo*>(unique) == nullptr);
		}

		FORGE_SUBCASE("move assignment") {
			auto unique = Unique<Foo>::create(Foo{ .a = 100 });
			FORGE_REQUIRE(unique->a == 100);
			Unique<Foo> move_assignment;
			FORGE_CHECK(move_assignment->a == 0);
			move_assignment = Forge::move(unique);
			FORGE_CHECK(move_assignment->a == 100);
			FORGE_CHECK(static_cast<Foo*>(unique) == nullptr);
		}

		struct Bar : Foo {
			int b = 0;
		};

		FORGE_SUBCASE("derived move constructor") {
			auto unique = Unique<Bar>::create(Bar{ .b = 200 });
			FORGE_REQUIRE(unique->a == 0);
			FORGE_REQUIRE(unique->b == 200);
			const Unique<Foo> move = Forge::move(unique);
			FORGE_CHECK(move->a == 0);
			FORGE_CHECK(static_cast<Bar*>(unique) == nullptr);
			FORGE_CHECK(static_cast<const Bar*>(static_cast<const Foo*>(move))->b == 200);
		}

		FORGE_SUBCASE("derived move assignment") {
			auto unique = Unique<Bar>::create(Bar{ .b = 200 });
			FORGE_REQUIRE(unique->a == 0);
			FORGE_REQUIRE(unique->b == 200);
			Unique<Foo> move_assignment;
			FORGE_CHECK(move_assignment->a == 0);
			move_assignment = Forge::move(unique);
			FORGE_CHECK(move_assignment->a == 0);
			FORGE_CHECK(static_cast<Bar*>(unique) == nullptr);
			FORGE_CHECK(static_cast<const Bar*>(static_cast<const Foo*>(move_assignment))->b == 200);
		}
	}
}
