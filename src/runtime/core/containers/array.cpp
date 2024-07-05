/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include "core/containers/array.h"
#include "core/debug/test.h"

using namespace op::core;

OP_TEST_SUITE("containers") {
	OP_TEST_CASE("Array<HeapAllocator>") {
		OP_SUBCASE("default constructor") {
			Array<int> arr;
			OP_CHECK(arr.len() == 0);
		}

		OP_SUBCASE("initializer_list") {
			Array<int> arr = { 1, 2, 3, 4, 5 };
			OP_CHECK(arr.len() == 5);
			OP_CHECK(arr[0] == 1);
			OP_CHECK(arr[1] == 2);
			OP_CHECK(arr[2] == 3);
			OP_CHECK(arr[3] == 4);
			OP_CHECK(arr[4] == 5);
		}

		OP_SUBCASE("slice constructor") {
			Slice<const int> slice = { 1, 2, 3, 4, 5 };
			Array<int> arr{ slice };
			OP_CHECK(arr.len() == 5);
			OP_CHECK(arr[0] == 1);
			OP_CHECK(arr[1] == 2);
			OP_CHECK(arr[2] == 3);
			OP_CHECK(arr[3] == 4);
			OP_CHECK(arr[4] == 5);
		}

		OP_SUBCASE("copy constructor") {
			Array<int> arr;
			arr.push(120);
			OP_REQUIRE(arr.is_valid_index(0));
			OP_REQUIRE(arr[0] == 120);

			Array<int> arr2{ arr };
			OP_REQUIRE(arr2.is_valid_index(0));
			OP_CHECK(arr2[0] == 120);
			OP_CHECK(&arr[0] != &arr2[0]);
		}

		OP_SUBCASE("copy assignment") {
			Array<int> arr;
			arr.push(120);
			// arr.push(120);
			OP_REQUIRE(arr.is_valid_index(0));
			OP_CHECK(arr[0] == 120);

			Array<int> arr2;
			arr2 = arr;
			OP_REQUIRE(arr2.is_valid_index(0));
			OP_CHECK(arr2[0] == 120);
			OP_CHECK(&arr[0] != &arr2[0]);
		}

		OP_SUBCASE("move constructor") {
			Array<int> arr;
			arr.push(120);
			OP_REQUIRE(arr.is_valid_index(0));
			OP_REQUIRE(arr[0] == 120);
			auto* ptr = &arr[0];

			Array<int> arr2{ op::move(arr) };
			OP_REQUIRE(arr2.is_valid_index(0));
			OP_CHECK(arr2[0] == 120);
			OP_CHECK(ptr == &arr2[0]);
		}

		OP_SUBCASE("move assignment") {
			Array<int> arr;
			arr.push(120);
			OP_REQUIRE(arr.is_valid_index(0));
			OP_REQUIRE(arr[0] == 120);
			auto* ptr = &arr[0];

			Array<int> arr2;
			arr2 = op::move(arr);
			OP_REQUIRE(arr2.is_valid_index(0));
			OP_CHECK(arr2[0] == 120);
			OP_CHECK(ptr == &arr2[0]);
		}

		OP_SUBCASE("as_slice") {
			Array<int> arr = { 1, 2, 3, 4, 5 };
			auto slice = arr.as_slice();
			OP_CHECK(slice.len() == 5);
			OP_CHECK(slice[0] == 1);
			OP_CHECK(slice[1] == 2);
			OP_CHECK(slice[2] == 3);
			OP_CHECK(slice[3] == 4);
			OP_CHECK(slice[4] == 5);
		}

		OP_SUBCASE("as_const_slice") {
			Array<int> arr = { 1, 2, 3, 4, 5 };
			auto slice = arr.as_const_slice();
			OP_CHECK(slice.len() == 5);
			OP_CHECK(slice[0] == 1);
			OP_CHECK(slice[1] == 2);
			OP_CHECK(slice[2] == 3);
			OP_CHECK(slice[3] == 4);
			OP_CHECK(slice[4] == 5);
		}

		OP_SUBCASE("get") {
			Array<int> arr;
			arr.push(120);
			OP_REQUIRE(arr.is_valid_index(0));
			OP_CHECK(arr.get(0).is_set());
			OP_CHECK(arr.get(0).unwrap() == 120);
			OP_CHECK(!arr.get(1).is_set());
		}

		OP_SUBCASE("last") {
			Array<int> arr;
			OP_CHECK(!arr.last().is_set());
			arr.push(120);
			arr.push(121);
			arr.push(122);
			OP_CHECK(arr.last().is_set());
			OP_CHECK(arr.last().unwrap() == 122);
		}

		OP_SUBCASE("reserve") {
			Array<int> arr;
			arr.reserve(10);
			OP_CHECK(arr.cap() >= 10);
		}

		OP_SUBCASE("move insert") {
			Array<Array<int>> arr;

			Array<int> inner;
			inner.insert(0, 120);
			auto* ptr = &inner[0];

			arr.insert(0, op::move(inner));
			OP_REQUIRE(arr.is_valid_index(0));
			OP_REQUIRE(arr[0].is_valid_index(0));
			OP_CHECK(arr[0][0] == 120);
			OP_CHECK(&arr[0][0] == ptr);
		}

		OP_SUBCASE("copy insert") {
			Array<int> arr;
			arr.insert(0, 1);
			OP_REQUIRE(arr.is_valid_index(0));
			OP_CHECK(arr[0] == 1);
		}

		OP_SUBCASE("move push") {
			Array<Array<int>> arr;

			Array<int> inner;
			inner.insert(0, 120);

			arr.push(op::move(inner));
			OP_REQUIRE(arr.is_valid_index(0));
			OP_REQUIRE(arr[0].is_valid_index(0));
			OP_CHECK(arr[0][0] == 120);
		}

		OP_SUBCASE("copy push") {
			Array<int> arr;
			arr.push(1);
			OP_REQUIRE(arr.len() == 1);
			OP_CHECK(arr[0] == 1);
		}

		OP_SUBCASE("remove") {
			Array<int> arr;

			arr.insert(0, 1);
			OP_REQUIRE(arr.is_valid_index(0));
			OP_REQUIRE(arr[0] == 1);

			const auto value = arr.remove(0);
			OP_CHECK(value == 1);
			OP_CHECK(arr.len() == 0);
		}

		OP_SUBCASE("move pop") {
			Array<Array<int>> arr;

			Array<int> inner;
			inner.push(120);
			auto* ptr = &inner[0];
			arr.push(op::move(inner));

			OP_REQUIRE(arr.is_valid_index(0));
			OP_REQUIRE(arr[0].is_valid_index(0));
			OP_REQUIRE(arr[0][0] == 120);

			auto value = arr.pop();
			OP_REQUIRE(value.is_set());
			auto unwrapped_inner = value.unwrap();
			OP_REQUIRE(unwrapped_inner.is_valid_index(0));
			OP_CHECK(unwrapped_inner[0] == 120);
			OP_CHECK(ptr == &unwrapped_inner[0]);
		}

		OP_SUBCASE("copy pop") {
			Array<int> arr;

			arr.push(1);
			OP_REQUIRE(arr.is_valid_index(0));
			OP_REQUIRE(arr[0] == 1);

			const auto value = arr.pop();
			OP_REQUIRE(value.is_set());
			OP_CHECK(value.unwrap() == 1);
		}
	}

	OP_TEST_CASE("Array<InlineAllocator>") {
		OP_SUBCASE("default constructor") {
			Array<int, InlineAllocator<120>> arr;
			OP_CHECK(arr.len() == 0);
		}

		OP_SUBCASE("initializer list") {
			Array<int, InlineAllocator<16>> arr = { 1, 2, 3, 4, 5 };
			OP_CHECK(arr.len() == 5);
			OP_CHECK(arr[0] == 1);
			OP_CHECK(arr[1] == 2);
			OP_CHECK(arr[2] == 3);
			OP_CHECK(arr[3] == 4);
			OP_CHECK(arr[4] == 5);
		}

		OP_SUBCASE("copy constructor") {
			Array<int, InlineAllocator<120>> arr;
			arr.push(120);
			OP_REQUIRE(arr.is_valid_index(0));
			OP_REQUIRE(arr[0] == 120);

			Array<int, InlineAllocator<120>> arr2{ arr };
			OP_REQUIRE(arr2.is_valid_index(0));
			OP_CHECK(arr2[0] == 120);
			OP_CHECK(&arr[0] != &arr2[0]);
		}

		OP_SUBCASE("copy assignment") {
			Array<int, InlineAllocator<120>> arr;
			arr.push(120);
			OP_REQUIRE(arr.is_valid_index(0));
			OP_REQUIRE(arr[0] == 120);

			Array<int, InlineAllocator<120>> arr2;
			arr2 = arr;
			OP_REQUIRE(arr2.is_valid_index(0));
			OP_CHECK(arr2[0] == 120);
			OP_CHECK(&arr[0] != &arr2[0]);
		}

		OP_SUBCASE("move constructor") {
			Array<int, InlineAllocator<120>> arr;
			arr.push(120);
			OP_REQUIRE(arr.is_valid_index(0));
			OP_REQUIRE(arr[0] == 120);

			Array<int, InlineAllocator<120>> arr2{ op::move(arr) };
			OP_REQUIRE(arr2.is_valid_index(0));
			OP_CHECK(arr2[0] == 120);
			OP_CHECK(arr.is_empty());
		}

		OP_SUBCASE("move assignment") {
			Array<int, InlineAllocator<120>> arr;
			arr.push(120);
			OP_REQUIRE(arr.is_valid_index(0));
			OP_REQUIRE(arr[0] == 120);

			Array<int, InlineAllocator<120>> arr2;
			arr2 = op::move(arr);
			OP_REQUIRE(arr2.is_valid_index(0));
			OP_CHECK(arr2[0] == 120);
			OP_CHECK(arr.is_empty());
		}

		OP_SUBCASE("move insert") {
			Array<Array<int, InlineAllocator<120>>, InlineAllocator<120>> arr;

			Array<int, InlineAllocator<120>> inner;
			inner.insert(0, 120);

			arr.insert(0, op::move(inner));
			OP_REQUIRE(arr.is_valid_index(0));
			OP_REQUIRE(arr[0].is_valid_index(0));
			OP_CHECK(arr[0][0] == 120);
		}

		OP_SUBCASE("copy insert") {
			Array<int, InlineAllocator<120>> arr;
			arr.insert(0, 1);
			OP_REQUIRE(arr.is_valid_index(0));
			OP_CHECK(arr[0] == 1);
		}

		OP_SUBCASE("move push") {
			Array<Array<int, InlineAllocator<120>>, InlineAllocator<120>> arr;

			Array<int, InlineAllocator<120>> inner;
			inner.insert(0, 120);

			arr.push(op::move(inner));
			OP_REQUIRE(arr.is_valid_index(0));
			OP_REQUIRE(arr[0].is_valid_index(0));
			OP_CHECK(arr[0][0] == 120);
		}

		OP_SUBCASE("copy push") {
			Array<int, InlineAllocator<120>> arr;
			arr.push(1);
			OP_REQUIRE(arr.len() == 1);
			OP_CHECK(arr[0] == 1);
		}

		OP_SUBCASE("remove") {
			Array<int, InlineAllocator<120>> arr;

			arr.insert(0, 1);
			OP_REQUIRE(arr.is_valid_index(0));
			OP_REQUIRE(arr[0] == 1);

			const auto value = arr.remove(0);
			OP_CHECK(value == 1);
			OP_CHECK(arr.len() == 0);
		}

		OP_SUBCASE("move pop") {
			Array<Array<int, InlineAllocator<120>>, InlineAllocator<120>> arr;

			Array<int, InlineAllocator<120>> inner;
			inner.push(120);
			arr.push(op::move(inner));

			OP_REQUIRE(arr.is_valid_index(0));
			OP_REQUIRE(arr[0].is_valid_index(0));
			OP_REQUIRE(arr[0][0] == 120);

			auto value = arr.pop();
			OP_REQUIRE(value.is_set());
			auto unwrapped_inner = value.unwrap();
			OP_REQUIRE(unwrapped_inner.is_valid_index(0));
			OP_CHECK(unwrapped_inner[0] == 120);
		}

		OP_SUBCASE("copy pop") {
			Array<int, InlineAllocator<120>> arr;

			arr.push(1);
			OP_REQUIRE(arr.is_valid_index(0));
			OP_REQUIRE(arr[0] == 1);

			const auto value = arr.pop();
			OP_REQUIRE(value.is_set());
			OP_CHECK(value.unwrap() == 1);
		}
	}
}