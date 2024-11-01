/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Containers/String.hpp>
#include <Core/Debug/Log.hpp>

using namespace Grizzly;
using namespace Core;

int main(int argc, char** argv) {
	const auto foo = String::format(u8"Hello, World! {}", 42);
	dbgln(u8"Hello, World! {}", foo);
	return 0;
}
