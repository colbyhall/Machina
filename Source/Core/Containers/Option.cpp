/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Containers/Array.hpp>
#include <Core/Containers/Option.hpp>
#include <Core/Debug/Test.hpp>

#if MACH_ENABLE_TEST
MACH_TEST_SUITE("Containers") {
	using namespace Mach::Core;

	MACH_TEST_CASE("Option non trivially copyable type") {
		MACH_SUBCASE("default constructor") {
			Option<Array<int>> opt;
			MACH_CHECK(!opt.is_set());
		}

		MACH_SUBCASE("nullopt constructor") {
			Option<Array<int>> opt = nullopt;
			MACH_CHECK(!opt.is_set());
		}

		MACH_SUBCASE("value move constructor") {
			Array<int> arr;
			arr.push(120);
			arr.push(122);

			Option<Array<int>> opt = Mach::move(arr);
			MACH_REQUIRE(opt.is_set());
			MACH_CHECK(opt.as_const_ref().unwrap()[0] == 120);
			MACH_CHECK(opt.as_const_ref().unwrap()[1] == 122);
		}

		MACH_SUBCASE("value move assignment") {
			Array<int> arr;
			arr.push(120);
			arr.push(122);

			Option<Array<int>> opt = nullopt;
			opt = Mach::move(arr);
			MACH_REQUIRE(opt.is_set());
			MACH_CHECK(opt.as_const_ref().unwrap()[0] == 120);
			MACH_CHECK(opt.as_const_ref().unwrap()[1] == 122);
		}

		MACH_SUBCASE("value copy constructor") {
			Array<int> arr;
			arr.push(120);
			arr.push(122);
			auto* ptr = &arr[0];

			Option<Array<int>> opt = arr;
			MACH_REQUIRE(opt.is_set());
			MACH_CHECK(opt.as_const_ref().unwrap()[0] == 120);
			MACH_CHECK(opt.as_const_ref().unwrap()[1] == 122);
			MACH_CHECK(ptr != &opt.as_const_ref().unwrap()[0]);
		}

		MACH_SUBCASE("value copy assignment") {
			Array<int> arr;
			arr.push(120);
			arr.push(122);
			auto* ptr = &arr[0];

			Option<Array<int>> opt = nullopt;
			opt = arr;
			MACH_REQUIRE(opt.is_set());
			MACH_CHECK(opt.as_const_ref().unwrap()[0] == 120);
			MACH_CHECK(opt.as_const_ref().unwrap()[1] == 122);
			MACH_CHECK(ptr != &opt.as_const_ref().unwrap()[0]);
		}

		MACH_SUBCASE("copy constructor") {
			Array<int> arr;
			arr.push(120);
			arr.push(122);
			Option<Array<int>> opt = Mach::move(arr);
			MACH_REQUIRE(opt.is_set());

			Option<Array<int>> opt2 = opt;
			MACH_REQUIRE(opt2.is_set());
			MACH_REQUIRE(opt.as_const_ref().unwrap().len() == opt2.as_const_ref().unwrap().len());
			MACH_CHECK(&opt.as_const_ref().unwrap()[0] != &opt2.as_const_ref().unwrap()[0]);
		}

		MACH_SUBCASE("copy assignment") {
			Array<int> arr;
			arr.push(120);
			arr.push(122);
			Option<Array<int>> opt = Mach::move(arr);
			MACH_REQUIRE(opt.is_set());

			Option<Array<int>> opt2 = nullopt;
			opt2 = opt;
			MACH_REQUIRE(opt2.is_set());
			MACH_REQUIRE(opt.as_const_ref().unwrap().len() == opt2.as_const_ref().unwrap().len());
			MACH_CHECK(&opt.as_const_ref().unwrap()[0] != &opt2.as_const_ref().unwrap()[0]);
		}

		MACH_SUBCASE("move constructor") {
			Array<int> arr;
			arr.push(120);
			arr.push(122);
			auto* ptr = &arr[0];

			Option<Array<int>> opt = Mach::move(arr);
			MACH_REQUIRE(opt.is_set());
			MACH_CHECK(opt.as_const_ref().unwrap()[0] == 120);
			MACH_CHECK(opt.as_const_ref().unwrap()[1] == 122);
			MACH_CHECK(ptr == &opt.as_const_ref().unwrap()[0]);
		}

		MACH_SUBCASE("move assignment") {
			Array<int> arr;
			arr.push(120);
			arr.push(122);
			auto* ptr = &arr[0];

			Option<Array<int>> opt = Mach::move(arr);
			MACH_REQUIRE(opt.is_set());

			Option<Array<int>> opt2 = nullopt;
			opt2 = Mach::move(opt);
			MACH_REQUIRE(opt2.is_set());
			MACH_CHECK(opt2.as_const_ref().unwrap()[0] == 120);
			MACH_CHECK(opt2.as_const_ref().unwrap()[1] == 122);
			MACH_CHECK(ptr == &opt2.as_const_ref().unwrap()[0]);
		}

		MACH_SUBCASE("unwrap") {
			Array<int> arr;
			arr.push(120);
			arr.push(122);

			Option<Array<int>> opt = Mach::move(arr);
			MACH_REQUIRE(opt.is_set());
			Array<int> unwrapped = opt.unwrap();
			MACH_CHECK(!opt.is_set());
			MACH_CHECK(unwrapped[0] == 120);
			MACH_CHECK(unwrapped[1] == 122);
		}

		MACH_SUBCASE("unwrap_or_default") {
			Option<Array<int>> opt;
			Array<int> unwrapped = opt.unwrap_or_default();
			MACH_CHECK(unwrapped.len() == 0);
		}

		MACH_SUBCASE("unwrap_or") {
			Option<Array<int>> opt = nullopt;
			MACH_REQUIRE(!opt.is_set());
			Array<int> unwrapped = opt.unwrap_or(Array<int>{});
			MACH_CHECK(unwrapped.is_empty());
		}

		MACH_SUBCASE("as_ref") {
			Array<int> arr;
			arr.push(120);
			arr.push(122);

			Option<Array<int>> opt = Mach::move(arr);
			MACH_REQUIRE(opt.is_set());
			Option<Array<int>&> mut = opt.as_ref();
			MACH_REQUIRE(mut.is_set());
			mut.unwrap().push(123);
			MACH_CHECK(opt.as_const_ref().unwrap()[2] == 123);
		}

		MACH_SUBCASE("as_const_ref") {
			Array<int> arr;
			arr.push(120);
			arr.push(122);

			Option<Array<int>> opt = Mach::move(arr);
			MACH_REQUIRE(opt.is_set());
			Option<Array<int> const&> ref = opt.as_const_ref();
			MACH_REQUIRE(ref.is_set());
			MACH_CHECK(ref.unwrap()[0] == 120);
			MACH_CHECK(ref.unwrap()[1] == 122);
		}
	}

	MACH_TEST_CASE("Option with trivially copyable type") {
		MACH_SUBCASE("default constructor") {
			Option<int> opt;
			MACH_CHECK(!opt.is_set());
		}

		MACH_SUBCASE("nullopt constructor") {
			Option<int> opt = nullopt;
			MACH_CHECK(!opt.is_set());
		}

		MACH_SUBCASE("value copy constructor") {
			int i = 120;
			const Option<int> opt = i;
			MACH_REQUIRE(opt.is_set());
			MACH_CHECK(opt.unwrap() == 120);
		}

		MACH_SUBCASE("value copy assignment") {
			int i = 120;
			Option<int> opt = nullopt;
			opt = i;
			MACH_REQUIRE(opt.is_set());
			MACH_CHECK(opt.unwrap() == 120);
		}

		MACH_SUBCASE("copy constructor") {
			Option<int> opt = 120;
			MACH_REQUIRE(opt.is_set());

			Option<int> opt2 = opt;
			MACH_REQUIRE(opt2.is_set());
			MACH_CHECK(opt.unwrap() == opt2.unwrap());
		}

		MACH_SUBCASE("copy assignment") {
			Option<int> opt = 120;
			MACH_REQUIRE(opt.is_set());

			Option<int> opt2 = nullopt;
			opt2 = opt;
			MACH_REQUIRE(opt2.is_set());
			MACH_CHECK(opt.unwrap() == opt2.unwrap());
		}

		MACH_SUBCASE("move constructor") {
			Option<int> opt = 120;
			MACH_REQUIRE(opt.is_set());

			Option<int> opt2 = Mach::move(opt);
			MACH_REQUIRE(opt2.is_set());
			MACH_CHECK(opt2.unwrap() == 120);
		}

		MACH_SUBCASE("move assignment") {
			Option<int> opt = 120;
			MACH_REQUIRE(opt.is_set());

			Option<int> opt2 = nullopt;
			opt2 = Mach::move(opt);
			MACH_REQUIRE(opt2.is_set());
			MACH_CHECK(opt2.unwrap() == 120);
		}

		MACH_SUBCASE("unwrap") {
			const Option<int> opt = 120;
			MACH_REQUIRE(opt.is_set());
			const int unwrapped = opt.unwrap();
			MACH_CHECK(opt.is_set());
			MACH_CHECK(unwrapped == 120);
		}

		MACH_SUBCASE("unwrap_or_default") {
			Option<int> opt;
			const int unwrapped = opt.unwrap_or_default();
			MACH_CHECK(unwrapped == 0);
		}

		MACH_SUBCASE("unwrap_or") {
			Option<int> opt = nullopt;
			MACH_REQUIRE(!opt.is_set());
			const int unwrapped = opt.unwrap_or(120);
			MACH_CHECK(unwrapped == 120);
		}

		MACH_SUBCASE("as_ref") {
			Option<int> opt = 120;
			MACH_REQUIRE(opt.is_set());
			Option<int&> mut = opt.as_ref();
			MACH_REQUIRE(mut.is_set());
			mut.unwrap() = 123;
			MACH_CHECK(opt.unwrap() == 123);
		}

		MACH_SUBCASE("as_const_ref") {
			Option<int> opt = 120;
			MACH_REQUIRE(opt.is_set());
			Option<int const&> ref = opt.as_const_ref();
			MACH_REQUIRE(ref.is_set());
			MACH_CHECK(ref.unwrap() == 120);
		}
	}

	MACH_TEST_CASE("Option with reference") {
		MACH_SUBCASE("default constructor") {
			Option<int&> opt;
			MACH_CHECK(!opt.is_set());
		}

		MACH_SUBCASE("nullopt constructor") {
			Option<int&> opt = nullopt;
			MACH_CHECK(!opt.is_set());
		}

		MACH_SUBCASE("value constructor") {
			int i = 120;
			Option<int&> opt = i;
			MACH_REQUIRE(opt.is_set());
			MACH_CHECK(opt.unwrap() == 120);
		}

		MACH_SUBCASE("copy constructor") {
			int i = 120;
			Option<int&> opt = i;
			MACH_REQUIRE(opt.is_set());

			Option<int&> opt2 = opt;
			MACH_REQUIRE(opt2.is_set());
			MACH_CHECK(opt.unwrap() == opt2.unwrap());
		}

		MACH_SUBCASE("copy assignment") {
			int i = 120;
			Option<int&> opt = i;
			MACH_REQUIRE(opt.is_set());

			Option<int&> opt2 = nullopt;
			opt2 = opt;
			MACH_REQUIRE(opt2.is_set());
			MACH_CHECK(opt.unwrap() == opt2.unwrap());
		}

		MACH_SUBCASE("move constructor") {
			int i = 120;
			Option<int&> opt = i;
			MACH_REQUIRE(opt.is_set());

			Option<int&> opt2 = Mach::move(opt);
			MACH_REQUIRE(opt2.is_set());
			MACH_CHECK(opt2.unwrap() == 120);
		}

		MACH_SUBCASE("move assignment") {
			int i = 120;
			Option<int&> opt = i;
			MACH_REQUIRE(opt.is_set());

			Option<int&> opt2 = nullopt;
			opt2 = Mach::move(opt);
			MACH_REQUIRE(opt2.is_set());
			MACH_CHECK(opt2.unwrap() == 120);
		}

		MACH_SUBCASE("unwrap") {
			int i = 120;
			Option<int&> opt = i;
			MACH_REQUIRE(opt.is_set());
			int& unwrapped = opt.unwrap();
			MACH_CHECK(opt.is_set());
			MACH_CHECK(unwrapped == 120);
		}
	}
}
#endif // MACH_ENABLE_TEST
