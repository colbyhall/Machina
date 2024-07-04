/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include "core/containers/array.h"
#include "core/test.h"

using namespace op::core;

OP_TEST_SUITE("containers") {
	OP_TEST_CASE("Array<HeapAllocator>") {
		OP_SUBCASE("default constructor") {
			Array<int> arr;
			OP_CHECK(arr.len() == 0);
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

	OP_TEST_CASE("Array<StackAllocator>") {
		OP_SUBCASE("default constructor") {
			Array<int, StackAllocator<120>> arr;
			OP_CHECK(arr.len() == 0);
		}

		OP_SUBCASE("copy constructor") {
			Array<int, StackAllocator<120>> arr;
			arr.push(120);
			OP_REQUIRE(arr.is_valid_index(0));
			OP_REQUIRE(arr[0] == 120);

			Array<int, StackAllocator<120>> arr2{ arr };
			OP_REQUIRE(arr2.is_valid_index(0));
			OP_CHECK(arr2[0] == 120);
			OP_CHECK(&arr[0] != &arr2[0]);
		}

		OP_SUBCASE("copy assignment") {
			Array<int, StackAllocator<120>> arr;
			arr.push(120);
			OP_REQUIRE(arr.is_valid_index(0));
			OP_REQUIRE(arr[0] == 120);

			Array<int, StackAllocator<120>> arr2;
			arr2 = arr;
			OP_REQUIRE(arr2.is_valid_index(0));
			OP_CHECK(arr2[0] == 120);
			OP_CHECK(&arr[0] != &arr2[0]);
		}

		OP_SUBCASE("move constructor") {
			Array<int, StackAllocator<120>> arr;
			arr.push(120);
			OP_REQUIRE(arr.is_valid_index(0));
			OP_REQUIRE(arr[0] == 120);

			Array<int, StackAllocator<120>> arr2{ op::move(arr) };
			OP_REQUIRE(arr2.is_valid_index(0));
			OP_CHECK(arr2[0] == 120);
		}

		OP_SUBCASE("move assignment") {
			Array<int, StackAllocator<120>> arr;
			arr.push(120);
			OP_REQUIRE(arr.is_valid_index(0));
			OP_REQUIRE(arr[0] == 120);

			Array<int, StackAllocator<120>> arr2;
			arr2 = op::move(arr);
			OP_REQUIRE(arr2.is_valid_index(0));
			OP_CHECK(arr2[0] == 120);
		}

		OP_SUBCASE("move insert") {
			Array<Array<int, StackAllocator<120>>, StackAllocator<120>> arr;

			Array<int, StackAllocator<120>> inner;
			inner.insert(0, 120);

			arr.insert(0, op::move(inner));
			OP_REQUIRE(arr.is_valid_index(0));
			OP_REQUIRE(arr[0].is_valid_index(0));
			OP_CHECK(arr[0][0] == 120);
		}

		OP_SUBCASE("copy insert") {
			Array<int, StackAllocator<120>> arr;
			arr.insert(0, 1);
			OP_REQUIRE(arr.is_valid_index(0));
			OP_CHECK(arr[0] == 1);
		}

		OP_SUBCASE("move push") {
			Array<Array<int, StackAllocator<120>>, StackAllocator<120>> arr;

			Array<int, StackAllocator<120>> inner;
			inner.insert(0, 120);

			arr.push(op::move(inner));
			OP_REQUIRE(arr.is_valid_index(0));
			OP_REQUIRE(arr[0].is_valid_index(0));
			OP_CHECK(arr[0][0] == 120);
		}

		OP_SUBCASE("copy push") {
			Array<int, StackAllocator<120>> arr;
			arr.push(1);
			OP_REQUIRE(arr.len() == 1);
			OP_CHECK(arr[0] == 1);
		}

		OP_SUBCASE("remove") {
			Array<int, StackAllocator<120>> arr;

			arr.insert(0, 1);
			OP_REQUIRE(arr.is_valid_index(0));
			OP_REQUIRE(arr[0] == 1);

			const auto value = arr.remove(0);
			OP_CHECK(value == 1);
			OP_CHECK(arr.len() == 0);
		}

		OP_SUBCASE("move pop") {
			Array<Array<int, StackAllocator<120>>, StackAllocator<120>> arr;

			Array<int, StackAllocator<120>> inner;
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
			Array<int, StackAllocator<120>> arr;

			arr.push(1);
			OP_REQUIRE(arr.is_valid_index(0));
			OP_REQUIRE(arr[0] == 1);

			const auto value = arr.pop();
			OP_REQUIRE(value.is_set());
			OP_CHECK(value.unwrap() == 1);
		}
	}
}