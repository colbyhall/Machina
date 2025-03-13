/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Containers/Slice.hpp>
#include <Core/Debug/Test.hpp>

#if FORGE_ENABLE_TEST
FORGE_TEST_SUITE("Containers") {
	using namespace Forge;

	FORGE_TEST_CASE("Slice") {
		FORGE_SUBCASE("default constructor") {
			Slice<int> slice;
			FORGE_CHECK(slice.len() == 0);
		}

		FORGE_SUBCASE("value constructor") {
			int arr[] = { 1, 2, 3, 4, 5 };
			Slice<int> slice{ arr, 5 };
			FORGE_CHECK(slice.len() == 5);
			FORGE_CHECK(slice[0] == 1);
			FORGE_CHECK(slice[1] == 2);
			FORGE_CHECK(slice[2] == 3);
			FORGE_CHECK(slice[3] == 4);
			FORGE_CHECK(slice[4] == 5);
		}

		FORGE_SUBCASE("initializer_list") {
			Slice<int const> slice = { 1, 2, 3, 4, 5 };
			FORGE_CHECK(slice.len() == 5);
			FORGE_CHECK(slice[0] == 1);
			FORGE_CHECK(slice[1] == 2);
			FORGE_CHECK(slice[2] == 3);
			FORGE_CHECK(slice[3] == 4);
			FORGE_CHECK(slice[4] == 5);
		}

		FORGE_SUBCASE("copy constructor") {
			Slice<int const> slice = { 1, 2, 3, 4, 5 };
			Slice<int const> copy = slice;
			FORGE_CHECK(copy.len() == 5);
			FORGE_CHECK(copy[0] == 1);
			FORGE_CHECK(copy[1] == 2);
			FORGE_CHECK(copy[2] == 3);
			FORGE_CHECK(copy[3] == 4);
			FORGE_CHECK(copy[4] == 5);
		}

		FORGE_SUBCASE("copy assignment") {
			Slice<int const> slice = { 1, 2, 3, 4, 5 };
			Slice<int const> copy;
			FORGE_CHECK(copy.is_empty());
			copy = slice;
			FORGE_CHECK(copy.len() == 5);
			FORGE_CHECK(copy[0] == 1);
			FORGE_CHECK(copy[1] == 2);
			FORGE_CHECK(copy[2] == 3);
			FORGE_CHECK(copy[3] == 4);
			FORGE_CHECK(copy[4] == 5);
		}

		FORGE_SUBCASE("move constructor") {
			Slice<int const> slice = { 1, 2, 3, 4, 5 };
			Slice<int const> move = Forge::move(slice);
			FORGE_CHECK(move.len() == 5);
			FORGE_CHECK(move[0] == 1);
			FORGE_CHECK(move[1] == 2);
			FORGE_CHECK(move[2] == 3);
			FORGE_CHECK(move[3] == 4);
			FORGE_CHECK(move[4] == 5);
			FORGE_CHECK(slice.is_empty());
		}

		FORGE_SUBCASE("move assignment") {
			Slice<int const> slice = { 1, 2, 3, 4, 5 };
			Slice<int const> move;
			FORGE_CHECK(move.is_empty());
			move = Forge::move(slice);
			FORGE_CHECK(move.len() == 5);
			FORGE_CHECK(move[0] == 1);
			FORGE_CHECK(move[1] == 2);
			FORGE_CHECK(move[2] == 3);
			FORGE_CHECK(move[3] == 4);
			FORGE_CHECK(move[4] == 5);
			FORGE_CHECK(slice.is_empty());
		}

		FORGE_SUBCASE("shrink") {
			Slice<int const> slice = { 1, 2, 3, 4, 5 };
			Slice<int const> shrunk = slice.shrink(2);
			FORGE_CHECK(shrunk.len() == 3);
			FORGE_CHECK(shrunk[0] == 1);
			FORGE_CHECK(shrunk[1] == 2);
			FORGE_CHECK(shrunk[2] == 3);
			FORGE_CHECK(slice.len() == 5);
			FORGE_CHECK(slice.begin() == shrunk.begin());
		}
	}
}
#endif // FORGE_ENABLE_TEST
