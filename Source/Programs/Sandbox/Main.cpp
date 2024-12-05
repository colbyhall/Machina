/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Async/Fiber.hpp>
#include <Core/Containers/String.hpp>
#include <Core/Debug/Log.hpp>

using namespace Grizzly;
using namespace Core;

int main(int argc, char** argv) {
	dbgln(u8"Hello, World!");

	const auto current = Fiber::current();
	const auto fiber = Fiber::spawn([&] {
		dbgln(u8"Hello, Fiber!");
		current->switch_to();
	});
	fiber->switch_to();

	dbgln(u8"Hello, Again!");

	return 0;
}
