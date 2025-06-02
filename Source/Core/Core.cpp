/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Core.hpp>

#include <Core/Debug/StackTrace.hpp>
#include <Core/FileSystem/File.hpp>
#include <Core/Format.hpp>

#include <cstdlib>

namespace Mach::Core {
	void trap() {
#if MACH_BUILD == MACH_BUILD_DEBUG
		Formatter formatter{ File::stderr };
		formatter.format(u8"\n{red}Abort Triggered:\n\n{yellow}CallStack:{default}\n"_sv);
		const auto stack_trace = StackTrace::capture();
		for (u32 i = 1; i < stack_trace.len(); i++) {
			formatter.format(u8"{}\n"_sv, stack_trace[i]);
		}
#endif
#if MACH_COMPILER == MACH_COMPILER_MSVC
		__debugbreak();
#elif MACH_COMPILER == MACH_COMPILER_CLANG
		__builtin_trap();
#else
	#error Unknown trap
#endif
	}

	void abort() {
		Formatter formatter{ File::stderr };
		formatter.format(u8"\n{red}Abort Triggered{default}\n\n{yellow}CallStack:{default}\n"_sv);
		const auto stack_trace = StackTrace::capture();

		// Defines how many from the end we should early to not show OS garbage
		constexpr usize early_out = 4;
		const usize count = stack_trace.len() - early_out;
		for (usize i = 1; i < stack_trace.len() - early_out; i++) {
#if MACH_OS == MACH_OS_WINDOWS
			formatter.format(u8"{}: {}\n"_sv, count - i, stack_trace[i]);
#else
			MACH_UNUSED(count);
			formatter.format(u8"{}\n"_sv, stack_trace[i]);
#endif
		}
		std::abort();
	}
	void exit(int status) { std::exit(status); }
} // namespace Mach::Core
