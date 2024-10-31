/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Containers/WString.hpp>
#include <Core/IO/Directory.hpp>
#include <windows.h>

namespace Grizzly::Core {
	Win32Directory Win32Directory::cwd() {
		// Query the length of the path
		const auto len = (usize)::GetCurrentDirectoryW(0, nullptr);
		GRIZZLY_ASSERT(len > 0);

		// Create a wide string buffer to get the cwd path
		WString buffer;
		buffer.reserve(len);
		::GetCurrentDirectoryW(static_cast<DWORD>(buffer.cap()), buffer.begin());
		buffer.set_len(len);

		// Convert the wide string to a utf8 string
		String result;
		result.reserve(len);
		for (const auto c : buffer) {
			result.push(utf16_to_utf32(c));
		}
		return Win32Directory(Grizzly::move(result));
	}
} // namespace Grizzly::Core
