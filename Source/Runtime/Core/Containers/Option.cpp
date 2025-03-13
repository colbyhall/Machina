/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Containers/Array.hpp>
#include <Core/Containers/Option.hpp>
#include <Core/Debug/Test.hpp>

#if FORGE_ENABLE_TEST
FORGE_TEST_SUITE("Containers") {
	using namespace Forge::Core;

	FORGE_TEST_CASE("Option non trivially copyable type") {
		FORGE_SUBCASE("default constructor") {
			Option<Array<int>> opt;
			FORGE_CHECK(!opt.is_set());
		}

		FORGE_SUBCASE("nullopt constructor") {
			Option<Array<int>> opt = nullopt;
			FORGE_CHECK(!opt.is_set());
		}

		FORGE_SUBCASE("value move constructor") {
			Array<int> arr;
			arr.push(120);
			arr.push(122);

			Option<Array<int>> opt = Forge::move(arr);
			FORGE_REQUIRE(opt.is_set());
			FORGE_CHECK(opt.as_const_ref().unwrap()[0] == 120);
			FORGE_CHECK(opt.as_const_ref().unwrap()[1] == 122);
		}

		FORGE_SUBCASE("value move assignment") {
			Array<int> arr;
			arr.push(120);
			arr.push(122);

			Option<Array<int>> opt = nullopt;
			opt = Forge::move(arr);
			FORGE_REQUIRE(opt.is_set());
			FORGE_CHECK(opt.as_const_ref().unwrap()[0] == 120);
			FORGE_CHECK(opt.as_const_ref().unwrap()[1] == 122);
		}

		FORGE_SUBCASE("value copy constructor") {
			Array<int> arr;
			arr.push(120);
			arr.push(122);
			auto* ptr = &arr[0];

			Option<Array<int>> opt = arr;
			FORGE_REQUIRE(opt.is_set());
			FORGE_CHECK(opt.as_const_ref().unwrap()[0] == 120);
			FORGE_CHECK(opt.as_const_ref().unwrap()[1] == 122);
			FORGE_CHECK(ptr != &opt.as_const_ref().unwrap()[0]);
		}

		FORGE_SUBCASE("value copy assignment") {
			Array<int> arr;
			arr.push(120);
			arr.push(122);
			auto* ptr = &arr[0];

			Option<Array<int>> opt = nullopt;
			opt = arr;
			FORGE_REQUIRE(opt.is_set());
			FORGE_CHECK(opt.as_const_ref().unwrap()[0] == 120);
			FORGE_CHECK(opt.as_const_ref().unwrap()[1] == 122);
			FORGE_CHECK(ptr != &opt.as_const_ref().unwrap()[0]);
		}

		FORGE_SUBCASE("copy constructor") {
			Array<int> arr;
			arr.push(120);
			arr.push(122);
			Option<Array<int>> opt = Forge::move(arr);
			FORGE_REQUIRE(opt.is_set());

			Option<Array<int>> opt2 = opt;
			FORGE_REQUIRE(opt2.is_set());
			FORGE_REQUIRE(opt.as_const_ref().unwrap().len() == opt2.as_const_ref().unwrap().len());
			FORGE_CHECK(&opt.as_const_ref().unwrap()[0] != &opt2.as_const_ref().unwrap()[0]);
		}

		FORGE_SUBCASE("copy assignment") {
			Array<int> arr;
			arr.push(120);
			arr.push(122);
			Option<Array<int>> opt = Forge::move(arr);
			FORGE_REQUIRE(opt.is_set());

			Option<Array<int>> opt2 = nullopt;
			opt2 = opt;
			FORGE_REQUIRE(opt2.is_set());
			FORGE_REQUIRE(opt.as_const_ref().unwrap().len() == opt2.as_const_ref().unwrap().len());
			FORGE_CHECK(&opt.as_const_ref().unwrap()[0] != &opt2.as_const_ref().unwrap()[0]);
		}

		FORGE_SUBCASE("move constructor") {
			Array<int> arr;
			arr.push(120);
			arr.push(122);
			auto* ptr = &arr[0];

			Option<Array<int>> opt = Forge::move(arr);
			FORGE_REQUIRE(opt.is_set());
			FORGE_CHECK(opt.as_const_ref().unwrap()[0] == 120);
			FORGE_CHECK(opt.as_const_ref().unwrap()[1] == 122);
			FORGE_CHECK(ptr == &opt.as_const_ref().unwrap()[0]);
		}

		FORGE_SUBCASE("move assignment") {
			Array<int> arr;
			arr.push(120);
			arr.push(122);
			auto* ptr = &arr[0];

			Option<Array<int>> opt = Forge::move(arr);
			FORGE_REQUIRE(opt.is_set());

			Option<Array<int>> opt2 = nullopt;
			opt2 = Forge::move(opt);
			FORGE_REQUIRE(opt2.is_set());
			FORGE_CHECK(opt2.as_const_ref().unwrap()[0] == 120);
			FORGE_CHECK(opt2.as_const_ref().unwrap()[1] == 122);
			FORGE_CHECK(ptr == &opt2.as_const_ref().unwrap()[0]);
		}

		FORGE_SUBCASE("unwrap") {
			Array<int> arr;
			arr.push(120);
			arr.push(122);

			Option<Array<int>> opt = Forge::move(arr);
			FORGE_REQUIRE(opt.is_set());
			Array<int> unwrapped = opt.unwrap();
			FORGE_CHECK(!opt.is_set());
			FORGE_CHECK(unwrapped[0] == 120);
			FORGE_CHECK(unwrapped[1] == 122);
		}

		FORGE_SUBCASE("unwrap_or_default") {
			Option<Array<int>> opt;
			Array<int> unwrapped = opt.unwrap_or_default();
			FORGE_CHECK(unwrapped.len() == 0);
		}

		FORGE_SUBCASE("unwrap_or") {
			Option<Array<int>> opt = nullopt;
			FORGE_REQUIRE(!opt.is_set());
			Array<int> unwrapped = opt.unwrap_or(Array<int>{});
			FORGE_CHECK(unwrapped.is_empty());
		}

		FORGE_SUBCASE("as_ref") {
			Array<int> arr;
			arr.push(120);
			arr.push(122);

			Option<Array<int>> opt = Forge::move(arr);
			FORGE_REQUIRE(opt.is_set());
			Option<Array<int>&> mut = opt.as_ref();
			FORGE_REQUIRE(mut.is_set());
			mut.unwrap().push(123);
			FORGE_CHECK(opt.as_const_ref().unwrap()[2] == 123);
		}

		FORGE_SUBCASE("as_const_ref") {
			Array<int> arr;
			arr.push(120);
			arr.push(122);

			Option<Array<int>> opt = Forge::move(arr);
			FORGE_REQUIRE(opt.is_set());
			Option<Array<int> const&> ref = opt.as_const_ref();
			FORGE_REQUIRE(ref.is_set());
			FORGE_CHECK(ref.unwrap()[0] == 120);
			FORGE_CHECK(ref.unwrap()[1] == 122);
		}
	}

	FORGE_TEST_CASE("Option with trivially copyable type") {
		FORGE_SUBCASE("default constructor") {
			Option<int> opt;
			FORGE_CHECK(!opt.is_set());
		}

		FORGE_SUBCASE("nullopt constructor") {
			Option<int> opt = nullopt;
			FORGE_CHECK(!opt.is_set());
		}

		FORGE_SUBCASE("value copy constructor") {
			int i = 120;
			const Option<int> opt = i;
			FORGE_REQUIRE(opt.is_set());
			FORGE_CHECK(opt.unwrap() == 120);
		}

		FORGE_SUBCASE("value copy assignment") {
			int i = 120;
			Option<int> opt = nullopt;
			opt = i;
			FORGE_REQUIRE(opt.is_set());
			FORGE_CHECK(opt.unwrap() == 120);
		}

		FORGE_SUBCASE("copy constructor") {
			Option<int> opt = 120;
			FORGE_REQUIRE(opt.is_set());

			Option<int> opt2 = opt;
			FORGE_REQUIRE(opt2.is_set());
			FORGE_CHECK(opt.unwrap() == opt2.unwrap());
		}

		FORGE_SUBCASE("copy assignment") {
			Option<int> opt = 120;
			FORGE_REQUIRE(opt.is_set());

			Option<int> opt2 = nullopt;
			opt2 = opt;
			FORGE_REQUIRE(opt2.is_set());
			FORGE_CHECK(opt.unwrap() == opt2.unwrap());
		}

		FORGE_SUBCASE("move constructor") {
			Option<int> opt = 120;
			FORGE_REQUIRE(opt.is_set());

			Option<int> opt2 = Forge::move(opt);
			FORGE_REQUIRE(opt2.is_set());
			FORGE_CHECK(opt2.unwrap() == 120);
		}

		FORGE_SUBCASE("move assignment") {
			Option<int> opt = 120;
			FORGE_REQUIRE(opt.is_set());

			Option<int> opt2 = nullopt;
			opt2 = Forge::move(opt);
			FORGE_REQUIRE(opt2.is_set());
			FORGE_CHECK(opt2.unwrap() == 120);
		}

		FORGE_SUBCASE("unwrap") {
			const Option<int> opt = 120;
			FORGE_REQUIRE(opt.is_set());
			const int unwrapped = opt.unwrap();
			FORGE_CHECK(opt.is_set());
			FORGE_CHECK(unwrapped == 120);
		}

		FORGE_SUBCASE("unwrap_or_default") {
			Option<int> opt;
			const int unwrapped = opt.unwrap_or_default();
			FORGE_CHECK(unwrapped == 0);
		}

		FORGE_SUBCASE("unwrap_or") {
			Option<int> opt = nullopt;
			FORGE_REQUIRE(!opt.is_set());
			const int unwrapped = opt.unwrap_or(120);
			FORGE_CHECK(unwrapped == 120);
		}

		FORGE_SUBCASE("as_ref") {
			Option<int> opt = 120;
			FORGE_REQUIRE(opt.is_set());
			Option<int&> mut = opt.as_ref();
			FORGE_REQUIRE(mut.is_set());
			mut.unwrap() = 123;
			FORGE_CHECK(opt.unwrap() == 123);
		}

		FORGE_SUBCASE("as_const_ref") {
			Option<int> opt = 120;
			FORGE_REQUIRE(opt.is_set());
			Option<int const&> ref = opt.as_const_ref();
			FORGE_REQUIRE(ref.is_set());
			FORGE_CHECK(ref.unwrap() == 120);
		}
	}

	FORGE_TEST_CASE("Option with reference") {
		FORGE_SUBCASE("default constructor") {
			Option<int&> opt;
			FORGE_CHECK(!opt.is_set());
		}

		FORGE_SUBCASE("nullopt constructor") {
			Option<int&> opt = nullopt;
			FORGE_CHECK(!opt.is_set());
		}

		FORGE_SUBCASE("value constructor") {
			int i = 120;
			Option<int&> opt = i;
			FORGE_REQUIRE(opt.is_set());
			FORGE_CHECK(opt.unwrap() == 120);
		}

		FORGE_SUBCASE("copy constructor") {
			int i = 120;
			Option<int&> opt = i;
			FORGE_REQUIRE(opt.is_set());

			Option<int&> opt2 = opt;
			FORGE_REQUIRE(opt2.is_set());
			FORGE_CHECK(opt.unwrap() == opt2.unwrap());
		}

		FORGE_SUBCASE("copy assignment") {
			int i = 120;
			Option<int&> opt = i;
			FORGE_REQUIRE(opt.is_set());

			Option<int&> opt2 = nullopt;
			opt2 = opt;
			FORGE_REQUIRE(opt2.is_set());
			FORGE_CHECK(opt.unwrap() == opt2.unwrap());
		}

		FORGE_SUBCASE("move constructor") {
			int i = 120;
			Option<int&> opt = i;
			FORGE_REQUIRE(opt.is_set());

			Option<int&> opt2 = Forge::move(opt);
			FORGE_REQUIRE(opt2.is_set());
			FORGE_CHECK(opt2.unwrap() == 120);
		}

		FORGE_SUBCASE("move assignment") {
			int i = 120;
			Option<int&> opt = i;
			FORGE_REQUIRE(opt.is_set());

			Option<int&> opt2 = nullopt;
			opt2 = Forge::move(opt);
			FORGE_REQUIRE(opt2.is_set());
			FORGE_CHECK(opt2.unwrap() == 120);
		}

		FORGE_SUBCASE("unwrap") {
			int i = 120;
			Option<int&> opt = i;
			FORGE_REQUIRE(opt.is_set());
			int& unwrapped = opt.unwrap();
			FORGE_CHECK(opt.is_set());
			FORGE_CHECK(unwrapped == 120);
		}
	}
}
#endif // FORGE_ENABLE_TEST
