/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Containers/Array.hpp>
#include <Core/Debug/Test.hpp>

#if MACH_ENABLE_TEST
MACH_TEST_SUITE("Containers") {
	using namespace Mach::Core;

	MACH_TEST_CASE("Array<HeapAllocator>") {
		MACH_SUBCASE("default constructor") {
			Array<int> arr;
			MACH_CHECK(arr.len() == 0);
		}

		MACH_SUBCASE("initializer_list") {
			Array<int> arr = { 1, 2, 3, 4, 5 };
			MACH_CHECK(arr.len() == 5);
			MACH_CHECK(arr[0] == 1);
			MACH_CHECK(arr[1] == 2);
			MACH_CHECK(arr[2] == 3);
			MACH_CHECK(arr[3] == 4);
			MACH_CHECK(arr[4] == 5);
		}

		MACH_SUBCASE("slice constructor") {
			Slice<const int> slice = { 1, 2, 3, 4, 5 };
			Array<int> arr{ slice };
			MACH_CHECK(arr.len() == 5);
			MACH_CHECK(arr[0] == 1);
			MACH_CHECK(arr[1] == 2);
			MACH_CHECK(arr[2] == 3);
			MACH_CHECK(arr[3] == 4);
			MACH_CHECK(arr[4] == 5);
		}

		MACH_SUBCASE("copy constructor") {
			Array<int> arr;
			arr.push(120);
			MACH_REQUIRE(arr.is_valid_index(0));
			MACH_REQUIRE(arr[0] == 120);

			Array<int> arr2{ arr };
			MACH_REQUIRE(arr2.is_valid_index(0));
			MACH_CHECK(arr2[0] == 120);
			MACH_CHECK(&arr[0] != &arr2[0]);
		}

		MACH_SUBCASE("copy assignment") {
			Array<int> arr;
			arr.push(120);
			// arr.push(120);
			MACH_REQUIRE(arr.is_valid_index(0));
			MACH_CHECK(arr[0] == 120);

			Array<int> arr2;
			arr2 = arr;
			MACH_REQUIRE(arr2.is_valid_index(0));
			MACH_CHECK(arr2[0] == 120);
			MACH_CHECK(&arr[0] != &arr2[0]);
		}

		MACH_SUBCASE("move constructor") {
			Array<int> arr;
			arr.push(120);
			MACH_REQUIRE(arr.is_valid_index(0));
			MACH_REQUIRE(arr[0] == 120);
			auto* ptr = &arr[0];

			Array<int> arr2{ Mach::move(arr) };
			MACH_REQUIRE(arr2.is_valid_index(0));
			MACH_CHECK(arr2[0] == 120);
			MACH_CHECK(ptr == &arr2[0]);
		}

		MACH_SUBCASE("move assignment") {
			Array<int> arr;
			arr.push(120);
			MACH_REQUIRE(arr.is_valid_index(0));
			MACH_REQUIRE(arr[0] == 120);
			auto* ptr = &arr[0];

			Array<int> arr2;
			arr2 = Mach::move(arr);
			MACH_REQUIRE(arr2.is_valid_index(0));
			MACH_CHECK(arr2[0] == 120);
			MACH_CHECK(ptr == &arr2[0]);
		}

		MACH_SUBCASE("as_slice") {
			Array<int> arr = { 1, 2, 3, 4, 5 };
			auto slice = arr.as_slice();
			MACH_CHECK(slice.len() == 5);
			MACH_CHECK(slice[0] == 1);
			MACH_CHECK(slice[1] == 2);
			MACH_CHECK(slice[2] == 3);
			MACH_CHECK(slice[3] == 4);
			MACH_CHECK(slice[4] == 5);
		}

		MACH_SUBCASE("as_const_slice") {
			Array<int> arr = { 1, 2, 3, 4, 5 };
			auto slice = arr.as_const_slice();
			MACH_CHECK(slice.len() == 5);
			MACH_CHECK(slice[0] == 1);
			MACH_CHECK(slice[1] == 2);
			MACH_CHECK(slice[2] == 3);
			MACH_CHECK(slice[3] == 4);
			MACH_CHECK(slice[4] == 5);
		}

		MACH_SUBCASE("get") {
			Array<int> arr;
			arr.push(120);
			MACH_REQUIRE(arr.is_valid_index(0));
			MACH_CHECK(arr.get(0).is_set());
			MACH_CHECK(arr.get(0).unwrap() == 120);
			MACH_CHECK(!arr.get(1).is_set());
		}

		MACH_SUBCASE("last") {
			Array<int> arr;
			MACH_CHECK(!arr.last().is_set());
			arr.push(120);
			arr.push(121);
			arr.push(122);
			MACH_CHECK(arr.last().is_set());
			MACH_CHECK(arr.last().unwrap() == 122);
		}

		MACH_SUBCASE("reserve") {
			Array<int> arr;
			arr.reserve(10);
			MACH_CHECK(arr.cap() >= 10);
		}

		MACH_SUBCASE("move insert") {
			Array<Array<int>> arr;

			Array<int> inner;
			inner.insert(0, 120);
			auto* ptr = &inner[0];

			arr.insert(0, Mach::move(inner));
			MACH_REQUIRE(arr.is_valid_index(0));
			MACH_REQUIRE(arr[0].is_valid_index(0));
			MACH_CHECK(arr[0][0] == 120);
			MACH_CHECK(&arr[0][0] == ptr);
		}

		MACH_SUBCASE("copy insert") {
			Array<int> arr;
			arr.insert(0, 1);
			MACH_REQUIRE(arr.is_valid_index(0));
			MACH_CHECK(arr[0] == 1);
		}

		MACH_SUBCASE("move push") {
			Array<Array<int>> arr;

			Array<int> inner;
			inner.insert(0, 120);

			arr.push(Mach::move(inner));
			MACH_REQUIRE(arr.is_valid_index(0));
			MACH_REQUIRE(arr[0].is_valid_index(0));
			MACH_CHECK(arr[0][0] == 120);
		}

		MACH_SUBCASE("copy push") {
			Array<int> arr;
			arr.push(1);
			MACH_REQUIRE(arr.len() == 1);
			MACH_CHECK(arr[0] == 1);
		}

		MACH_SUBCASE("remove") {
			Array<int> arr;

			arr.insert(0, 1);
			MACH_REQUIRE(arr.is_valid_index(0));
			MACH_REQUIRE(arr[0] == 1);

			const auto value = arr.remove(0);
			MACH_CHECK(value == 1);
			MACH_CHECK(arr.len() == 0);
		}

		MACH_SUBCASE("move pop") {
			Array<Array<int>> arr;

			Array<int> inner;
			inner.push(120);
			auto* ptr = &inner[0];
			arr.push(Mach::move(inner));

			MACH_REQUIRE(arr.is_valid_index(0));
			MACH_REQUIRE(arr[0].is_valid_index(0));
			MACH_REQUIRE(arr[0][0] == 120);

			auto value = arr.pop();
			MACH_REQUIRE(value.is_set());
			auto unwrapped_inner = value.unwrap();
			MACH_REQUIRE(unwrapped_inner.is_valid_index(0));
			MACH_CHECK(unwrapped_inner[0] == 120);
			MACH_CHECK(ptr == &unwrapped_inner[0]);
		}

		MACH_SUBCASE("copy pop") {
			Array<int> arr;

			arr.push(1);
			MACH_REQUIRE(arr.is_valid_index(0));
			MACH_REQUIRE(arr[0] == 1);

			const auto value = arr.pop();
			MACH_REQUIRE(value.is_set());
			MACH_CHECK(value.unwrap() == 1);
		}

		MACH_SUBCASE("set len") {
			Array<int> arr;
			arr.set_len(10);
			MACH_CHECK(arr.len() == 10);
			for (usize i = 0; i < 10; i++) {
				MACH_CHECK(arr[i] == 0);
			}
		}

		MACH_SUBCASE("set len uninitialized") {
			Array<int> arr;
			arr.set_len_uninitialized(10);
			MACH_CHECK(arr.len() == 10);
		}
	}

	MACH_TEST_CASE("Array<InlineAllocator>") {
		MACH_SUBCASE("default constructor") {
			Array<int, InlineAllocator<120>> arr;
			MACH_CHECK(arr.len() == 0);
		}

		MACH_SUBCASE("initializer list") {
			Array<int, InlineAllocator<16>> arr = { 1, 2, 3, 4, 5 };
			MACH_CHECK(arr.len() == 5);
			MACH_CHECK(arr[0] == 1);
			MACH_CHECK(arr[1] == 2);
			MACH_CHECK(arr[2] == 3);
			MACH_CHECK(arr[3] == 4);
			MACH_CHECK(arr[4] == 5);
		}

		MACH_SUBCASE("copy constructor") {
			Array<int, InlineAllocator<120>> arr;
			arr.push(120);
			MACH_REQUIRE(arr.is_valid_index(0));
			MACH_REQUIRE(arr[0] == 120);

			Array<int, InlineAllocator<120>> arr2{ arr };
			MACH_REQUIRE(arr2.is_valid_index(0));
			MACH_CHECK(arr2[0] == 120);
			MACH_CHECK(&arr[0] != &arr2[0]);
		}

		MACH_SUBCASE("copy assignment") {
			Array<int, InlineAllocator<120>> arr;
			arr.push(120);
			MACH_REQUIRE(arr.is_valid_index(0));
			MACH_REQUIRE(arr[0] == 120);

			Array<int, InlineAllocator<120>> arr2;
			arr2 = arr;
			MACH_REQUIRE(arr2.is_valid_index(0));
			MACH_CHECK(arr2[0] == 120);
			MACH_CHECK(&arr[0] != &arr2[0]);
		}

		MACH_SUBCASE("move constructor") {
			Array<int, InlineAllocator<120>> arr;
			arr.push(120);
			MACH_REQUIRE(arr.is_valid_index(0));
			MACH_REQUIRE(arr[0] == 120);

			Array<int, InlineAllocator<120>> arr2{ Mach::move(arr) };
			MACH_REQUIRE(arr2.is_valid_index(0));
			MACH_CHECK(arr2[0] == 120);
			MACH_CHECK(arr.is_empty());
		}

		MACH_SUBCASE("move assignment") {
			Array<int, InlineAllocator<120>> arr;
			arr.push(120);
			MACH_REQUIRE(arr.is_valid_index(0));
			MACH_REQUIRE(arr[0] == 120);

			Array<int, InlineAllocator<120>> arr2;
			arr2 = Mach::move(arr);
			MACH_REQUIRE(arr2.is_valid_index(0));
			MACH_CHECK(arr2[0] == 120);
			MACH_CHECK(arr.is_empty());
		}

		MACH_SUBCASE("move insert") {
			Array<Array<int, InlineAllocator<120>>, InlineAllocator<120>> arr;

			Array<int, InlineAllocator<120>> inner;
			inner.insert(0, 120);

			arr.insert(0, Mach::move(inner));
			MACH_REQUIRE(arr.is_valid_index(0));
			MACH_REQUIRE(arr[0].is_valid_index(0));
			MACH_CHECK(arr[0][0] == 120);
		}

		MACH_SUBCASE("copy insert") {
			Array<int, InlineAllocator<120>> arr;
			arr.insert(0, 1);
			MACH_REQUIRE(arr.is_valid_index(0));
			MACH_CHECK(arr[0] == 1);
		}

		MACH_SUBCASE("move push") {
			Array<Array<int, InlineAllocator<120>>, InlineAllocator<120>> arr;

			Array<int, InlineAllocator<120>> inner;
			inner.insert(0, 120);

			arr.push(Mach::move(inner));
			MACH_REQUIRE(arr.is_valid_index(0));
			MACH_REQUIRE(arr[0].is_valid_index(0));
			MACH_CHECK(arr[0][0] == 120);
		}

		MACH_SUBCASE("copy push") {
			Array<int, InlineAllocator<120>> arr;
			arr.push(1);
			MACH_REQUIRE(arr.len() == 1);
			MACH_CHECK(arr[0] == 1);
		}

		MACH_SUBCASE("remove") {
			Array<int, InlineAllocator<120>> arr;

			arr.insert(0, 1);
			MACH_REQUIRE(arr.is_valid_index(0));
			MACH_REQUIRE(arr[0] == 1);

			const auto value = arr.remove(0);
			MACH_CHECK(value == 1);
			MACH_CHECK(arr.len() == 0);
		}

		MACH_SUBCASE("move pop") {
			Array<Array<int, InlineAllocator<120>>, InlineAllocator<120>> arr;

			Array<int, InlineAllocator<120>> inner;
			inner.push(120);
			arr.push(Mach::move(inner));

			MACH_REQUIRE(arr.is_valid_index(0));
			MACH_REQUIRE(arr[0].is_valid_index(0));
			MACH_REQUIRE(arr[0][0] == 120);

			auto value = arr.pop();
			MACH_REQUIRE(value.is_set());
			auto unwrapped_inner = value.unwrap();
			MACH_REQUIRE(unwrapped_inner.is_valid_index(0));
			MACH_CHECK(unwrapped_inner[0] == 120);
		}

		MACH_SUBCASE("copy pop") {
			Array<int, InlineAllocator<120>> arr;

			arr.push(1);
			MACH_REQUIRE(arr.is_valid_index(0));
			MACH_REQUIRE(arr[0] == 1);

			const auto value = arr.pop();
			MACH_REQUIRE(value.is_set());
			MACH_CHECK(value.unwrap() == 1);
		}

		MACH_SUBCASE("set len") {
			Array<int, InlineAllocator<120>> arr;
			arr.set_len(10);
			MACH_CHECK(arr.len() == 10);
			for (usize i = 0; i < 10; i++) {
				MACH_CHECK(arr[i] == 0);
			}
		}

		MACH_SUBCASE("set len uninitialized") {
			Array<int, InlineAllocator<120>> arr;
			arr.set_len_uninitialized(10);
			MACH_CHECK(arr.len() == 10);
		}
	}
}
#endif // MACH_ENABLE_TEST
