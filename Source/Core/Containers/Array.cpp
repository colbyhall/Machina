/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Containers/Array.hpp>
#include <Core/Debug/Test.hpp>

#if FORGE_ENABLE_TEST
FORGE_TEST_SUITE("Containers") {
	using namespace Forge::Core;

	FORGE_TEST_CASE("Array<HeapAllocator>") {
		FORGE_SUBCASE("default constructor") {
			Array<int> arr;
			FORGE_CHECK(arr.len() == 0);
		}

		FORGE_SUBCASE("initializer_list") {
			Array<int> arr = { 1, 2, 3, 4, 5 };
			FORGE_CHECK(arr.len() == 5);
			FORGE_CHECK(arr[0] == 1);
			FORGE_CHECK(arr[1] == 2);
			FORGE_CHECK(arr[2] == 3);
			FORGE_CHECK(arr[3] == 4);
			FORGE_CHECK(arr[4] == 5);
		}

		FORGE_SUBCASE("slice constructor") {
			Slice<const int> slice = { 1, 2, 3, 4, 5 };
			Array<int> arr{ slice };
			FORGE_CHECK(arr.len() == 5);
			FORGE_CHECK(arr[0] == 1);
			FORGE_CHECK(arr[1] == 2);
			FORGE_CHECK(arr[2] == 3);
			FORGE_CHECK(arr[3] == 4);
			FORGE_CHECK(arr[4] == 5);
		}

		FORGE_SUBCASE("copy constructor") {
			Array<int> arr;
			arr.push(120);
			FORGE_REQUIRE(arr.is_valid_index(0));
			FORGE_REQUIRE(arr[0] == 120);

			Array<int> arr2{ arr };
			FORGE_REQUIRE(arr2.is_valid_index(0));
			FORGE_CHECK(arr2[0] == 120);
			FORGE_CHECK(&arr[0] != &arr2[0]);
		}

		FORGE_SUBCASE("copy assignment") {
			Array<int> arr;
			arr.push(120);
			// arr.push(120);
			FORGE_REQUIRE(arr.is_valid_index(0));
			FORGE_CHECK(arr[0] == 120);

			Array<int> arr2;
			arr2 = arr;
			FORGE_REQUIRE(arr2.is_valid_index(0));
			FORGE_CHECK(arr2[0] == 120);
			FORGE_CHECK(&arr[0] != &arr2[0]);
		}

		FORGE_SUBCASE("move constructor") {
			Array<int> arr;
			arr.push(120);
			FORGE_REQUIRE(arr.is_valid_index(0));
			FORGE_REQUIRE(arr[0] == 120);
			auto* ptr = &arr[0];

			Array<int> arr2{ Forge::move(arr) };
			FORGE_REQUIRE(arr2.is_valid_index(0));
			FORGE_CHECK(arr2[0] == 120);
			FORGE_CHECK(ptr == &arr2[0]);
		}

		FORGE_SUBCASE("move assignment") {
			Array<int> arr;
			arr.push(120);
			FORGE_REQUIRE(arr.is_valid_index(0));
			FORGE_REQUIRE(arr[0] == 120);
			auto* ptr = &arr[0];

			Array<int> arr2;
			arr2 = Forge::move(arr);
			FORGE_REQUIRE(arr2.is_valid_index(0));
			FORGE_CHECK(arr2[0] == 120);
			FORGE_CHECK(ptr == &arr2[0]);
		}

		FORGE_SUBCASE("as_slice") {
			Array<int> arr = { 1, 2, 3, 4, 5 };
			auto slice = arr.as_slice();
			FORGE_CHECK(slice.len() == 5);
			FORGE_CHECK(slice[0] == 1);
			FORGE_CHECK(slice[1] == 2);
			FORGE_CHECK(slice[2] == 3);
			FORGE_CHECK(slice[3] == 4);
			FORGE_CHECK(slice[4] == 5);
		}

		FORGE_SUBCASE("as_const_slice") {
			Array<int> arr = { 1, 2, 3, 4, 5 };
			auto slice = arr.as_const_slice();
			FORGE_CHECK(slice.len() == 5);
			FORGE_CHECK(slice[0] == 1);
			FORGE_CHECK(slice[1] == 2);
			FORGE_CHECK(slice[2] == 3);
			FORGE_CHECK(slice[3] == 4);
			FORGE_CHECK(slice[4] == 5);
		}

		FORGE_SUBCASE("get") {
			Array<int> arr;
			arr.push(120);
			FORGE_REQUIRE(arr.is_valid_index(0));
			FORGE_CHECK(arr.get(0).is_set());
			FORGE_CHECK(arr.get(0).unwrap() == 120);
			FORGE_CHECK(!arr.get(1).is_set());
		}

		FORGE_SUBCASE("last") {
			Array<int> arr;
			FORGE_CHECK(!arr.last().is_set());
			arr.push(120);
			arr.push(121);
			arr.push(122);
			FORGE_CHECK(arr.last().is_set());
			FORGE_CHECK(arr.last().unwrap() == 122);
		}

		FORGE_SUBCASE("reserve") {
			Array<int> arr;
			arr.reserve(10);
			FORGE_CHECK(arr.cap() >= 10);
		}

		FORGE_SUBCASE("move insert") {
			Array<Array<int>> arr;

			Array<int> inner;
			inner.insert(0, 120);
			auto* ptr = &inner[0];

			arr.insert(0, Forge::move(inner));
			FORGE_REQUIRE(arr.is_valid_index(0));
			FORGE_REQUIRE(arr[0].is_valid_index(0));
			FORGE_CHECK(arr[0][0] == 120);
			FORGE_CHECK(&arr[0][0] == ptr);
		}

		FORGE_SUBCASE("copy insert") {
			Array<int> arr;
			arr.insert(0, 1);
			FORGE_REQUIRE(arr.is_valid_index(0));
			FORGE_CHECK(arr[0] == 1);
		}

		FORGE_SUBCASE("move push") {
			Array<Array<int>> arr;

			Array<int> inner;
			inner.insert(0, 120);

			arr.push(Forge::move(inner));
			FORGE_REQUIRE(arr.is_valid_index(0));
			FORGE_REQUIRE(arr[0].is_valid_index(0));
			FORGE_CHECK(arr[0][0] == 120);
		}

		FORGE_SUBCASE("copy push") {
			Array<int> arr;
			arr.push(1);
			FORGE_REQUIRE(arr.len() == 1);
			FORGE_CHECK(arr[0] == 1);
		}

		FORGE_SUBCASE("remove") {
			Array<int> arr;

			arr.insert(0, 1);
			FORGE_REQUIRE(arr.is_valid_index(0));
			FORGE_REQUIRE(arr[0] == 1);

			const auto value = arr.remove(0);
			FORGE_CHECK(value == 1);
			FORGE_CHECK(arr.len() == 0);
		}

		FORGE_SUBCASE("move pop") {
			Array<Array<int>> arr;

			Array<int> inner;
			inner.push(120);
			auto* ptr = &inner[0];
			arr.push(Forge::move(inner));

			FORGE_REQUIRE(arr.is_valid_index(0));
			FORGE_REQUIRE(arr[0].is_valid_index(0));
			FORGE_REQUIRE(arr[0][0] == 120);

			auto value = arr.pop();
			FORGE_REQUIRE(value.is_set());
			auto unwrapped_inner = value.unwrap();
			FORGE_REQUIRE(unwrapped_inner.is_valid_index(0));
			FORGE_CHECK(unwrapped_inner[0] == 120);
			FORGE_CHECK(ptr == &unwrapped_inner[0]);
		}

		FORGE_SUBCASE("copy pop") {
			Array<int> arr;

			arr.push(1);
			FORGE_REQUIRE(arr.is_valid_index(0));
			FORGE_REQUIRE(arr[0] == 1);

			const auto value = arr.pop();
			FORGE_REQUIRE(value.is_set());
			FORGE_CHECK(value.unwrap() == 1);
		}

		FORGE_SUBCASE("set len") {
			Array<int> arr;
			arr.set_len(10);
			FORGE_CHECK(arr.len() == 10);
			for (usize i = 0; i < 10; i++) {
				FORGE_CHECK(arr[i] == 0);
			}
		}

		FORGE_SUBCASE("set len uninitialized") {
			Array<int> arr;
			arr.set_len_uninitialized(10);
			FORGE_CHECK(arr.len() == 10);
		}
	}

	FORGE_TEST_CASE("Array<InlineAllocator>") {
		FORGE_SUBCASE("default constructor") {
			Array<int, InlineAllocator<120>> arr;
			FORGE_CHECK(arr.len() == 0);
		}

		FORGE_SUBCASE("initializer list") {
			Array<int, InlineAllocator<16>> arr = { 1, 2, 3, 4, 5 };
			FORGE_CHECK(arr.len() == 5);
			FORGE_CHECK(arr[0] == 1);
			FORGE_CHECK(arr[1] == 2);
			FORGE_CHECK(arr[2] == 3);
			FORGE_CHECK(arr[3] == 4);
			FORGE_CHECK(arr[4] == 5);
		}

		FORGE_SUBCASE("copy constructor") {
			Array<int, InlineAllocator<120>> arr;
			arr.push(120);
			FORGE_REQUIRE(arr.is_valid_index(0));
			FORGE_REQUIRE(arr[0] == 120);

			Array<int, InlineAllocator<120>> arr2{ arr };
			FORGE_REQUIRE(arr2.is_valid_index(0));
			FORGE_CHECK(arr2[0] == 120);
			FORGE_CHECK(&arr[0] != &arr2[0]);
		}

		FORGE_SUBCASE("copy assignment") {
			Array<int, InlineAllocator<120>> arr;
			arr.push(120);
			FORGE_REQUIRE(arr.is_valid_index(0));
			FORGE_REQUIRE(arr[0] == 120);

			Array<int, InlineAllocator<120>> arr2;
			arr2 = arr;
			FORGE_REQUIRE(arr2.is_valid_index(0));
			FORGE_CHECK(arr2[0] == 120);
			FORGE_CHECK(&arr[0] != &arr2[0]);
		}

		FORGE_SUBCASE("move constructor") {
			Array<int, InlineAllocator<120>> arr;
			arr.push(120);
			FORGE_REQUIRE(arr.is_valid_index(0));
			FORGE_REQUIRE(arr[0] == 120);

			Array<int, InlineAllocator<120>> arr2{ Forge::move(arr) };
			FORGE_REQUIRE(arr2.is_valid_index(0));
			FORGE_CHECK(arr2[0] == 120);
			FORGE_CHECK(arr.is_empty());
		}

		FORGE_SUBCASE("move assignment") {
			Array<int, InlineAllocator<120>> arr;
			arr.push(120);
			FORGE_REQUIRE(arr.is_valid_index(0));
			FORGE_REQUIRE(arr[0] == 120);

			Array<int, InlineAllocator<120>> arr2;
			arr2 = Forge::move(arr);
			FORGE_REQUIRE(arr2.is_valid_index(0));
			FORGE_CHECK(arr2[0] == 120);
			FORGE_CHECK(arr.is_empty());
		}

		FORGE_SUBCASE("move insert") {
			Array<Array<int, InlineAllocator<120>>, InlineAllocator<120>> arr;

			Array<int, InlineAllocator<120>> inner;
			inner.insert(0, 120);

			arr.insert(0, Forge::move(inner));
			FORGE_REQUIRE(arr.is_valid_index(0));
			FORGE_REQUIRE(arr[0].is_valid_index(0));
			FORGE_CHECK(arr[0][0] == 120);
		}

		FORGE_SUBCASE("copy insert") {
			Array<int, InlineAllocator<120>> arr;
			arr.insert(0, 1);
			FORGE_REQUIRE(arr.is_valid_index(0));
			FORGE_CHECK(arr[0] == 1);
		}

		FORGE_SUBCASE("move push") {
			Array<Array<int, InlineAllocator<120>>, InlineAllocator<120>> arr;

			Array<int, InlineAllocator<120>> inner;
			inner.insert(0, 120);

			arr.push(Forge::move(inner));
			FORGE_REQUIRE(arr.is_valid_index(0));
			FORGE_REQUIRE(arr[0].is_valid_index(0));
			FORGE_CHECK(arr[0][0] == 120);
		}

		FORGE_SUBCASE("copy push") {
			Array<int, InlineAllocator<120>> arr;
			arr.push(1);
			FORGE_REQUIRE(arr.len() == 1);
			FORGE_CHECK(arr[0] == 1);
		}

		FORGE_SUBCASE("remove") {
			Array<int, InlineAllocator<120>> arr;

			arr.insert(0, 1);
			FORGE_REQUIRE(arr.is_valid_index(0));
			FORGE_REQUIRE(arr[0] == 1);

			const auto value = arr.remove(0);
			FORGE_CHECK(value == 1);
			FORGE_CHECK(arr.len() == 0);
		}

		FORGE_SUBCASE("move pop") {
			Array<Array<int, InlineAllocator<120>>, InlineAllocator<120>> arr;

			Array<int, InlineAllocator<120>> inner;
			inner.push(120);
			arr.push(Forge::move(inner));

			FORGE_REQUIRE(arr.is_valid_index(0));
			FORGE_REQUIRE(arr[0].is_valid_index(0));
			FORGE_REQUIRE(arr[0][0] == 120);

			auto value = arr.pop();
			FORGE_REQUIRE(value.is_set());
			auto unwrapped_inner = value.unwrap();
			FORGE_REQUIRE(unwrapped_inner.is_valid_index(0));
			FORGE_CHECK(unwrapped_inner[0] == 120);
		}

		FORGE_SUBCASE("copy pop") {
			Array<int, InlineAllocator<120>> arr;

			arr.push(1);
			FORGE_REQUIRE(arr.is_valid_index(0));
			FORGE_REQUIRE(arr[0] == 1);

			const auto value = arr.pop();
			FORGE_REQUIRE(value.is_set());
			FORGE_CHECK(value.unwrap() == 1);
		}

		FORGE_SUBCASE("set len") {
			Array<int, InlineAllocator<120>> arr;
			arr.set_len(10);
			FORGE_CHECK(arr.len() == 10);
			for (usize i = 0; i < 10; i++) {
				FORGE_CHECK(arr[i] == 0);
			}
		}

		FORGE_SUBCASE("set len uninitialized") {
			Array<int, InlineAllocator<120>> arr;
			arr.set_len_uninitialized(10);
			FORGE_CHECK(arr.len() == 10);
		}
	}
}
#endif // FORGE_ENABLE_TEST
