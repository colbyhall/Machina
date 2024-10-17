/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <core/containers/array.h>
#include <core/debug/test.h>

#if GRIZZLY_ENABLE_TEST
GRIZZLY_TEST_SUITE("containers") {
	using namespace grizzly::core;

	GRIZZLY_TEST_CASE("Array<HeapAllocator>") {
		GRIZZLY_SUBCASE("default constructor") {
			Array<int> arr;
			GRIZZLY_CHECK(arr.len() == 0);
		}

		GRIZZLY_SUBCASE("initializer_list") {
			Array<int> arr = { 1, 2, 3, 4, 5 };
			GRIZZLY_CHECK(arr.len() == 5);
			GRIZZLY_CHECK(arr[0] == 1);
			GRIZZLY_CHECK(arr[1] == 2);
			GRIZZLY_CHECK(arr[2] == 3);
			GRIZZLY_CHECK(arr[3] == 4);
			GRIZZLY_CHECK(arr[4] == 5);
		}

		GRIZZLY_SUBCASE("slice constructor") {
			Slice<const int> slice = { 1, 2, 3, 4, 5 };
			Array<int> arr{ slice };
			GRIZZLY_CHECK(arr.len() == 5);
			GRIZZLY_CHECK(arr[0] == 1);
			GRIZZLY_CHECK(arr[1] == 2);
			GRIZZLY_CHECK(arr[2] == 3);
			GRIZZLY_CHECK(arr[3] == 4);
			GRIZZLY_CHECK(arr[4] == 5);
		}

		GRIZZLY_SUBCASE("copy constructor") {
			Array<int> arr;
			arr.push(120);
			GRIZZLY_REQUIRE(arr.is_valid_index(0));
			GRIZZLY_REQUIRE(arr[0] == 120);

			Array<int> arr2{ arr };
			GRIZZLY_REQUIRE(arr2.is_valid_index(0));
			GRIZZLY_CHECK(arr2[0] == 120);
			GRIZZLY_CHECK(&arr[0] != &arr2[0]);
		}

		GRIZZLY_SUBCASE("copy assignment") {
			Array<int> arr;
			arr.push(120);
			// arr.push(120);
			GRIZZLY_REQUIRE(arr.is_valid_index(0));
			GRIZZLY_CHECK(arr[0] == 120);

			Array<int> arr2;
			arr2 = arr;
			GRIZZLY_REQUIRE(arr2.is_valid_index(0));
			GRIZZLY_CHECK(arr2[0] == 120);
			GRIZZLY_CHECK(&arr[0] != &arr2[0]);
		}

		GRIZZLY_SUBCASE("move constructor") {
			Array<int> arr;
			arr.push(120);
			GRIZZLY_REQUIRE(arr.is_valid_index(0));
			GRIZZLY_REQUIRE(arr[0] == 120);
			auto* ptr = &arr[0];

			Array<int> arr2{ grizzly::move(arr) };
			GRIZZLY_REQUIRE(arr2.is_valid_index(0));
			GRIZZLY_CHECK(arr2[0] == 120);
			GRIZZLY_CHECK(ptr == &arr2[0]);
		}

		GRIZZLY_SUBCASE("move assignment") {
			Array<int> arr;
			arr.push(120);
			GRIZZLY_REQUIRE(arr.is_valid_index(0));
			GRIZZLY_REQUIRE(arr[0] == 120);
			auto* ptr = &arr[0];

			Array<int> arr2;
			arr2 = grizzly::move(arr);
			GRIZZLY_REQUIRE(arr2.is_valid_index(0));
			GRIZZLY_CHECK(arr2[0] == 120);
			GRIZZLY_CHECK(ptr == &arr2[0]);
		}

		GRIZZLY_SUBCASE("as_slice") {
			Array<int> arr = { 1, 2, 3, 4, 5 };
			auto slice = arr.as_slice();
			GRIZZLY_CHECK(slice.len() == 5);
			GRIZZLY_CHECK(slice[0] == 1);
			GRIZZLY_CHECK(slice[1] == 2);
			GRIZZLY_CHECK(slice[2] == 3);
			GRIZZLY_CHECK(slice[3] == 4);
			GRIZZLY_CHECK(slice[4] == 5);
		}

		GRIZZLY_SUBCASE("as_const_slice") {
			Array<int> arr = { 1, 2, 3, 4, 5 };
			auto slice = arr.as_const_slice();
			GRIZZLY_CHECK(slice.len() == 5);
			GRIZZLY_CHECK(slice[0] == 1);
			GRIZZLY_CHECK(slice[1] == 2);
			GRIZZLY_CHECK(slice[2] == 3);
			GRIZZLY_CHECK(slice[3] == 4);
			GRIZZLY_CHECK(slice[4] == 5);
		}

		GRIZZLY_SUBCASE("get") {
			Array<int> arr;
			arr.push(120);
			GRIZZLY_REQUIRE(arr.is_valid_index(0));
			GRIZZLY_CHECK(arr.get(0).is_set());
			GRIZZLY_CHECK(arr.get(0).unwrap() == 120);
			GRIZZLY_CHECK(!arr.get(1).is_set());
		}

		GRIZZLY_SUBCASE("last") {
			Array<int> arr;
			GRIZZLY_CHECK(!arr.last().is_set());
			arr.push(120);
			arr.push(121);
			arr.push(122);
			GRIZZLY_CHECK(arr.last().is_set());
			GRIZZLY_CHECK(arr.last().unwrap() == 122);
		}

		GRIZZLY_SUBCASE("reserve") {
			Array<int> arr;
			arr.reserve(10);
			GRIZZLY_CHECK(arr.cap() >= 10);
		}

		GRIZZLY_SUBCASE("move insert") {
			Array<Array<int>> arr;

			Array<int> inner;
			inner.insert(0, 120);
			auto* ptr = &inner[0];

			arr.insert(0, grizzly::move(inner));
			GRIZZLY_REQUIRE(arr.is_valid_index(0));
			GRIZZLY_REQUIRE(arr[0].is_valid_index(0));
			GRIZZLY_CHECK(arr[0][0] == 120);
			GRIZZLY_CHECK(&arr[0][0] == ptr);
		}

		GRIZZLY_SUBCASE("copy insert") {
			Array<int> arr;
			arr.insert(0, 1);
			GRIZZLY_REQUIRE(arr.is_valid_index(0));
			GRIZZLY_CHECK(arr[0] == 1);
		}

		GRIZZLY_SUBCASE("move push") {
			Array<Array<int>> arr;

			Array<int> inner;
			inner.insert(0, 120);

			arr.push(grizzly::move(inner));
			GRIZZLY_REQUIRE(arr.is_valid_index(0));
			GRIZZLY_REQUIRE(arr[0].is_valid_index(0));
			GRIZZLY_CHECK(arr[0][0] == 120);
		}

		GRIZZLY_SUBCASE("copy push") {
			Array<int> arr;
			arr.push(1);
			GRIZZLY_REQUIRE(arr.len() == 1);
			GRIZZLY_CHECK(arr[0] == 1);
		}

		GRIZZLY_SUBCASE("remove") {
			Array<int> arr;

			arr.insert(0, 1);
			GRIZZLY_REQUIRE(arr.is_valid_index(0));
			GRIZZLY_REQUIRE(arr[0] == 1);

			const auto value = arr.remove(0);
			GRIZZLY_CHECK(value == 1);
			GRIZZLY_CHECK(arr.len() == 0);
		}

		GRIZZLY_SUBCASE("move pop") {
			Array<Array<int>> arr;

			Array<int> inner;
			inner.push(120);
			auto* ptr = &inner[0];
			arr.push(grizzly::move(inner));

			GRIZZLY_REQUIRE(arr.is_valid_index(0));
			GRIZZLY_REQUIRE(arr[0].is_valid_index(0));
			GRIZZLY_REQUIRE(arr[0][0] == 120);

			auto value = arr.pop();
			GRIZZLY_REQUIRE(value.is_set());
			auto unwrapped_inner = value.unwrap();
			GRIZZLY_REQUIRE(unwrapped_inner.is_valid_index(0));
			GRIZZLY_CHECK(unwrapped_inner[0] == 120);
			GRIZZLY_CHECK(ptr == &unwrapped_inner[0]);
		}

		GRIZZLY_SUBCASE("copy pop") {
			Array<int> arr;

			arr.push(1);
			GRIZZLY_REQUIRE(arr.is_valid_index(0));
			GRIZZLY_REQUIRE(arr[0] == 1);

			const auto value = arr.pop();
			GRIZZLY_REQUIRE(value.is_set());
			GRIZZLY_CHECK(value.unwrap() == 1);
		}

		GRIZZLY_SUBCASE("set len") {
			Array<int> arr;
			arr.set_len(10);
			GRIZZLY_CHECK(arr.len() == 10);
			for (usize i = 0; i < 10; i++) {
				GRIZZLY_CHECK(arr[i] == 0);
			}
		}

		GRIZZLY_SUBCASE("set len uninitialized") {
			Array<int> arr;
			arr.set_len_uninitialized(10);
			GRIZZLY_CHECK(arr.len() == 10);
		}
	}

	GRIZZLY_TEST_CASE("Array<InlineAllocator>") {
		GRIZZLY_SUBCASE("default constructor") {
			Array<int, InlineAllocator<120>> arr;
			GRIZZLY_CHECK(arr.len() == 0);
		}

		GRIZZLY_SUBCASE("initializer list") {
			Array<int, InlineAllocator<16>> arr = { 1, 2, 3, 4, 5 };
			GRIZZLY_CHECK(arr.len() == 5);
			GRIZZLY_CHECK(arr[0] == 1);
			GRIZZLY_CHECK(arr[1] == 2);
			GRIZZLY_CHECK(arr[2] == 3);
			GRIZZLY_CHECK(arr[3] == 4);
			GRIZZLY_CHECK(arr[4] == 5);
		}

		GRIZZLY_SUBCASE("copy constructor") {
			Array<int, InlineAllocator<120>> arr;
			arr.push(120);
			GRIZZLY_REQUIRE(arr.is_valid_index(0));
			GRIZZLY_REQUIRE(arr[0] == 120);

			Array<int, InlineAllocator<120>> arr2{ arr };
			GRIZZLY_REQUIRE(arr2.is_valid_index(0));
			GRIZZLY_CHECK(arr2[0] == 120);
			GRIZZLY_CHECK(&arr[0] != &arr2[0]);
		}

		GRIZZLY_SUBCASE("copy assignment") {
			Array<int, InlineAllocator<120>> arr;
			arr.push(120);
			GRIZZLY_REQUIRE(arr.is_valid_index(0));
			GRIZZLY_REQUIRE(arr[0] == 120);

			Array<int, InlineAllocator<120>> arr2;
			arr2 = arr;
			GRIZZLY_REQUIRE(arr2.is_valid_index(0));
			GRIZZLY_CHECK(arr2[0] == 120);
			GRIZZLY_CHECK(&arr[0] != &arr2[0]);
		}

		GRIZZLY_SUBCASE("move constructor") {
			Array<int, InlineAllocator<120>> arr;
			arr.push(120);
			GRIZZLY_REQUIRE(arr.is_valid_index(0));
			GRIZZLY_REQUIRE(arr[0] == 120);

			Array<int, InlineAllocator<120>> arr2{ grizzly::move(arr) };
			GRIZZLY_REQUIRE(arr2.is_valid_index(0));
			GRIZZLY_CHECK(arr2[0] == 120);
			GRIZZLY_CHECK(arr.is_empty());
		}

		GRIZZLY_SUBCASE("move assignment") {
			Array<int, InlineAllocator<120>> arr;
			arr.push(120);
			GRIZZLY_REQUIRE(arr.is_valid_index(0));
			GRIZZLY_REQUIRE(arr[0] == 120);

			Array<int, InlineAllocator<120>> arr2;
			arr2 = grizzly::move(arr);
			GRIZZLY_REQUIRE(arr2.is_valid_index(0));
			GRIZZLY_CHECK(arr2[0] == 120);
			GRIZZLY_CHECK(arr.is_empty());
		}

		GRIZZLY_SUBCASE("move insert") {
			Array<Array<int, InlineAllocator<120>>, InlineAllocator<120>> arr;

			Array<int, InlineAllocator<120>> inner;
			inner.insert(0, 120);

			arr.insert(0, grizzly::move(inner));
			GRIZZLY_REQUIRE(arr.is_valid_index(0));
			GRIZZLY_REQUIRE(arr[0].is_valid_index(0));
			GRIZZLY_CHECK(arr[0][0] == 120);
		}

		GRIZZLY_SUBCASE("copy insert") {
			Array<int, InlineAllocator<120>> arr;
			arr.insert(0, 1);
			GRIZZLY_REQUIRE(arr.is_valid_index(0));
			GRIZZLY_CHECK(arr[0] == 1);
		}

		GRIZZLY_SUBCASE("move push") {
			Array<Array<int, InlineAllocator<120>>, InlineAllocator<120>> arr;

			Array<int, InlineAllocator<120>> inner;
			inner.insert(0, 120);

			arr.push(grizzly::move(inner));
			GRIZZLY_REQUIRE(arr.is_valid_index(0));
			GRIZZLY_REQUIRE(arr[0].is_valid_index(0));
			GRIZZLY_CHECK(arr[0][0] == 120);
		}

		GRIZZLY_SUBCASE("copy push") {
			Array<int, InlineAllocator<120>> arr;
			arr.push(1);
			GRIZZLY_REQUIRE(arr.len() == 1);
			GRIZZLY_CHECK(arr[0] == 1);
		}

		GRIZZLY_SUBCASE("remove") {
			Array<int, InlineAllocator<120>> arr;

			arr.insert(0, 1);
			GRIZZLY_REQUIRE(arr.is_valid_index(0));
			GRIZZLY_REQUIRE(arr[0] == 1);

			const auto value = arr.remove(0);
			GRIZZLY_CHECK(value == 1);
			GRIZZLY_CHECK(arr.len() == 0);
		}

		GRIZZLY_SUBCASE("move pop") {
			Array<Array<int, InlineAllocator<120>>, InlineAllocator<120>> arr;

			Array<int, InlineAllocator<120>> inner;
			inner.push(120);
			arr.push(grizzly::move(inner));

			GRIZZLY_REQUIRE(arr.is_valid_index(0));
			GRIZZLY_REQUIRE(arr[0].is_valid_index(0));
			GRIZZLY_REQUIRE(arr[0][0] == 120);

			auto value = arr.pop();
			GRIZZLY_REQUIRE(value.is_set());
			auto unwrapped_inner = value.unwrap();
			GRIZZLY_REQUIRE(unwrapped_inner.is_valid_index(0));
			GRIZZLY_CHECK(unwrapped_inner[0] == 120);
		}

		GRIZZLY_SUBCASE("copy pop") {
			Array<int, InlineAllocator<120>> arr;

			arr.push(1);
			GRIZZLY_REQUIRE(arr.is_valid_index(0));
			GRIZZLY_REQUIRE(arr[0] == 1);

			const auto value = arr.pop();
			GRIZZLY_REQUIRE(value.is_set());
			GRIZZLY_CHECK(value.unwrap() == 1);
		}

		GRIZZLY_SUBCASE("set len") {
			Array<int, InlineAllocator<120>> arr;
			arr.set_len(10);
			GRIZZLY_CHECK(arr.len() == 10);
			for (usize i = 0; i < 10; i++) {
				GRIZZLY_CHECK(arr[i] == 0);
			}
		}

		GRIZZLY_SUBCASE("set len uninitialized") {
			Array<int, InlineAllocator<120>> arr;
			arr.set_len_uninitialized(10);
			GRIZZLY_CHECK(arr.len() == 10);
		}
	}
}
#endif // GRIZZLY_ENABLE_TEST
