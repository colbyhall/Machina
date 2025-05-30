/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Containers/Slice.hpp>
#include <Core/Debug/Test.hpp>

#if MACH_ENABLE_TEST
MACH_TEST_SUITE("Containers") {
	using namespace Mach;

	MACH_TEST_CASE("Slice") {
		MACH_SUBCASE("default constructor") {
			Slice<int> slice;
			MACH_CHECK(slice.len() == 0);
		}

		MACH_SUBCASE("value constructor") {
			int arr[] = { 1, 2, 3, 4, 5 };
			Slice<int> slice{ arr, 5 };
			MACH_CHECK(slice.len() == 5);
			MACH_CHECK(slice[0] == 1);
			MACH_CHECK(slice[1] == 2);
			MACH_CHECK(slice[2] == 3);
			MACH_CHECK(slice[3] == 4);
			MACH_CHECK(slice[4] == 5);
		}

		MACH_SUBCASE("initializer_list") {
			Slice<int const> slice = { 1, 2, 3, 4, 5 };
			MACH_CHECK(slice.len() == 5);
			MACH_CHECK(slice[0] == 1);
			MACH_CHECK(slice[1] == 2);
			MACH_CHECK(slice[2] == 3);
			MACH_CHECK(slice[3] == 4);
			MACH_CHECK(slice[4] == 5);
		}

		MACH_SUBCASE("copy constructor") {
			Slice<int const> slice = { 1, 2, 3, 4, 5 };
			Slice<int const> copy = slice;
			MACH_CHECK(copy.len() == 5);
			MACH_CHECK(copy[0] == 1);
			MACH_CHECK(copy[1] == 2);
			MACH_CHECK(copy[2] == 3);
			MACH_CHECK(copy[3] == 4);
			MACH_CHECK(copy[4] == 5);
		}

		MACH_SUBCASE("copy assignment") {
			Slice<int const> slice = { 1, 2, 3, 4, 5 };
			Slice<int const> copy;
			MACH_CHECK(copy.is_empty());
			copy = slice;
			MACH_CHECK(copy.len() == 5);
			MACH_CHECK(copy[0] == 1);
			MACH_CHECK(copy[1] == 2);
			MACH_CHECK(copy[2] == 3);
			MACH_CHECK(copy[3] == 4);
			MACH_CHECK(copy[4] == 5);
		}

		MACH_SUBCASE("move constructor") {
			Slice<int const> slice = { 1, 2, 3, 4, 5 };
			Slice<int const> move = Mach::move(slice);
			MACH_CHECK(move.len() == 5);
			MACH_CHECK(move[0] == 1);
			MACH_CHECK(move[1] == 2);
			MACH_CHECK(move[2] == 3);
			MACH_CHECK(move[3] == 4);
			MACH_CHECK(move[4] == 5);
			MACH_CHECK(slice.is_empty());
		}

		MACH_SUBCASE("move assignment") {
			Slice<int const> slice = { 1, 2, 3, 4, 5 };
			Slice<int const> move;
			MACH_CHECK(move.is_empty());
			move = Mach::move(slice);
			MACH_CHECK(move.len() == 5);
			MACH_CHECK(move[0] == 1);
			MACH_CHECK(move[1] == 2);
			MACH_CHECK(move[2] == 3);
			MACH_CHECK(move[3] == 4);
			MACH_CHECK(move[4] == 5);
			MACH_CHECK(slice.is_empty());
		}

		MACH_SUBCASE("shrink") {
			Slice<int const> slice = { 1, 2, 3, 4, 5 };
			Slice<int const> shrunk = slice.shrink(2);
			MACH_CHECK(shrunk.len() == 3);
			MACH_CHECK(shrunk[0] == 1);
			MACH_CHECK(shrunk[1] == 2);
			MACH_CHECK(shrunk[2] == 3);
			MACH_CHECK(slice.len() == 5);
			MACH_CHECK(slice.begin() == shrunk.begin());
		}
	}
}
#endif // MACH_ENABLE_TEST
