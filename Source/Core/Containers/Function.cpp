/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Containers/Function.hpp>

#include <Core/Debug/Test.hpp>

#if MACH_ENABLE_TEST
MACH_TEST_SUITE("Containers") {
	using namespace Mach::Core;

	int test(int x) { return x + 7; }

	class Functor {
	public:
		int operator()(int x) const { return x + 1; }
	};

	struct Foo {
		int x = 0;

		int bar(int y) const { return x + y; }
	};

	MACH_TEST_CASE("Function") {
		MACH_SUBCASE("from lambda") {
			int foo = 5;
			Function<int(int)> fn = [foo](int x) { return x + foo; };
			MACH_CHECK(fn(5) == 10);
		}

		MACH_SUBCASE("from functor") {
			Function<int(int)> fn = Functor{};
			MACH_CHECK(fn(5) == 6);
		};

		MACH_SUBCASE("from function pointer") {
			Function<int(int)> fn = &test;
			MACH_CHECK(fn(5) == 12);
		}
	}

	MACH_TEST_CASE("FunctionRef") {
		MACH_SUBCASE("from lambda") {
			FunctionRef<int(int)> fn = [](int x) { return x + 1; };
			MACH_CHECK(fn(5) == 6);
		}

		MACH_SUBCASE("from functor") {
			FunctionRef<int(int)> fn = Functor{};
			MACH_CHECK(fn(5) == 6);
		};

		MACH_SUBCASE("from function pointer") {
			FunctionRef<int(int)> fn = &test;
			MACH_CHECK(fn(5) == 12);
		}
	}
}
#endif // MACH_ENABLE_TEST
