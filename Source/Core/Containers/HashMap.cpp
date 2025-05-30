/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Containers/HashMap.hpp>
#include <Core/Debug/Test.hpp>

#if MACH_ENABLE_TEST
MACH_TEST_SUITE("Containers") {
	using namespace Mach::Core;
	MACH_TEST_CASE("HashMap") {
		MACH_SUBCASE("insert") {
			HashMap<u32, u32> map;
			map.insert(1, 1);

			MACH_CHECK(map.len() == 1);
			MACH_CHECK(map.find(1).is_set());
		}

		MACH_SUBCASE("remove") {
			HashMap<u32, u32> map;
			map.insert(1, 1);
			map.insert(2, 2);
			map.insert(3, 3);

			MACH_CHECK(map.len() == 3);
			MACH_CHECK(map.find(1).is_set());
			MACH_CHECK(map.find(2).is_set());
			MACH_CHECK(map.find(3).is_set());

			const auto removed = map.remove(2);
			MACH_CHECK(removed.is_set());
			MACH_CHECK(removed.unwrap() == 2);

			MACH_CHECK(map.len() == 2);
			MACH_CHECK(map.find(1).is_set());
			MACH_CHECK(!map.find(2).is_set());
			MACH_CHECK(map.find(3).is_set());
		}
	}
}
#endif
