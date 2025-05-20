/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Debug/StackTrace.hpp>

#include <Core/Containers/StringView.hpp>

#if FORGE_OS == FORGE_OS_WINDOWS
	#include <Core/Windows.hpp>
	#include <DbgHelp.h>

namespace Forge::Core::StackTrace {
	Array<Frame> capture() {
		Array<Frame> result;

	#if 0
		constexpr int max_frames = 128;
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

			result.push(String::from(StringView{
				(const UTF8Char*)&symbol->Name[0],
				static_cast<usize>(symbol->NameLen),
			}));
		}

		free(symbol);
	#endif
		return result;
	}
} // namespace Forge::Core::StackTrace

#else
	#include <execinfo.h>

namespace Forge::Core::StackTrace {
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
} // namespace Forge::Core::StackTrace
#endif
