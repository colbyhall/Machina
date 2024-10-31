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
	const auto stack_trace = StackTrace::capture();
	format(File::stderr, u8"\033[5;4;31mHello World {}\n", 145);
	return 0;
}
