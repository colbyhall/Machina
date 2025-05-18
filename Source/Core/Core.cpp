/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Core.hpp>
#include <cstdlib>

#include <Core/Debug/StackTrace.hpp>
#include <Core/Format.hpp>
#include <Core/IO/File.hpp>

namespace Forge::Core {
	void trap() {
#if FORGE_BUILD == FORGE_BUILD_DEBUG
		Formatter formatter{ File::stderr };
		formatter.format(u8"\n{red}Abort Triggered:\n\n{yellow}CallStack:{default}\n"_sv);
		const auto stack_trace = StackTrace::capture();
		for (u32 i = 1; i < stack_trace.len(); i++) {
			formatter.format(u8"{}\n"_sv, stack_trace[i]);
		}
#endif
#if FORGE_COMPILER == FORGE_COMPILER_MSVC
		__debugbreak();
#elif FORGE_COMPILER == FORGE_COMPILER_CLANG
		__builtin_trap();
#else
	#error Unknown trap
#endif
	}

	void abort() {
		Formatter formatter{ File::stderr };
		formatter.format(u8"\n{red}Abort Triggered{default}\n\n{yellow}CallStack:{default}\n"_sv);
		const auto stack_trace = StackTrace::capture();
		for (u32 i = 1; i < stack_trace.len(); i++) {
			formatter.format(u8"{}\n"_sv, stack_trace[i]);
		}
		std::abort();
	}
	void exit(int status) { std::exit(status); }
} // namespace Forge::Core
