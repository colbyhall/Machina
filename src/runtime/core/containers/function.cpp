/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include "core/containers/function.h"
#include "core/containers/unique.h"
#include "core/debug/test.h"

#if OP_ENABLE_TEST
OP_TEST_SUITE("containers") {
	using namespace op::core;

	int test(int x) { return x + 7; }

	class Functor {
	public:
		int operator()(int x) const { return x + 1; }
	};

	struct Foo {
		int x = 0;

		int bar(int y) const { return x + y; }
	};

	OP_TEST_CASE("Function") {
		OP_SUBCASE("from lambda") {
			int foo = 5;
			Function<int(int)> fn = [foo](int x) { return x + foo; };
			OP_CHECK(fn(5) == 10);
		}

		OP_SUBCASE("from functor") {
			Function<int(int)> fn = Functor{};
			OP_CHECK(fn(5) == 6);
		};

		OP_SUBCASE("from function pointer") {
			Function<int(int)> fn = &test;
			OP_CHECK(fn(5) == 12);
		}
	}

	OP_TEST_CASE("FunctionRef") {
		OP_SUBCASE("from lambda") {
			FunctionRef<int(int)> fn = [](int x) { return x + 1; };
			OP_CHECK(fn(5) == 6);
		}

		OP_SUBCASE("from functor") {
			FunctionRef<int(int)> fn = Functor{};
			OP_CHECK(fn(5) == 6);
		};

		OP_SUBCASE("from function pointer") {
			FunctionRef<int(int)> fn = &test;
			OP_CHECK(fn(5) == 12);
		}
	}
}
#endif // OP_ENABLE_TEST
