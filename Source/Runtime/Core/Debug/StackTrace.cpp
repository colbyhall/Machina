/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Debug/StackTrace.hpp>
#include <execinfo.h>

namespace Grizzly::Core::StackTrace {
	Array<Frame> capture() {
		constexpr int max_frames = 128;
		Array<Frame> result;

		void* stack_frames[max_frames];
		const int frame_count = backtrace(stack_frames, max_frames);

		char** symbols = backtrace_symbols(stack_frames, frame_count);
		if (symbols) {
			result.reserve(max_frames);

			for (int i = 0; i < frame_count; ++i) {
				const StringView as((const UTF8Char*)symbols[i]);
				result.push(String::from(as));
			}
			free(symbols);
		}

		return result;
	}
} // namespace Grizzly::Core::StackTrace
