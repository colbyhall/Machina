/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <core/containers/non_null.h>
#include <core/debug/test.h>

#if GRIZZLY_ENABLE_TEST
GRIZZLY_TEST_SUITE("containers") {
	using namespace grizzly::core;

	GRIZZLY_TEST_CASE("NonNull") {
		GRIZZLY_SUBCASE("constructor") {
			int value = 5;
			const NonNull<int> ptr = &value;
			GRIZZLY_CHECK(ptr == &value);
		}

		struct Foo {
			int a = 0;
		};

		GRIZZLY_SUBCASE("operator->") {
			Foo value{ .a = 120 };
			const NonNull<Foo> ptr = &value;
			GRIZZLY_CHECK(ptr->a == 120);
		}

		GRIZZLY_SUBCASE("operator*") {
			Foo value{ .a = 120 };
			const NonNull<Foo> ptr = &value;
			GRIZZLY_CHECK((*ptr).a == 120);
		}

		GRIZZLY_SUBCASE("operator[]") {
			Foo value[] = { Foo{ .a = 120 }, Foo{ .a = 121 } };
			const NonNull<Foo> ptr = value;
			GRIZZLY_CHECK(ptr[0].a == 120);
			GRIZZLY_CHECK(ptr[1].a == 121);
		}

		GRIZZLY_SUBCASE("operator==") {
			int value = 5;
			const NonNull<int> ptr = &value;
			GRIZZLY_CHECK(ptr == &value);
		}

		GRIZZLY_SUBCASE("operator!=") {
			int value = 5;
			const NonNull<int> ptr = &value;
			GRIZZLY_CHECK(ptr != nullptr);
		}
	}

	GRIZZLY_TEST_CASE("NonNull<void>") {
		GRIZZLY_SUBCASE("constructor") {
			int value = 5;
			const NonNull<void> ptr = &value;
			GRIZZLY_CHECK(ptr == &value);
		}

		GRIZZLY_SUBCASE("operator void*") {
			int value = 5;
			const NonNull<void> ptr = &value;
			GRIZZLY_CHECK(static_cast<void*>(ptr) == &value);
		}

		GRIZZLY_SUBCASE("operator*") {
			int value = 5;
			const NonNull<void> ptr = &value;
			GRIZZLY_CHECK(*ptr == &value);
		}

		GRIZZLY_SUBCASE("operator==") {
			int value = 5;
			const NonNull<void> ptr = &value;
			GRIZZLY_CHECK(ptr == &value);
		}

		GRIZZLY_SUBCASE("operator!=") {
			int value = 5;
			const NonNull<void> ptr = &value;
			GRIZZLY_CHECK(ptr != nullptr);
		}

		GRIZZLY_SUBCASE("as") {
			int value = 5;
			const NonNull<void> ptr = &value;
			GRIZZLY_CHECK(ptr.as<int>() == &value);
		}
	}

	GRIZZLY_TEST_CASE("NonNull<void const>") {
		GRIZZLY_SUBCASE("constructor") {
			int value = 5;
			const NonNull<void const> ptr = &value;
			GRIZZLY_CHECK(ptr == &value);
		}

		GRIZZLY_SUBCASE("operator void const*") {
			int value = 5;
			const NonNull<void const> ptr = &value;
			GRIZZLY_CHECK(static_cast<void const*>(ptr) == &value);
		}

		GRIZZLY_SUBCASE("operator*") {
			int value = 5;
			const NonNull<void const> ptr = &value;
			GRIZZLY_CHECK(*ptr == &value);
		}

		GRIZZLY_SUBCASE("operator==") {
			int value = 5;
			const NonNull<void const> ptr = &value;
			GRIZZLY_CHECK(ptr == &value);
		}

		GRIZZLY_SUBCASE("operator!=") {
			int value = 5;
			const NonNull<void const> ptr = &value;
			GRIZZLY_CHECK(ptr != nullptr);
		}

		GRIZZLY_SUBCASE("as") {
			int value = 5;
			const NonNull<void const> ptr = &value;
			GRIZZLY_CHECK(ptr.as<const int>() == &value);
		}
	}
}
#endif // GRIZZLY_ENABLE_TEST
