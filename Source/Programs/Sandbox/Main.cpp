/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/IO/File.hpp>

using namespace Grizzly;
using namespace Core;

int main(int argc, char** argv) {
	const auto text = StringView(u8"Hello World\n");
	const auto bytes = Slice<u8 const>((const u8*)*text, text.len());
	auto& stderr = File::stderr;
	stderr.write(bytes);
	return 0;
}
