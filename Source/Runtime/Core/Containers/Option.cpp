/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Containers/Array.hpp>
#include <Core/Containers/Option.hpp>
#include <Core/Debug/Test.hpp>

#if GRIZZLY_ENABLE_TEST
GRIZZLY_TEST_SUITE("Containers") {
	using namespace Grizzly::Core;

	GRIZZLY_TEST_CASE("Option non trivially copyable type") {
		GRIZZLY_SUBCASE("default constructor") {
			Option<Array<int>> opt;
			GRIZZLY_CHECK(!opt.is_set());
		}

		GRIZZLY_SUBCASE("nullopt constructor") {
			Option<Array<int>> opt = nullopt;
			GRIZZLY_CHECK(!opt.is_set());
		}

		GRIZZLY_SUBCASE("value move constructor") {
			Array<int> arr;
			arr.push(120);
			arr.push(122);

			Option<Array<int>> opt = Grizzly::move(arr);
			GRIZZLY_REQUIRE(opt.is_set());
			GRIZZLY_CHECK(opt.as_const_ref().unwrap()[0] == 120);
			GRIZZLY_CHECK(opt.as_const_ref().unwrap()[1] == 122);
		}

		GRIZZLY_SUBCASE("value move assignment") {
			Array<int> arr;
			arr.push(120);
			arr.push(122);

			Option<Array<int>> opt = nullopt;
			opt = Grizzly::move(arr);
			GRIZZLY_REQUIRE(opt.is_set());
			GRIZZLY_CHECK(opt.as_const_ref().unwrap()[0] == 120);
			GRIZZLY_CHECK(opt.as_const_ref().unwrap()[1] == 122);
		}

		GRIZZLY_SUBCASE("value copy constructor") {
			Array<int> arr;
			arr.push(120);
			arr.push(122);
			auto* ptr = &arr[0];

			Option<Array<int>> opt = arr;
			GRIZZLY_REQUIRE(opt.is_set());
			GRIZZLY_CHECK(opt.as_const_ref().unwrap()[0] == 120);
			GRIZZLY_CHECK(opt.as_const_ref().unwrap()[1] == 122);
			GRIZZLY_CHECK(ptr != &opt.as_const_ref().unwrap()[0]);
		}

		GRIZZLY_SUBCASE("value copy assignment") {
			Array<int> arr;
			arr.push(120);
			arr.push(122);
			auto* ptr = &arr[0];

			Option<Array<int>> opt = nullopt;
			opt = arr;
			GRIZZLY_REQUIRE(opt.is_set());
			GRIZZLY_CHECK(opt.as_const_ref().unwrap()[0] == 120);
			GRIZZLY_CHECK(opt.as_const_ref().unwrap()[1] == 122);
			GRIZZLY_CHECK(ptr != &opt.as_const_ref().unwrap()[0]);
		}

		GRIZZLY_SUBCASE("copy constructor") {
			Array<int> arr;
			arr.push(120);
			arr.push(122);
			Option<Array<int>> opt = Grizzly::move(arr);
			GRIZZLY_REQUIRE(opt.is_set());

			Option<Array<int>> opt2 = opt;
			GRIZZLY_REQUIRE(opt2.is_set());
			GRIZZLY_REQUIRE(opt.as_const_ref().unwrap().len() == opt2.as_const_ref().unwrap().len());
			GRIZZLY_CHECK(&opt.as_const_ref().unwrap()[0] != &opt2.as_const_ref().unwrap()[0]);
		}

		GRIZZLY_SUBCASE("copy assignment") {
			Array<int> arr;
			arr.push(120);
			arr.push(122);
			Option<Array<int>> opt = Grizzly::move(arr);
			GRIZZLY_REQUIRE(opt.is_set());

			Option<Array<int>> opt2 = nullopt;
			opt2 = opt;
			GRIZZLY_REQUIRE(opt2.is_set());
			GRIZZLY_REQUIRE(opt.as_const_ref().unwrap().len() == opt2.as_const_ref().unwrap().len());
			GRIZZLY_CHECK(&opt.as_const_ref().unwrap()[0] != &opt2.as_const_ref().unwrap()[0]);
		}

		GRIZZLY_SUBCASE("move constructor") {
			Array<int> arr;
			arr.push(120);
			arr.push(122);
			auto* ptr = &arr[0];

			Option<Array<int>> opt = Grizzly::move(arr);
			GRIZZLY_REQUIRE(opt.is_set());
			GRIZZLY_CHECK(opt.as_const_ref().unwrap()[0] == 120);
			GRIZZLY_CHECK(opt.as_const_ref().unwrap()[1] == 122);
			GRIZZLY_CHECK(ptr == &opt.as_const_ref().unwrap()[0]);
		}

		GRIZZLY_SUBCASE("move assignment") {
			Array<int> arr;
			arr.push(120);
			arr.push(122);
			auto* ptr = &arr[0];

			Option<Array<int>> opt = Grizzly::move(arr);
			GRIZZLY_REQUIRE(opt.is_set());

			Option<Array<int>> opt2 = nullopt;
			opt2 = Grizzly::move(opt);
			GRIZZLY_REQUIRE(opt2.is_set());
			GRIZZLY_CHECK(opt2.as_const_ref().unwrap()[0] == 120);
			GRIZZLY_CHECK(opt2.as_const_ref().unwrap()[1] == 122);
			GRIZZLY_CHECK(ptr == &opt2.as_const_ref().unwrap()[0]);
		}

		GRIZZLY_SUBCASE("unwrap") {
			Array<int> arr;
			arr.push(120);
			arr.push(122);

			Option<Array<int>> opt = Grizzly::move(arr);
			GRIZZLY_REQUIRE(opt.is_set());
			Array<int> unwrapped = opt.unwrap();
			GRIZZLY_CHECK(!opt.is_set());
			GRIZZLY_CHECK(unwrapped[0] == 120);
			GRIZZLY_CHECK(unwrapped[1] == 122);
		}

		GRIZZLY_SUBCASE("unwrap_or_default") {
			Option<Array<int>> opt;
			Array<int> unwrapped = opt.unwrap_or_default();
			GRIZZLY_CHECK(unwrapped.len() == 0);
		}

		GRIZZLY_SUBCASE("unwrap_or") {
			Option<Array<int>> opt = nullopt;
			GRIZZLY_REQUIRE(!opt.is_set());
			Array<int> unwrapped = opt.unwrap_or(Array<int>{});
			GRIZZLY_CHECK(unwrapped.is_empty());
		}

		GRIZZLY_SUBCASE("as_ref") {
			Array<int> arr;
			arr.push(120);
			arr.push(122);

			Option<Array<int>> opt = Grizzly::move(arr);
			GRIZZLY_REQUIRE(opt.is_set());
			Option<Array<int>&> mut = opt.as_ref();
			GRIZZLY_REQUIRE(mut.is_set());
			mut.unwrap().push(123);
			GRIZZLY_CHECK(opt.as_const_ref().unwrap()[2] == 123);
		}

		GRIZZLY_SUBCASE("as_const_ref") {
			Array<int> arr;
			arr.push(120);
			arr.push(122);

			Option<Array<int>> opt = Grizzly::move(arr);
			GRIZZLY_REQUIRE(opt.is_set());
			Option<Array<int> const&> ref = opt.as_const_ref();
			GRIZZLY_REQUIRE(ref.is_set());
			GRIZZLY_CHECK(ref.unwrap()[0] == 120);
			GRIZZLY_CHECK(ref.unwrap()[1] == 122);
		}
	}

	GRIZZLY_TEST_CASE("Option with trivially copyable type") {
		GRIZZLY_SUBCASE("default constructor") {
			Option<int> opt;
			GRIZZLY_CHECK(!opt.is_set());
		}

		GRIZZLY_SUBCASE("nullopt constructor") {
			Option<int> opt = nullopt;
			GRIZZLY_CHECK(!opt.is_set());
		}

		GRIZZLY_SUBCASE("value copy constructor") {
			int i = 120;
			const Option<int> opt = i;
			GRIZZLY_REQUIRE(opt.is_set());
			GRIZZLY_CHECK(opt.unwrap() == 120);
		}

		GRIZZLY_SUBCASE("value copy assignment") {
			int i = 120;
			Option<int> opt = nullopt;
			opt = i;
			GRIZZLY_REQUIRE(opt.is_set());
			GRIZZLY_CHECK(opt.unwrap() == 120);
		}

		GRIZZLY_SUBCASE("copy constructor") {
			Option<int> opt = 120;
			GRIZZLY_REQUIRE(opt.is_set());

			Option<int> opt2 = opt;
			GRIZZLY_REQUIRE(opt2.is_set());
			GRIZZLY_CHECK(opt.unwrap() == opt2.unwrap());
		}

		GRIZZLY_SUBCASE("copy assignment") {
			Option<int> opt = 120;
			GRIZZLY_REQUIRE(opt.is_set());

			Option<int> opt2 = nullopt;
			opt2 = opt;
			GRIZZLY_REQUIRE(opt2.is_set());
			GRIZZLY_CHECK(opt.unwrap() == opt2.unwrap());
		}

		GRIZZLY_SUBCASE("move constructor") {
			Option<int> opt = 120;
			GRIZZLY_REQUIRE(opt.is_set());

			Option<int> opt2 = Grizzly::move(opt);
			GRIZZLY_REQUIRE(opt2.is_set());
			GRIZZLY_CHECK(opt2.unwrap() == 120);
		}

		GRIZZLY_SUBCASE("move assignment") {
			Option<int> opt = 120;
			GRIZZLY_REQUIRE(opt.is_set());

			Option<int> opt2 = nullopt;
			opt2 = Grizzly::move(opt);
			GRIZZLY_REQUIRE(opt2.is_set());
			GRIZZLY_CHECK(opt2.unwrap() == 120);
		}

		GRIZZLY_SUBCASE("unwrap") {
			const Option<int> opt = 120;
			GRIZZLY_REQUIRE(opt.is_set());
			const int unwrapped = opt.unwrap();
			GRIZZLY_CHECK(opt.is_set());
			GRIZZLY_CHECK(unwrapped == 120);
		}

		GRIZZLY_SUBCASE("unwrap_or_default") {
			Option<int> opt;
			const int unwrapped = opt.unwrap_or_default();
			GRIZZLY_CHECK(unwrapped == 0);
		}

		GRIZZLY_SUBCASE("unwrap_or") {
			Option<int> opt = nullopt;
			GRIZZLY_REQUIRE(!opt.is_set());
			const int unwrapped = opt.unwrap_or(120);
			GRIZZLY_CHECK(unwrapped == 120);
		}

		GRIZZLY_SUBCASE("as_ref") {
			Option<int> opt = 120;
			GRIZZLY_REQUIRE(opt.is_set());
			Option<int&> mut = opt.as_ref();
			GRIZZLY_REQUIRE(mut.is_set());
			mut.unwrap() = 123;
			GRIZZLY_CHECK(opt.unwrap() == 123);
		}

		GRIZZLY_SUBCASE("as_const_ref") {
			Option<int> opt = 120;
			GRIZZLY_REQUIRE(opt.is_set());
			Option<int const&> ref = opt.as_const_ref();
			GRIZZLY_REQUIRE(ref.is_set());
			GRIZZLY_CHECK(ref.unwrap() == 120);
		}
	}

	GRIZZLY_TEST_CASE("Option with reference") {
		GRIZZLY_SUBCASE("default constructor") {
			Option<int&> opt;
			GRIZZLY_CHECK(!opt.is_set());
		}

		GRIZZLY_SUBCASE("nullopt constructor") {
			Option<int&> opt = nullopt;
			GRIZZLY_CHECK(!opt.is_set());
		}

		GRIZZLY_SUBCASE("value constructor") {
			int i = 120;
			Option<int&> opt = i;
			GRIZZLY_REQUIRE(opt.is_set());
			GRIZZLY_CHECK(opt.unwrap() == 120);
		}

		GRIZZLY_SUBCASE("copy constructor") {
			int i = 120;
			Option<int&> opt = i;
			GRIZZLY_REQUIRE(opt.is_set());

			Option<int&> opt2 = opt;
			GRIZZLY_REQUIRE(opt2.is_set());
			GRIZZLY_CHECK(opt.unwrap() == opt2.unwrap());
		}

		GRIZZLY_SUBCASE("copy assignment") {
			int i = 120;
			Option<int&> opt = i;
			GRIZZLY_REQUIRE(opt.is_set());

			Option<int&> opt2 = nullopt;
			opt2 = opt;
			GRIZZLY_REQUIRE(opt2.is_set());
			GRIZZLY_CHECK(opt.unwrap() == opt2.unwrap());
		}

		GRIZZLY_SUBCASE("move constructor") {
			int i = 120;
			Option<int&> opt = i;
			GRIZZLY_REQUIRE(opt.is_set());

			Option<int&> opt2 = Grizzly::move(opt);
			GRIZZLY_REQUIRE(opt2.is_set());
			GRIZZLY_CHECK(opt2.unwrap() == 120);
		}

		GRIZZLY_SUBCASE("move assignment") {
			int i = 120;
			Option<int&> opt = i;
			GRIZZLY_REQUIRE(opt.is_set());

			Option<int&> opt2 = nullopt;
			opt2 = Grizzly::move(opt);
			GRIZZLY_REQUIRE(opt2.is_set());
			GRIZZLY_CHECK(opt2.unwrap() == 120);
		}

		GRIZZLY_SUBCASE("unwrap") {
			int i = 120;
			Option<int&> opt = i;
			GRIZZLY_REQUIRE(opt.is_set());
			int& unwrapped = opt.unwrap();
			GRIZZLY_CHECK(opt.is_set());
			GRIZZLY_CHECK(unwrapped == 120);
		}
	}
}
#endif // GRIZZLY_ENABLE_TEST
