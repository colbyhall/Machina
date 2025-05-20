/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Atomic.hpp>
#include <Core/Debug/Test.hpp>

#if FORGE_ENABLE_TEST
FORGE_TEST_SUITE("Core") {
	using namespace Forge::Core;

	FORGE_TEST_CASE("Atomic") {
		FORGE_SUBCASE("default constructor") {
			Atomic<int> atomic;
			FORGE_CHECK(atomic.load() == 0);
		}

		FORGE_SUBCASE("value constructor") {
			Atomic<int> atomic{ 5 };
			FORGE_CHECK(atomic.load() == 5);
		}

		FORGE_SUBCASE("copy constructor") {
			Atomic<int> atomic{ 5 };
			Atomic<int> copy{ atomic };
			FORGE_CHECK(copy.load() == 5);
		}

		FORGE_SUBCASE("copy assignment") {
			Atomic<int> atomic{ 5 };
			Atomic<int> copy;
			copy = atomic;
			FORGE_CHECK(copy.load() == 5);
		}

		FORGE_SUBCASE("move constructor") {
			Atomic<int> atomic{ 5 };
			Atomic<int> move{ Forge::move(atomic) };
			FORGE_CHECK(move.load() == 5);
			FORGE_CHECK(atomic.load() == 5);
		}

		FORGE_SUBCASE("move assignment") {
			Atomic<int> atomic{ 5 };
			Atomic<int> move;
			move = Forge::move(atomic);
			FORGE_CHECK(move.load() == 5);
			FORGE_CHECK(atomic.load() == 5);
		}

		FORGE_SUBCASE("load") {
			Atomic<int> atomic{ 5 };
			FORGE_CHECK(atomic.load() == 5);
		}

		FORGE_SUBCASE("store") {
			Atomic<int> atomic;
			atomic.store(5);
			FORGE_CHECK(atomic.load() == 5);
		}

		FORGE_SUBCASE("exchange") {
			Atomic<int> atomic{ 5 };
			FORGE_CHECK(atomic.exchange(10) == 5);
			FORGE_CHECK(atomic.load() == 10);
		}

		FORGE_SUBCASE("compare_exchange_weak") {
			Atomic<int> atomic{ 5 };
			FORGE_CHECK(atomic.compare_exchange_weak(5, 10).unwrap() == 5);
			FORGE_CHECK(atomic.load() == 10);
		}

		FORGE_SUBCASE("compare_exchange_strong") {
			Atomic<int> atomic{ 5 };
			FORGE_CHECK(atomic.compare_exchange_strong(5, 10).unwrap() == 5);
			FORGE_CHECK(atomic.load() == 10);
		}

		FORGE_SUBCASE("fetch_add") {
			Atomic<int> atomic{ 5 };
			FORGE_CHECK(atomic.fetch_add(5) == 5);
			FORGE_CHECK(atomic.load() == 10);
		}

		FORGE_SUBCASE("fetch_sub") {
			Atomic<int> atomic{ 5 };
			FORGE_CHECK(atomic.fetch_sub(5) == 5);
			FORGE_CHECK(atomic.load() == 0);
		}

		FORGE_SUBCASE("fetch_and") {
			Atomic<int> atomic{ 5 };
			FORGE_CHECK(atomic.fetch_and(3) == 5);
			FORGE_CHECK(atomic.load() == 1);
		}

		FORGE_SUBCASE("fetch_or") {
			Atomic<int> atomic{ 5 };
			FORGE_CHECK(atomic.fetch_or(3) == 5);
			FORGE_CHECK(atomic.load() == 7);
		}

		FORGE_SUBCASE("fetch_xor") {
			Atomic<int> atomic{ 5 };
			FORGE_CHECK(atomic.fetch_xor(3) == 5);
			FORGE_CHECK(atomic.load() == 6);
		}
	}
}
#endif // FORGE_ENABLE_TEST
