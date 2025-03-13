/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Containers/Function.hpp>
#include <Core/Containers/Unique.hpp>
#include <Core/Debug/Test.hpp>

#if FORGE_ENABLE_TEST
FORGE_TEST_SUITE("Containers") {
	using namespace Forge::Core;

	int test(int x) { return x + 7; }

	class Functor {
	public:
		int operator()(int x) const { return x + 1; }
	};

	struct Foo {
		int x = 0;

		int bar(int y) const { return x + y; }
	};

	FORGE_TEST_CASE("Function") {
		FORGE_SUBCASE("from lambda") {
			int foo = 5;
			Function<int(int)> fn = [foo](int x) { return x + foo; };
			FORGE_CHECK(fn(5) == 10);
		}

		FORGE_SUBCASE("from functor") {
			Function<int(int)> fn = Functor{};
			FORGE_CHECK(fn(5) == 6);
		};

		FORGE_SUBCASE("from function pointer") {
			Function<int(int)> fn = &test;
			FORGE_CHECK(fn(5) == 12);
		}
	}

	FORGE_TEST_CASE("FunctionRef") {
		FORGE_SUBCASE("from lambda") {
			FunctionRef<int(int)> fn = [](int x) { return x + 1; };
			FORGE_CHECK(fn(5) == 6);
		}

		FORGE_SUBCASE("from functor") {
			FunctionRef<int(int)> fn = Functor{};
			FORGE_CHECK(fn(5) == 6);
		};

		FORGE_SUBCASE("from function pointer") {
			FunctionRef<int(int)> fn = &test;
			FORGE_CHECK(fn(5) == 12);
		}
	}
}
#endif // FORGE_ENABLE_TEST
