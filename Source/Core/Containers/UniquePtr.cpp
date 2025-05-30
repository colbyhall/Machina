/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Containers/UniquePtr.hpp>
#include <Core/Debug/Test.hpp>

MACH_TEST_SUITE("Containers") {
	using namespace Mach::Core;

	MACH_TEST_CASE("Unique") {
		struct Foo {
			int a = 0;
		};

		MACH_SUBCASE("default constructor") {
			const UniquePtr<Foo> unique;
			MACH_CHECK(!unique.is_valid());
		}

		MACH_SUBCASE("create") {
			const auto unique = UniquePtr<Foo>::create(Foo{ .a = 100 });
			MACH_CHECK(unique->a == 100);
		}

		MACH_SUBCASE("copy constructor") {
			const auto unique = UniquePtr<Foo>::create(Foo{ .a = 100 });
			MACH_REQUIRE(unique->a == 100);
			const auto copy = unique;
			MACH_CHECK(copy->a == 100);
		}

		MACH_SUBCASE("copy assignment") {
			const auto unique = UniquePtr<Foo>::create(Foo{ .a = 100 });
			MACH_REQUIRE(unique->a == 100);
			UniquePtr<Foo> copy_assignment;
			MACH_CHECK(!copy_assignment.is_valid());
			copy_assignment = unique;
			MACH_CHECK(copy_assignment->a == 100);
		}

		MACH_SUBCASE("move constructor") {
			auto unique = UniquePtr<Foo>::create(Foo{ .a = 100 });
			MACH_REQUIRE(unique->a == 100);
			const auto move = Mach::move(unique);
			MACH_CHECK(move->a == 100);
			MACH_CHECK(static_cast<Foo*>(unique) == nullptr);
		}

		MACH_SUBCASE("move assignment") {
			auto unique = UniquePtr<Foo>::create(Foo{ .a = 100 });
			MACH_REQUIRE(unique->a == 100);
			UniquePtr<Foo> move_assignment;
			MACH_CHECK(!move_assignment.is_valid());
			move_assignment = Mach::move(unique);
			MACH_CHECK(move_assignment->a == 100);
			MACH_CHECK(static_cast<Foo*>(unique) == nullptr);
		}

		struct Bar : Foo {
			int b = 0;
		};

		MACH_SUBCASE("derived move constructor") {
			auto unique = UniquePtr<Bar>::create(Bar{ .b = 200 });
			MACH_REQUIRE(unique->a == 0);
			MACH_REQUIRE(unique->b == 200);
			const UniquePtr<Foo> move = Mach::move(unique);
			MACH_CHECK(move->a == 0);
			MACH_CHECK(static_cast<Bar*>(unique) == nullptr);
			MACH_CHECK(static_cast<const Bar*>(static_cast<const Foo*>(move))->b == 200);
		}

		MACH_SUBCASE("derived move assignment") {
			auto unique = UniquePtr<Bar>::create(Bar{ .b = 200 });
			MACH_REQUIRE(unique->a == 0);
			MACH_REQUIRE(unique->b == 200);
			UniquePtr<Foo> move_assignment;
			MACH_CHECK(!move_assignment.is_valid());
			move_assignment = Mach::move(unique);
			MACH_CHECK(move_assignment->a == 0);
			MACH_CHECK(static_cast<Bar*>(unique) == nullptr);
			MACH_CHECK(static_cast<const Bar*>(static_cast<const Foo*>(move_assignment))->b == 200);
		}
	}
}
