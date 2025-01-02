/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Containers/HashMap.hpp>
#include <Core/Debug/Test.hpp>

#if GRIZZLY_ENABLE_TEST
GRIZZLY_TEST_SUITE("Containers") {
	using namespace Grizzly::Core;
	GRIZZLY_TEST_CASE("HashMap") {
		GRIZZLY_SUBCASE("insert") {
			HashMap<u32, u32> map;
			map.insert(1, 1);

			GRIZZLY_CHECK(map.len() == 1);
			GRIZZLY_CHECK(map.find(1).is_set());
		}

		GRIZZLY_SUBCASE("remove") {
			HashMap<u32, u32> map;
			map.insert(1, 1);
			map.insert(2, 2);
			map.insert(3, 3);

			GRIZZLY_CHECK(map.len() == 3);
			GRIZZLY_CHECK(map.find(1).is_set());
			GRIZZLY_CHECK(map.find(2).is_set());
			GRIZZLY_CHECK(map.find(3).is_set());

			const auto removed = map.remove(2);
			GRIZZLY_CHECK(removed.is_set());
			GRIZZLY_CHECK(removed.unwrap() == 2);

			GRIZZLY_CHECK(map.len() == 2);
			GRIZZLY_CHECK(map.find(1).is_set());
			GRIZZLY_CHECK(!map.find(2).is_set());
			GRIZZLY_CHECK(map.find(3).is_set());
		}
	}
}
#endif
