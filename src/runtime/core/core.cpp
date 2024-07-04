#include "core/test.h"

OP_TEST_SUITE("core") {
	OP_TEST_CASE("foo bar") {
		OP_INFO("Hello World");
		OP_CHECK(1 == 1);
	}
}