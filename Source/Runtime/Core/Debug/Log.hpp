/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Format.hpp>
#include <Core/IO/File.hpp>

namespace Grizzly {
	template <typename... Args>
	void dbgln(const StringView& fmt, const Args&... args) {
		Core::BufferedWriter writer{ Core::File::stderr };
		format(writer, u8"{yellow}");
		format(writer, fmt, args...);
		format(writer, u8"\n{default}");
		writer.flush();
	}
} // namespace Grizzly
