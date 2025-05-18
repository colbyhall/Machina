/**
 * Copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Core.hpp>

namespace Forge {
	extern int main();
}

int main(int argc, char** argv) {
	FORGE_UNUSED(argc);
	FORGE_UNUSED(argv);
	return Forge::main();
}
