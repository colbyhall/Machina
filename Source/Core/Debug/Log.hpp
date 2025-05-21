/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/FileSystem/File.hpp>
#include <Core/Format.hpp>

namespace Forge {
	template <typename... Args>
	void dbgln(const StringView& fmt, const Args&... args) {
		Core::BufferedWriter writer{ Core::File::stderr };
		Core::Formatter formatter{ writer };
		formatter.format(u8"{yellow}"_sv);
		formatter.format(fmt, args...);
		formatter.format(u8"\n{default}"_sv);
		writer.flush();
	}
} // namespace Forge
