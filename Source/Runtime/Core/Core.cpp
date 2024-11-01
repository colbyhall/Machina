/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Core.hpp>
#include <cstdlib>

#if GRIZZLY_BUILD == GRIZZLY_BUILD_DEBUG
	#include <Core/Debug/StackTrace.hpp>
	#include <Core/Format.hpp>
	#include <Core/IO/File.hpp>
#endif

namespace Grizzly::Core {
	void trap() {
#if GRIZZLY_COMPILER == GRIZZLY_COMPILER_MSVC
		__debugbreak();
#elif GRIZZLY_COMPILER == GRIZZLY_COMPILER_CLANG
		__builtin_trap();
#else
	#error Unknown trap
#endif

#if GRIZZLY_BUILD == GRIZZLY_BUILD_DEBUG
		Formatter formatter{ File::stderr };
		formatter.format(u8"\n{red}Abort Triggered:\n\n{yellow}CallStack:{default}\n");
		const auto stack_trace = StackTrace::capture();
		for (u32 i = 1; i < stack_trace.len(); i++) {
			formatter.format(u8"{}\n", stack_trace[i]);
		}
#endif
	}

	void abort() {
#if GRIZZLY_BUILD == GRIZZLY_BUILD_DEBUG
		Formatter formatter{ File::stderr };
		formatter.format(u8"\n{red}Abort Triggered{default}\n\n{yellow}CallStack:{default}\n");
		const auto stack_trace = StackTrace::capture();
		for (u32 i = 1; i < stack_trace.len(); i++) {
			formatter.format(u8"{}\n", stack_trace[i]);
		}
#endif
		std::abort();
	}
	void exit(int status) { std::exit(status); }
} // namespace Grizzly::Core
