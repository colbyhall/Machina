/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Atomic.hpp>
#include <Core/Debug/Test.hpp>

#if MACH_ENABLE_TEST
MACH_TEST_SUITE("Core") {
	using namespace Mach::Core;

	MACH_TEST_CASE("Atomic") {
		MACH_SUBCASE("default constructor") {
			Atomic<int> atomic;
			MACH_CHECK(atomic.load() == 0);
		}

		MACH_SUBCASE("value constructor") {
			Atomic<int> atomic{ 5 };
			MACH_CHECK(atomic.load() == 5);
		}

		MACH_SUBCASE("copy constructor") {
			Atomic<int> atomic{ 5 };
			Atomic<int> copy{ atomic };
			MACH_CHECK(copy.load() == 5);
		}

		MACH_SUBCASE("copy assignment") {
			Atomic<int> atomic{ 5 };
			Atomic<int> copy;
			copy = atomic;
			MACH_CHECK(copy.load() == 5);
		}

		MACH_SUBCASE("move constructor") {
			Atomic<int> atomic{ 5 };
			Atomic<int> move{ Mach::move(atomic) };
			MACH_CHECK(move.load() == 5);
			MACH_CHECK(atomic.load() == 5);
		}

		MACH_SUBCASE("move assignment") {
			Atomic<int> atomic{ 5 };
			Atomic<int> move;
			move = Mach::move(atomic);
			MACH_CHECK(move.load() == 5);
			MACH_CHECK(atomic.load() == 5);
		}

		MACH_SUBCASE("load") {
			Atomic<int> atomic{ 5 };
			MACH_CHECK(atomic.load() == 5);
		}

		MACH_SUBCASE("store") {
			Atomic<int> atomic;
			atomic.store(5);
			MACH_CHECK(atomic.load() == 5);
		}

		MACH_SUBCASE("exchange") {
			Atomic<int> atomic{ 5 };
			MACH_CHECK(atomic.exchange(10) == 5);
			MACH_CHECK(atomic.load() == 10);
		}

		MACH_SUBCASE("compare_exchange_weak") {
			Atomic<int> atomic{ 5 };
			MACH_CHECK(atomic.compare_exchange_weak(5, 10).unwrap() == 5);
			MACH_CHECK(atomic.load() == 10);
		}

		MACH_SUBCASE("compare_exchange_strong") {
			Atomic<int> atomic{ 5 };
			MACH_CHECK(atomic.compare_exchange_strong(5, 10).unwrap() == 5);
			MACH_CHECK(atomic.load() == 10);
		}

		MACH_SUBCASE("fetch_add") {
			Atomic<int> atomic{ 5 };
			MACH_CHECK(atomic.fetch_add(5) == 5);
			MACH_CHECK(atomic.load() == 10);
		}

		MACH_SUBCASE("fetch_sub") {
			Atomic<int> atomic{ 5 };
			MACH_CHECK(atomic.fetch_sub(5) == 5);
			MACH_CHECK(atomic.load() == 0);
		}

		MACH_SUBCASE("fetch_and") {
			Atomic<int> atomic{ 5 };
			MACH_CHECK(atomic.fetch_and(3) == 5);
			MACH_CHECK(atomic.load() == 1);
		}

		MACH_SUBCASE("fetch_or") {
			Atomic<int> atomic{ 5 };
			MACH_CHECK(atomic.fetch_or(3) == 5);
			MACH_CHECK(atomic.load() == 7);
		}

		MACH_SUBCASE("fetch_xor") {
			Atomic<int> atomic{ 5 };
			MACH_CHECK(atomic.fetch_xor(3) == 5);
			MACH_CHECK(atomic.load() == 6);
		}
	}
}
#endif // MACH_ENABLE_TEST
