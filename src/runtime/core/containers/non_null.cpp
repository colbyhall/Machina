/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include "core/containers/non_null.h"
#include "core/debug/test.h"

#if OP_ENABLE_TEST
OP_TEST_SUITE("containers") {
	using namespace op::core;

	OP_TEST_CASE("NonNull") {
		OP_SUBCASE("constructor") {
			int value = 5;
			const NonNull<int> ptr = &value;
			OP_CHECK(ptr == &value);
		}

		struct Foo {
			int a = 0;
		};

		OP_SUBCASE("operator->") {
			Foo value{ .a = 120 };
			const NonNull<Foo> ptr = &value;
			OP_CHECK(ptr->a == 120);
		}

		OP_SUBCASE("operator*") {
			Foo value{ .a = 120 };
			const NonNull<Foo> ptr = &value;
			OP_CHECK((*ptr).a == 120);
		}

		OP_SUBCASE("operator[]") {
			Foo value[] = { Foo{ .a = 120 }, Foo{ .a = 121 } };
			const NonNull<Foo> ptr = value;
			OP_CHECK(ptr[0].a == 120);
			OP_CHECK(ptr[1].a == 121);
		}

		OP_SUBCASE("operator==") {
			int value = 5;
			const NonNull<int> ptr = &value;
			OP_CHECK(ptr == &value);
		}

		OP_SUBCASE("operator!=") {
			int value = 5;
			const NonNull<int> ptr = &value;
			OP_CHECK(ptr != nullptr);
		}
	}

	OP_TEST_CASE("NonNull<void>") {
		OP_SUBCASE("constructor") {
			int value = 5;
			const NonNull<void> ptr = &value;
			OP_CHECK(ptr == &value);
		}

		OP_SUBCASE("operator void*") {
			int value = 5;
			const NonNull<void> ptr = &value;
			OP_CHECK(static_cast<void*>(ptr) == &value);
		}

		OP_SUBCASE("operator*") {
			int value = 5;
			const NonNull<void> ptr = &value;
			OP_CHECK(*ptr == &value);
		}

		OP_SUBCASE("operator==") {
			int value = 5;
			const NonNull<void> ptr = &value;
			OP_CHECK(ptr == &value);
		}

		OP_SUBCASE("operator!=") {
			int value = 5;
			const NonNull<void> ptr = &value;
			OP_CHECK(ptr != nullptr);
		}

		OP_SUBCASE("as") {
			int value = 5;
			const NonNull<void> ptr = &value;
			OP_CHECK(ptr.as<int>() == &value);
		}
	}

	OP_TEST_CASE("NonNull<void const>") {
		OP_SUBCASE("constructor") {
			int value = 5;
			const NonNull<void const> ptr = &value;
			OP_CHECK(ptr == &value);
		}

		OP_SUBCASE("operator void const*") {
			int value = 5;
			const NonNull<void const> ptr = &value;
			OP_CHECK(static_cast<void const*>(ptr) == &value);
		}

		OP_SUBCASE("operator*") {
			int value = 5;
			const NonNull<void const> ptr = &value;
			OP_CHECK(*ptr == &value);
		}

		OP_SUBCASE("operator==") {
			int value = 5;
			const NonNull<void const> ptr = &value;
			OP_CHECK(ptr == &value);
		}

		OP_SUBCASE("operator!=") {
			int value = 5;
			const NonNull<void const> ptr = &value;
			OP_CHECK(ptr != nullptr);
		}

		OP_SUBCASE("as") {
			int value = 5;
			const NonNull<void const> ptr = &value;
			OP_CHECK(ptr.as<const int>() == &value);
		}
	}
}
#endif // OP_ENABLE_TEST
