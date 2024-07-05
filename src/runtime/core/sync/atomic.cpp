/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include "core/sync/atomic.h"
#include "core/debug/test.h"

using namespace op::core;

OP_TEST_SUITE("sync") {
	OP_TEST_CASE("Atomic") {
		OP_SUBCASE("default constructor") {
			Atomic<int> atomic;
			OP_CHECK(atomic.load() == 0);
		}

		OP_SUBCASE("value constructor") {
			Atomic<int> atomic{ 5 };
			OP_CHECK(atomic.load() == 5);
		}

		OP_SUBCASE("copy constructor") {
			Atomic<int> atomic{ 5 };
			Atomic<int> copy{ atomic };
			OP_CHECK(copy.load() == 5);
		}

		OP_SUBCASE("copy assignment") {
			Atomic<int> atomic{ 5 };
			Atomic<int> copy;
			copy = atomic;
			OP_CHECK(copy.load() == 5);
		}

		OP_SUBCASE("move constructor") {
			Atomic<int> atomic{ 5 };
			Atomic<int> move{ op::move(atomic) };
			OP_CHECK(move.load() == 5);
			OP_CHECK(atomic.load() == 5);
		}

		OP_SUBCASE("move assignment") {
			Atomic<int> atomic{ 5 };
			Atomic<int> move;
			move = op::move(atomic);
			OP_CHECK(move.load() == 5);
			OP_CHECK(atomic.load() == 5);
		}

		OP_SUBCASE("load") {
			Atomic<int> atomic{ 5 };
			OP_CHECK(atomic.load() == 5);
		}

		OP_SUBCASE("store") {
			Atomic<int> atomic;
			atomic.store(5);
			OP_CHECK(atomic.load() == 5);
		}

		OP_SUBCASE("exchange") {
			Atomic<int> atomic{ 5 };
			OP_CHECK(atomic.exchange(10) == 5);
			OP_CHECK(atomic.load() == 10);
		}

		OP_SUBCASE("compare_exchange_weak") {
			Atomic<int> atomic{ 5 };
			OP_CHECK(atomic.compare_exchange_weak(5, 10).unwrap() == 5);
			OP_CHECK(atomic.load() == 10);
		}

		OP_SUBCASE("compare_exchange_strong") {
			Atomic<int> atomic{ 5 };
			OP_CHECK(atomic.compare_exchange_strong(5, 10).unwrap() == 5);
			OP_CHECK(atomic.load() == 10);
		}

		OP_SUBCASE("fetch_add") {
			Atomic<int> atomic{ 5 };
			OP_CHECK(atomic.fetch_add(5) == 5);
			OP_CHECK(atomic.load() == 10);
		}

		OP_SUBCASE("fetch_sub") {
			Atomic<int> atomic{ 5 };
			OP_CHECK(atomic.fetch_sub(5) == 5);
			OP_CHECK(atomic.load() == 0);
		}

		OP_SUBCASE("fetch_and") {
			Atomic<int> atomic{ 5 };
			OP_CHECK(atomic.fetch_and(3) == 5);
			OP_CHECK(atomic.load() == 1);
		}

		OP_SUBCASE("fetch_or") {
			Atomic<int> atomic{ 5 };
			OP_CHECK(atomic.fetch_or(3) == 5);
			OP_CHECK(atomic.load() == 7);
		}

		OP_SUBCASE("fetch_xor") {
			Atomic<int> atomic{ 5 };
			OP_CHECK(atomic.fetch_xor(3) == 5);
			OP_CHECK(atomic.load() == 6);
		}
	}
}