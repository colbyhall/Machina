/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <core/containers/hash_map.h>
#include <core/debug/test.h>

#if GRIZZLY_ENABLE_TEST
GRIZZLY_TEST_SUITE("containers") {
	using namespace grizzly::core;
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
