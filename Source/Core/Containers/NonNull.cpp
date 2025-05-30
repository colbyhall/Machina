/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Containers/NonNull.hpp>
#include <Core/Debug/Test.hpp>

#if MACH_ENABLE_TEST
MACH_TEST_SUITE("Containers") {
	using namespace Mach::Core;

	MACH_TEST_CASE("NonNull") {
		MACH_SUBCASE("constructor") {
			int value = 5;
			const NonNull<int> ptr = &value;
			MACH_CHECK(ptr == &value);
		}

		struct Foo {
			int a = 0;
		};

		MACH_SUBCASE("operator->") {
			Foo value{ .a = 120 };
			const NonNull<Foo> ptr = &value;
			MACH_CHECK(ptr->a == 120);
		}

		MACH_SUBCASE("operator*") {
			Foo value{ .a = 120 };
			const NonNull<Foo> ptr = &value;
			MACH_CHECK((*ptr).a == 120);
		}

		MACH_SUBCASE("operator[]") {
			Foo value[] = { Foo{ .a = 120 }, Foo{ .a = 121 } };
			const NonNull<Foo> ptr = value;
			MACH_CHECK(ptr[0].a == 120);
			MACH_CHECK(ptr[1].a == 121);
		}

		MACH_SUBCASE("operator==") {
			int value = 5;
			const NonNull<int> ptr = &value;
			MACH_CHECK(ptr == &value);
		}

		MACH_SUBCASE("operator!=") {
			int value = 5;
			const NonNull<int> ptr = &value;
			MACH_CHECK(ptr != nullptr);
		}
	}

	MACH_TEST_CASE("NonNull<void>") {
		MACH_SUBCASE("constructor") {
			int value = 5;
			const NonNull<void> ptr = &value;
			MACH_CHECK(ptr == &value);
		}

		MACH_SUBCASE("operator void*") {
			int value = 5;
			const NonNull<void> ptr = &value;
			MACH_CHECK(static_cast<void*>(ptr) == &value);
		}

		MACH_SUBCASE("operator*") {
			int value = 5;
			const NonNull<void> ptr = &value;
			MACH_CHECK(*ptr == &value);
		}

		MACH_SUBCASE("operator==") {
			int value = 5;
			const NonNull<void> ptr = &value;
			MACH_CHECK(ptr == &value);
		}

		MACH_SUBCASE("operator!=") {
			int value = 5;
			const NonNull<void> ptr = &value;
			MACH_CHECK(ptr != nullptr);
		}

		MACH_SUBCASE("as") {
			int value = 5;
			const NonNull<void> ptr = &value;
			MACH_CHECK(ptr.as<int>() == &value);
		}
	}

	MACH_TEST_CASE("NonNull<void const>") {
		MACH_SUBCASE("constructor") {
			int value = 5;
			const NonNull<void const> ptr = &value;
			MACH_CHECK(ptr == &value);
		}

		MACH_SUBCASE("operator void const*") {
			int value = 5;
			const NonNull<void const> ptr = &value;
			MACH_CHECK(static_cast<void const*>(ptr) == &value);
		}

		MACH_SUBCASE("operator*") {
			int value = 5;
			const NonNull<void const> ptr = &value;
			MACH_CHECK(*ptr == &value);
		}

		MACH_SUBCASE("operator==") {
			int value = 5;
			const NonNull<void const> ptr = &value;
			MACH_CHECK(ptr == &value);
		}

		MACH_SUBCASE("operator!=") {
			int value = 5;
			const NonNull<void const> ptr = &value;
			MACH_CHECK(ptr != nullptr);
		}

		MACH_SUBCASE("as") {
			int value = 5;
			const NonNull<void const> ptr = &value;
			MACH_CHECK(ptr.as<const int>() == &value);
		}
	}
}
#endif // MACH_ENABLE_TEST
