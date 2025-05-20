/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Containers/NonNull.hpp>
#include <Core/Debug/Test.hpp>

#if FORGE_ENABLE_TEST
FORGE_TEST_SUITE("Containers") {
	using namespace Forge::Core;

	FORGE_TEST_CASE("NonNull") {
		FORGE_SUBCASE("constructor") {
			int value = 5;
			const NonNull<int> ptr = &value;
			FORGE_CHECK(ptr == &value);
		}

		struct Foo {
			int a = 0;
		};

		FORGE_SUBCASE("operator->") {
			Foo value{ .a = 120 };
			const NonNull<Foo> ptr = &value;
			FORGE_CHECK(ptr->a == 120);
		}

		FORGE_SUBCASE("operator*") {
			Foo value{ .a = 120 };
			const NonNull<Foo> ptr = &value;
			FORGE_CHECK((*ptr).a == 120);
		}

		FORGE_SUBCASE("operator[]") {
			Foo value[] = { Foo{ .a = 120 }, Foo{ .a = 121 } };
			const NonNull<Foo> ptr = value;
			FORGE_CHECK(ptr[0].a == 120);
			FORGE_CHECK(ptr[1].a == 121);
		}

		FORGE_SUBCASE("operator==") {
			int value = 5;
			const NonNull<int> ptr = &value;
			FORGE_CHECK(ptr == &value);
		}

		FORGE_SUBCASE("operator!=") {
			int value = 5;
			const NonNull<int> ptr = &value;
			FORGE_CHECK(ptr != nullptr);
		}
	}

	FORGE_TEST_CASE("NonNull<void>") {
		FORGE_SUBCASE("constructor") {
			int value = 5;
			const NonNull<void> ptr = &value;
			FORGE_CHECK(ptr == &value);
		}

		FORGE_SUBCASE("operator void*") {
			int value = 5;
			const NonNull<void> ptr = &value;
			FORGE_CHECK(static_cast<void*>(ptr) == &value);
		}

		FORGE_SUBCASE("operator*") {
			int value = 5;
			const NonNull<void> ptr = &value;
			FORGE_CHECK(*ptr == &value);
		}

		FORGE_SUBCASE("operator==") {
			int value = 5;
			const NonNull<void> ptr = &value;
			FORGE_CHECK(ptr == &value);
		}

		FORGE_SUBCASE("operator!=") {
			int value = 5;
			const NonNull<void> ptr = &value;
			FORGE_CHECK(ptr != nullptr);
		}

		FORGE_SUBCASE("as") {
			int value = 5;
			const NonNull<void> ptr = &value;
			FORGE_CHECK(ptr.as<int>() == &value);
		}
	}

	FORGE_TEST_CASE("NonNull<void const>") {
		FORGE_SUBCASE("constructor") {
			int value = 5;
			const NonNull<void const> ptr = &value;
			FORGE_CHECK(ptr == &value);
		}

		FORGE_SUBCASE("operator void const*") {
			int value = 5;
			const NonNull<void const> ptr = &value;
			FORGE_CHECK(static_cast<void const*>(ptr) == &value);
		}

		FORGE_SUBCASE("operator*") {
			int value = 5;
			const NonNull<void const> ptr = &value;
			FORGE_CHECK(*ptr == &value);
		}

		FORGE_SUBCASE("operator==") {
			int value = 5;
			const NonNull<void const> ptr = &value;
			FORGE_CHECK(ptr == &value);
		}

		FORGE_SUBCASE("operator!=") {
			int value = 5;
			const NonNull<void const> ptr = &value;
			FORGE_CHECK(ptr != nullptr);
		}

		FORGE_SUBCASE("as") {
			int value = 5;
			const NonNull<void const> ptr = &value;
			FORGE_CHECK(ptr.as<const int>() == &value);
		}
	}
}
#endif // FORGE_ENABLE_TEST
