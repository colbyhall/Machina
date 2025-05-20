/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Containers/HashMap.hpp>
#include <Core/Debug/Test.hpp>

#if FORGE_ENABLE_TEST
FORGE_TEST_SUITE("Containers") {
	using namespace Forge::Core;
	FORGE_TEST_CASE("HashMap") {
		FORGE_SUBCASE("insert") {
			HashMap<u32, u32> map;
			map.insert(1, 1);

			FORGE_CHECK(map.len() == 1);
			FORGE_CHECK(map.find(1).is_set());
		}

		FORGE_SUBCASE("remove") {
			HashMap<u32, u32> map;
			map.insert(1, 1);
			map.insert(2, 2);
			map.insert(3, 3);

			FORGE_CHECK(map.len() == 3);
			FORGE_CHECK(map.find(1).is_set());
			FORGE_CHECK(map.find(2).is_set());
			FORGE_CHECK(map.find(3).is_set());

			const auto removed = map.remove(2);
			FORGE_CHECK(removed.is_set());
			FORGE_CHECK(removed.unwrap() == 2);

			FORGE_CHECK(map.len() == 2);
			FORGE_CHECK(map.find(1).is_set());
			FORGE_CHECK(!map.find(2).is_set());
			FORGE_CHECK(map.find(3).is_set());
		}
	}
}
#endif
