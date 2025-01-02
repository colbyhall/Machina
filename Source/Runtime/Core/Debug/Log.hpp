/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
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
		Core::Formatter formatter{ writer };
		formatter.format(u8"{yellow}"sv);
		formatter.format(fmt, args...);
		formatter.format(u8"\n{default}"sv);
		writer.flush();
	}
} // namespace Grizzly
