/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include "core/containers/option.h"
#include "core/containers/array.h"
#include "core/debug/test.h"

using namespace op::core;

OP_TEST_SUITE("containers") {
	OP_TEST_CASE("Option non trivially copyable type") {
		OP_SUBCASE("default constructor") {
			Option<Array<int>> opt;
			OP_CHECK(!opt.is_set());
		}

		OP_SUBCASE("nullopt constructor") {
			Option<Array<int>> opt = nullopt;
			OP_CHECK(!opt.is_set());
		}

		OP_SUBCASE("value move constructor") {
			Array<int> arr;
			arr.push(120);
			arr.push(122);

			Option<Array<int>> opt = op::move(arr);
			OP_REQUIRE(opt.is_set());
			OP_CHECK(opt.as_ref().unwrap()[0] == 120);
			OP_CHECK(opt.as_ref().unwrap()[1] == 122);
		}

		OP_SUBCASE("value move assignment") {
			Array<int> arr;
			arr.push(120);
			arr.push(122);

			Option<Array<int>> opt = nullopt;
			opt = op::move(arr);
			OP_REQUIRE(opt.is_set());
			OP_CHECK(opt.as_ref().unwrap()[0] == 120);
			OP_CHECK(opt.as_ref().unwrap()[1] == 122);
		}

		OP_SUBCASE("value copy constructor") {
			Array<int> arr;
			arr.push(120);
			arr.push(122);
			auto* ptr = &arr[0];

			Option<Array<int>> opt = arr;
			OP_REQUIRE(opt.is_set());
			OP_CHECK(opt.as_ref().unwrap()[0] == 120);
			OP_CHECK(opt.as_ref().unwrap()[1] == 122);
			OP_CHECK(ptr != &opt.as_ref().unwrap()[0]);
		}

		OP_SUBCASE("value copy assignment") {
			Array<int> arr;
			arr.push(120);
			arr.push(122);
			auto* ptr = &arr[0];

			Option<Array<int>> opt = nullopt;
			opt = arr;
			OP_REQUIRE(opt.is_set());
			OP_CHECK(opt.as_ref().unwrap()[0] == 120);
			OP_CHECK(opt.as_ref().unwrap()[1] == 122);
			OP_CHECK(ptr != &opt.as_ref().unwrap()[0]);
		}

		OP_SUBCASE("copy constructor") {
			Array<int> arr;
			arr.push(120);
			arr.push(122);
			Option<Array<int>> opt = op::move(arr);
			OP_REQUIRE(opt.is_set());

			Option<Array<int>> opt2 = opt;
			OP_REQUIRE(opt2.is_set());
			OP_REQUIRE(opt.as_ref().unwrap().len() == opt2.as_ref().unwrap().len());
			OP_CHECK(&opt.as_ref().unwrap()[0] != &opt2.as_ref().unwrap()[0]);
		}

		OP_SUBCASE("copy assignment") {
			Array<int> arr;
			arr.push(120);
			arr.push(122);
			Option<Array<int>> opt = op::move(arr);
			OP_REQUIRE(opt.is_set());

			Option<Array<int>> opt2 = nullopt;
			opt2 = opt;
			OP_REQUIRE(opt2.is_set());
			OP_REQUIRE(opt.as_ref().unwrap().len() == opt2.as_ref().unwrap().len());
			OP_CHECK(&opt.as_ref().unwrap()[0] != &opt2.as_ref().unwrap()[0]);
		}

		OP_SUBCASE("move constructor") {
			Array<int> arr;
			arr.push(120);
			arr.push(122);
			auto* ptr = &arr[0];

			Option<Array<int>> opt = op::move(arr);
			OP_REQUIRE(opt.is_set());
			OP_CHECK(opt.as_ref().unwrap()[0] == 120);
			OP_CHECK(opt.as_ref().unwrap()[1] == 122);
			OP_CHECK(ptr == &opt.as_ref().unwrap()[0]);
		}

		OP_SUBCASE("move assignment") {
			Array<int> arr;
			arr.push(120);
			arr.push(122);
			auto* ptr = &arr[0];

			Option<Array<int>> opt = op::move(arr);
			OP_REQUIRE(opt.is_set());

			Option<Array<int>> opt2 = nullopt;
			opt2 = op::move(opt);
			OP_REQUIRE(opt2.is_set());
			OP_CHECK(opt2.as_ref().unwrap()[0] == 120);
			OP_CHECK(opt2.as_ref().unwrap()[1] == 122);
			OP_CHECK(ptr == &opt2.as_ref().unwrap()[0]);
		}

		OP_SUBCASE("unwrap") {
			Array<int> arr;
			arr.push(120);
			arr.push(122);

			Option<Array<int>> opt = op::move(arr);
			OP_REQUIRE(opt.is_set());
			Array<int> unwrapped = opt.unwrap();
			OP_CHECK(!opt.is_set());
			OP_CHECK(unwrapped[0] == 120);
			OP_CHECK(unwrapped[1] == 122);
		}

		OP_SUBCASE("unwrap_or_default") {
			Option<Array<int>> opt;
			Array<int> unwrapped = opt.unwrap_or_default();
			OP_CHECK(unwrapped.len() == 0);
		}

		OP_SUBCASE("unwrap_or") {
			Option<Array<int>> opt = nullopt;
			OP_REQUIRE(!opt.is_set());
			Array<int> unwrapped = opt.unwrap_or(Array<int>{});
			OP_CHECK(unwrapped.is_empty());
		}

		OP_SUBCASE("as_mut") {
			Array<int> arr;
			arr.push(120);
			arr.push(122);

			Option<Array<int>> opt = op::move(arr);
			OP_REQUIRE(opt.is_set());
			Option<Array<int>&> mut = opt.as_mut();
			OP_REQUIRE(mut.is_set());
			mut.unwrap().push(123);
			OP_CHECK(opt.as_ref().unwrap()[2] == 123);
		}

		OP_SUBCASE("as_ref") {
			Array<int> arr;
			arr.push(120);
			arr.push(122);

			Option<Array<int>> opt = op::move(arr);
			OP_REQUIRE(opt.is_set());
			Option<Array<int> const&> ref = opt.as_ref();
			OP_REQUIRE(ref.is_set());
			OP_CHECK(ref.unwrap()[0] == 120);
			OP_CHECK(ref.unwrap()[1] == 122);
		}
	}

	OP_TEST_CASE("Option with trivially copyable type") {
		OP_SUBCASE("default constructor") {
			Option<int> opt;
			OP_CHECK(!opt.is_set());
		}

		OP_SUBCASE("nullopt constructor") {
			Option<int> opt = nullopt;
			OP_CHECK(!opt.is_set());
		}

		OP_SUBCASE("value copy constructor") {
			int i = 120;
			const Option<int> opt = i;
			OP_REQUIRE(opt.is_set());
			OP_CHECK(opt.unwrap() == 120);
		}

		OP_SUBCASE("value copy assignment") {
			int i = 120;
			Option<int> opt = nullopt;
			opt = i;
			OP_REQUIRE(opt.is_set());
			OP_CHECK(opt.unwrap() == 120);
		}

		OP_SUBCASE("copy constructor") {
			Option<int> opt = 120;
			OP_REQUIRE(opt.is_set());

			Option<int> opt2 = opt;
			OP_REQUIRE(opt2.is_set());
			OP_CHECK(opt.unwrap() == opt2.unwrap());
		}

		OP_SUBCASE("copy assignment") {
			Option<int> opt = 120;
			OP_REQUIRE(opt.is_set());

			Option<int> opt2 = nullopt;
			opt2 = opt;
			OP_REQUIRE(opt2.is_set());
			OP_CHECK(opt.unwrap() == opt2.unwrap());
		}

		OP_SUBCASE("move constructor") {
			Option<int> opt = 120;
			OP_REQUIRE(opt.is_set());

			Option<int> opt2 = op::move(opt);
			OP_REQUIRE(opt2.is_set());
			OP_CHECK(opt2.unwrap() == 120);
		}

		OP_SUBCASE("move assignment") {
			Option<int> opt = 120;
			OP_REQUIRE(opt.is_set());

			Option<int> opt2 = nullopt;
			opt2 = op::move(opt);
			OP_REQUIRE(opt2.is_set());
			OP_CHECK(opt2.unwrap() == 120);
		}

		OP_SUBCASE("unwrap") {
			const Option<int> opt = 120;
			OP_REQUIRE(opt.is_set());
			const int unwrapped = opt.unwrap();
			OP_CHECK(opt.is_set());
			OP_CHECK(unwrapped == 120);
		}

		OP_SUBCASE("unwrap_or_default") {
			Option<int> opt;
			const int unwrapped = opt.unwrap_or_default();
			OP_CHECK(unwrapped == 0);
		}

		OP_SUBCASE("unwrap_or") {
			Option<int> opt = nullopt;
			OP_REQUIRE(!opt.is_set());
			const int unwrapped = opt.unwrap_or(120);
			OP_CHECK(unwrapped == 120);
		}

		OP_SUBCASE("as_mut") {
			Option<int> opt = 120;
			OP_REQUIRE(opt.is_set());
			Option<int&> mut = opt.as_mut();
			OP_REQUIRE(mut.is_set());
			mut.unwrap() = 123;
			OP_CHECK(opt.unwrap() == 123);
		}

		OP_SUBCASE("as_ref") {
			Option<int> opt = 120;
			OP_REQUIRE(opt.is_set());
			Option<int const&> ref = opt.as_ref();
			OP_REQUIRE(ref.is_set());
			OP_CHECK(ref.unwrap() == 120);
		}
	}

	OP_TEST_CASE("Option with reference") {
		OP_SUBCASE("default constructor") {
			Option<int&> opt;
			OP_CHECK(!opt.is_set());
		}

		OP_SUBCASE("nullopt constructor") {
			Option<int&> opt = nullopt;
			OP_CHECK(!opt.is_set());
		}

		OP_SUBCASE("value constructor") {
			int i = 120;
			Option<int&> opt = i;
			OP_REQUIRE(opt.is_set());
			OP_CHECK(opt.unwrap() == 120);
		}

		OP_SUBCASE("copy constructor") {
			int i = 120;
			Option<int&> opt = i;
			OP_REQUIRE(opt.is_set());

			Option<int&> opt2 = opt;
			OP_REQUIRE(opt2.is_set());
			OP_CHECK(opt.unwrap() == opt2.unwrap());
		}

		OP_SUBCASE("copy assignment") {
			int i = 120;
			Option<int&> opt = i;
			OP_REQUIRE(opt.is_set());

			Option<int&> opt2 = nullopt;
			opt2 = opt;
			OP_REQUIRE(opt2.is_set());
			OP_CHECK(opt.unwrap() == opt2.unwrap());
		}

		OP_SUBCASE("move constructor") {
			int i = 120;
			Option<int&> opt = i;
			OP_REQUIRE(opt.is_set());

			Option<int&> opt2 = op::move(opt);
			OP_REQUIRE(opt2.is_set());
			OP_CHECK(opt2.unwrap() == 120);
		}

		OP_SUBCASE("move assignment") {
			int i = 120;
			Option<int&> opt = i;
			OP_REQUIRE(opt.is_set());

			Option<int&> opt2 = nullopt;
			opt2 = op::move(opt);
			OP_REQUIRE(opt2.is_set());
			OP_CHECK(opt2.unwrap() == 120);
		}

		OP_SUBCASE("unwrap") {
			int i = 120;
			Option<int&> opt = i;
			OP_REQUIRE(opt.is_set());
			int& unwrapped = opt.unwrap();
			OP_CHECK(opt.is_set());
			OP_CHECK(unwrapped == 120);
		}
	}
}