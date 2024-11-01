/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Debug/StackTrace.hpp>
#include <Core/Format.hpp>
#include <Core/IO/File.hpp>

using namespace Grizzly;
using namespace Core;

int main(int argc, char** argv) {
	Core::format(File::stderr, u8"{}\n", 1, 2, 3, 4, 5);
	return 0;
}
