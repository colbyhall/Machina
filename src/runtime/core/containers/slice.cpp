/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include "core/containers/slice.h"
#include "core/debug/test.h"

#if OP_ENABLE_TEST
OP_TEST_SUITE("containers") {
	using namespace op;

	OP_TEST_CASE("Slice") {
		OP_SUBCASE("default constructor") {
			Slice<int> slice;
			OP_CHECK(slice.len() == 0);
		}

		OP_SUBCASE("value constructor") {
			int arr[] = { 1, 2, 3, 4, 5 };
			Slice<int> slice{ arr, 5 };
			OP_CHECK(slice.len() == 5);
			OP_CHECK(slice[0] == 1);
			OP_CHECK(slice[1] == 2);
			OP_CHECK(slice[2] == 3);
			OP_CHECK(slice[3] == 4);
			OP_CHECK(slice[4] == 5);
		}

		OP_SUBCASE("initializer_list") {
			Slice<int const> slice = { 1, 2, 3, 4, 5 };
			OP_CHECK(slice.len() == 5);
			OP_CHECK(slice[0] == 1);
			OP_CHECK(slice[1] == 2);
			OP_CHECK(slice[2] == 3);
			OP_CHECK(slice[3] == 4);
			OP_CHECK(slice[4] == 5);
		}

		OP_SUBCASE("copy constructor") {
			Slice<int const> slice = { 1, 2, 3, 4, 5 };
			Slice<int const> copy = slice;
			OP_CHECK(copy.len() == 5);
			OP_CHECK(copy[0] == 1);
			OP_CHECK(copy[1] == 2);
			OP_CHECK(copy[2] == 3);
			OP_CHECK(copy[3] == 4);
			OP_CHECK(copy[4] == 5);
		}

		OP_SUBCASE("copy assignment") {
			Slice<int const> slice = { 1, 2, 3, 4, 5 };
			Slice<int const> copy;
			OP_CHECK(copy.is_empty());
			copy = slice;
			OP_CHECK(copy.len() == 5);
			OP_CHECK(copy[0] == 1);
			OP_CHECK(copy[1] == 2);
			OP_CHECK(copy[2] == 3);
			OP_CHECK(copy[3] == 4);
			OP_CHECK(copy[4] == 5);
		}

		OP_SUBCASE("move constructor") {
			Slice<int const> slice = { 1, 2, 3, 4, 5 };
			Slice<int const> move = op::move(slice);
			OP_CHECK(move.len() == 5);
			OP_CHECK(move[0] == 1);
			OP_CHECK(move[1] == 2);
			OP_CHECK(move[2] == 3);
			OP_CHECK(move[3] == 4);
			OP_CHECK(move[4] == 5);
			OP_CHECK(slice.is_empty());
		}

		OP_SUBCASE("move assignment") {
			Slice<int const> slice = { 1, 2, 3, 4, 5 };
			Slice<int const> move;
			OP_CHECK(move.is_empty());
			move = op::move(slice);
			OP_CHECK(move.len() == 5);
			OP_CHECK(move[0] == 1);
			OP_CHECK(move[1] == 2);
			OP_CHECK(move[2] == 3);
			OP_CHECK(move[3] == 4);
			OP_CHECK(move[4] == 5);
			OP_CHECK(slice.is_empty());
		}

		OP_SUBCASE("shrink") {
			Slice<int const> slice = { 1, 2, 3, 4, 5 };
			Slice<int const> shrunk = slice.shrink(2);
			OP_CHECK(shrunk.len() == 3);
			OP_CHECK(shrunk[0] == 1);
			OP_CHECK(shrunk[1] == 2);
			OP_CHECK(shrunk[2] == 3);
			OP_CHECK(slice.len() == 5);
			OP_CHECK(slice.begin() == shrunk.begin());
		}
	}
}
#endif // OP_ENABLE_TEST
