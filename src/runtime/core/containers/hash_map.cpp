/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include "core/containers/hash_map.h"

#include "core/debug/test.h"

OP_TEST_SUITE("containers") {
	using namespace op::core;
	OP_TEST_CASE("HashMap") {
		OP_SUBCASE("insert") {
			HashMap<u32, u32> map;
			map.insert(1, 1);

			OP_CHECK(map.len() == 1);
			OP_CHECK(map.find(1).is_set());
		}

		OP_SUBCASE("remove") {
			HashMap<u32, u32> map;
			map.insert(1, 1);
			map.insert(2, 2);
			map.insert(3, 3);

			OP_CHECK(map.len() == 3);
			OP_CHECK(map.find(1).is_set());
			OP_CHECK(map.find(2).is_set());
			OP_CHECK(map.find(3).is_set());

			const auto removed = map.remove(2);
			OP_CHECK(removed.is_set());
			OP_CHECK(removed.unwrap() == 2);

			OP_CHECK(map.len() == 2);
			OP_CHECK(map.find(1).is_set());
			OP_CHECK(!map.find(2).is_set());
			OP_CHECK(map.find(3).is_set());
		}
	}
}