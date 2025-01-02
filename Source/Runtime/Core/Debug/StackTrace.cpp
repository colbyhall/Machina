/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include "Core/Containers/StringView.hpp"
#include <Core/Debug/StackTrace.hpp>

#if GRIZZLY_PLATFORM == GRIZZLY_PLATFORM_WINDOWS
	#include <DbgHelp.h>
	#include <Windows.h>

namespace Grizzly::Core::StackTrace {
	Array<Frame> capture() {
		constexpr int max_frames = 128;
		Array<Frame> result;

		void* stack_frames[max_frames];
		const int frame_count = ::CaptureStackBackTrace(0, max_frames, stack_frames, nullptr);

		SYMBOL_INFO* symbol = (SYMBOL_INFO*)malloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char));
		symbol->MaxNameLen = 255;
		symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

		HANDLE process = ::GetCurrentProcess();
		::SymInitialize(process, nullptr, TRUE);

		result.reserve(max_frames);

		for (int i = 0; i < frame_count; ++i) {
			::SymFromAddr(process, (DWORD64)(stack_frames[i]), 0, symbol);

			result.push(String::from(StringView{ (const UTF8Char*)&symbol->Name[0] }));
		}

		free(symbol);
		return result;
	}
} // namespace Grizzly::Core::StackTrace

#else
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
				const StringView as((const UTF8Char*)symbols[i], strlen(symbols[i]));
				result.push(String::from(as));
			}
			free(symbols);
		}

		return result;
	}
} // namespace Grizzly::Core::StackTrace
#endif
