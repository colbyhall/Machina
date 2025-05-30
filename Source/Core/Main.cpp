/**
 * Copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Core.hpp>

namespace Mach {
	extern int main();
}

int main(int argc, char** argv) {
	MACH_UNUSED(argc);
	MACH_UNUSED(argv);
	return Mach::main();
}
