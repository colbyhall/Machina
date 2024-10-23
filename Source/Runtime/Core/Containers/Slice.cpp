/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Containers/Slice.h>
#include <Core/Debug/Test.h>

#if GRIZZLY_ENABLE_TEST
GRIZZLY_TEST_SUITE("Containers") {
	using namespace Grizzly;

	GRIZZLY_TEST_CASE("Slice") {
		GRIZZLY_SUBCASE("default constructor") {
			Slice<int> slice;
			GRIZZLY_CHECK(slice.len() == 0);
		}

		GRIZZLY_SUBCASE("value constructor") {
			int arr[] = { 1, 2, 3, 4, 5 };
			Slice<int> slice{ arr, 5 };
			GRIZZLY_CHECK(slice.len() == 5);
			GRIZZLY_CHECK(slice[0] == 1);
			GRIZZLY_CHECK(slice[1] == 2);
			GRIZZLY_CHECK(slice[2] == 3);
			GRIZZLY_CHECK(slice[3] == 4);
			GRIZZLY_CHECK(slice[4] == 5);
		}

		GRIZZLY_SUBCASE("initializer_list") {
			Slice<int const> slice = { 1, 2, 3, 4, 5 };
			GRIZZLY_CHECK(slice.len() == 5);
			GRIZZLY_CHECK(slice[0] == 1);
			GRIZZLY_CHECK(slice[1] == 2);
			GRIZZLY_CHECK(slice[2] == 3);
			GRIZZLY_CHECK(slice[3] == 4);
			GRIZZLY_CHECK(slice[4] == 5);
		}

		GRIZZLY_SUBCASE("copy constructor") {
			Slice<int const> slice = { 1, 2, 3, 4, 5 };
			Slice<int const> copy = slice;
			GRIZZLY_CHECK(copy.len() == 5);
			GRIZZLY_CHECK(copy[0] == 1);
			GRIZZLY_CHECK(copy[1] == 2);
			GRIZZLY_CHECK(copy[2] == 3);
			GRIZZLY_CHECK(copy[3] == 4);
			GRIZZLY_CHECK(copy[4] == 5);
		}

		GRIZZLY_SUBCASE("copy assignment") {
			Slice<int const> slice = { 1, 2, 3, 4, 5 };
			Slice<int const> copy;
			GRIZZLY_CHECK(copy.is_empty());
			copy = slice;
			GRIZZLY_CHECK(copy.len() == 5);
			GRIZZLY_CHECK(copy[0] == 1);
			GRIZZLY_CHECK(copy[1] == 2);
			GRIZZLY_CHECK(copy[2] == 3);
			GRIZZLY_CHECK(copy[3] == 4);
			GRIZZLY_CHECK(copy[4] == 5);
		}

		GRIZZLY_SUBCASE("move constructor") {
			Slice<int const> slice = { 1, 2, 3, 4, 5 };
			Slice<int const> move = Grizzly::move(slice);
			GRIZZLY_CHECK(move.len() == 5);
			GRIZZLY_CHECK(move[0] == 1);
			GRIZZLY_CHECK(move[1] == 2);
			GRIZZLY_CHECK(move[2] == 3);
			GRIZZLY_CHECK(move[3] == 4);
			GRIZZLY_CHECK(move[4] == 5);
			GRIZZLY_CHECK(slice.is_empty());
		}

		GRIZZLY_SUBCASE("move assignment") {
			Slice<int const> slice = { 1, 2, 3, 4, 5 };
			Slice<int const> move;
			GRIZZLY_CHECK(move.is_empty());
			move = Grizzly::move(slice);
			GRIZZLY_CHECK(move.len() == 5);
			GRIZZLY_CHECK(move[0] == 1);
			GRIZZLY_CHECK(move[1] == 2);
			GRIZZLY_CHECK(move[2] == 3);
			GRIZZLY_CHECK(move[3] == 4);
			GRIZZLY_CHECK(move[4] == 5);
			GRIZZLY_CHECK(slice.is_empty());
		}

		GRIZZLY_SUBCASE("shrink") {
			Slice<int const> slice = { 1, 2, 3, 4, 5 };
			Slice<int const> shrunk = slice.shrink(2);
			GRIZZLY_CHECK(shrunk.len() == 3);
			GRIZZLY_CHECK(shrunk[0] == 1);
			GRIZZLY_CHECK(shrunk[1] == 2);
			GRIZZLY_CHECK(shrunk[2] == 3);
			GRIZZLY_CHECK(slice.len() == 5);
			GRIZZLY_CHECK(slice.begin() == shrunk.begin());
		}
	}
}
#endif // GRIZZLY_ENABLE_TEST
