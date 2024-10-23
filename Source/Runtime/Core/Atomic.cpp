/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Atomic.h>
#include <Core/Debug/Test.h>

#if GRIZZLY_ENABLE_TEST
GRIZZLY_TEST_SUITE("Core") {
	using namespace Grizzly::Core;

	GRIZZLY_TEST_CASE("Atomic") {
		GRIZZLY_SUBCASE("default constructor") {
			Atomic<int> atomic;
			GRIZZLY_CHECK(atomic.load() == 0);
		}

		GRIZZLY_SUBCASE("value constructor") {
			Atomic<int> atomic{ 5 };
			GRIZZLY_CHECK(atomic.load() == 5);
		}

		GRIZZLY_SUBCASE("copy constructor") {
			Atomic<int> atomic{ 5 };
			Atomic<int> copy{ atomic };
			GRIZZLY_CHECK(copy.load() == 5);
		}

		GRIZZLY_SUBCASE("copy assignment") {
			Atomic<int> atomic{ 5 };
			Atomic<int> copy;
			copy = atomic;
			GRIZZLY_CHECK(copy.load() == 5);
		}

		GRIZZLY_SUBCASE("move constructor") {
			Atomic<int> atomic{ 5 };
			Atomic<int> move{ Grizzly::move(atomic) };
			GRIZZLY_CHECK(move.load() == 5);
			GRIZZLY_CHECK(atomic.load() == 5);
		}

		GRIZZLY_SUBCASE("move assignment") {
			Atomic<int> atomic{ 5 };
			Atomic<int> move;
			move = Grizzly::move(atomic);
			GRIZZLY_CHECK(move.load() == 5);
			GRIZZLY_CHECK(atomic.load() == 5);
		}

		GRIZZLY_SUBCASE("load") {
			Atomic<int> atomic{ 5 };
			GRIZZLY_CHECK(atomic.load() == 5);
		}

		GRIZZLY_SUBCASE("store") {
			Atomic<int> atomic;
			atomic.store(5);
			GRIZZLY_CHECK(atomic.load() == 5);
		}

		GRIZZLY_SUBCASE("exchange") {
			Atomic<int> atomic{ 5 };
			GRIZZLY_CHECK(atomic.exchange(10) == 5);
			GRIZZLY_CHECK(atomic.load() == 10);
		}

		GRIZZLY_SUBCASE("compare_exchange_weak") {
			Atomic<int> atomic{ 5 };
			GRIZZLY_CHECK(atomic.compare_exchange_weak(5, 10).unwrap() == 5);
			GRIZZLY_CHECK(atomic.load() == 10);
		}

		GRIZZLY_SUBCASE("compare_exchange_strong") {
			Atomic<int> atomic{ 5 };
			GRIZZLY_CHECK(atomic.compare_exchange_strong(5, 10).unwrap() == 5);
			GRIZZLY_CHECK(atomic.load() == 10);
		}

		GRIZZLY_SUBCASE("fetch_add") {
			Atomic<int> atomic{ 5 };
			GRIZZLY_CHECK(atomic.fetch_add(5) == 5);
			GRIZZLY_CHECK(atomic.load() == 10);
		}

		GRIZZLY_SUBCASE("fetch_sub") {
			Atomic<int> atomic{ 5 };
			GRIZZLY_CHECK(atomic.fetch_sub(5) == 5);
			GRIZZLY_CHECK(atomic.load() == 0);
		}

		GRIZZLY_SUBCASE("fetch_and") {
			Atomic<int> atomic{ 5 };
			GRIZZLY_CHECK(atomic.fetch_and(3) == 5);
			GRIZZLY_CHECK(atomic.load() == 1);
		}

		GRIZZLY_SUBCASE("fetch_or") {
			Atomic<int> atomic{ 5 };
			GRIZZLY_CHECK(atomic.fetch_or(3) == 5);
			GRIZZLY_CHECK(atomic.load() == 7);
		}

		GRIZZLY_SUBCASE("fetch_xor") {
			Atomic<int> atomic{ 5 };
			GRIZZLY_CHECK(atomic.fetch_xor(3) == 5);
			GRIZZLY_CHECK(atomic.load() == 6);
		}
	}
}
#endif // GRIZZLY_ENABLE_TEST
