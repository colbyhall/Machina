/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

// Define supported platforms
#define FORGE_OS_UNKNOWN 0
#define FORGE_OS_WINDOWS 1
#define FORGE_OS_MACOS	 2
#define FORGE_OS_LINUX	 3

// Determine platform
#if defined(_WIN32) || defined(_WIN64)
	#include <winapifamily.h>
	#if WINAPI_FAMILY == WINAPI_FAMILY_APP
		#error UWP is not supported
	#endif
	#define FORGE_OS FORGE_OS_WINDOWS
#elif __APPLE__
	#define FORGE_OS FORGE_OS_MACOS
#endif

#ifndef FORGE_OS
	#define FORGE_OS FORGE_OS_UNKNOWN
#endif

#if FORGE_OS == FORGE_OS_UNKNOWN
	#error Unsupported platform
#endif

// Define supported compilers
#define FORGE_COMPILER_UNKNOWN 0
#define FORGE_COMPILER_MSVC	   1
#define FORGE_COMPILER_CLANG   2

// Determine the compiler
#if defined(_MSC_VER)
	#define FORGE_COMPILER FORGE_COMPILER_MSVC
#elif defined(__clang__)
	#define FORGE_COMPILER FORGE_COMPILER_CLANG
#endif

#ifndef FORGE_COMPILER
	#define FORGE_COMPILER FORGE_COMPILER_UNKNOWN
#endif

#if FORGE_COMPILER == FORGE_COMPILER_UNKNOWN
	#error Unsupported compiler
#endif

// Define supported CPU architectures
#define FORGE_CPU_UNKNOWN 0
#define FORGE_CPU_X86	  1
#define FORGE_CPU_ARM	  2

// Determine the CPU architecture
#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
	// X86 CPU architecture
	#define FORGE_CPU FORGE_CPU_X86
	#if !defined(__x86_64__) && !defined(_M_X64)
		#error Only support 64 bit architecture
	#endif
#elif __arm64__
	#define FORGE_CPU FORGE_CPU_ARM
#endif

#ifndef FORGE_CPU_X86
	#define FORGE_CPU FORGE_CPU_UNKNOWN
#endif

#if FORGE_CPU == FORGE_CPU_UNKNOWN
	#error Unsupported CPU architecture
#endif

// Pragmas to store / restore the warning state and to disable individual
// warnings
#if FORGE_COMPILER == FORGE_COMPILER_MSVC
	#define FORGE_PRAGMA(x)				   __pragma(x)
	#define FORGE_SUPPRESS_WARNING_PUSH	   FORGE_PRAGMA(warning(push))
	#define FORGE_SUPPRESS_WARNING_POP	   FORGE_PRAGMA(warning(pop))
	#define FORGE_MSVC_SUPPRESS_WARNING(w) FORGE_PRAGMA(warning(disable : w))
#else
	#define FORGE_MSVC_SUPPRESS_WARNING(w)
#endif

#if FORGE_COMPILER == FORGE_COMPILER_MSVC
	#define FORGE_ALWAYS_INLINE __forceinline
#elif FORGE_COMPILER == FORGE_COMPILER_CLANG
	#define FORGE_ALWAYS_INLINE __attribute__((always_inline))
#else
	#error Undefined inline
#endif

#define FORGE_NO_DISCARD [[nodiscard]]
#define FORGE_NO_RETURN	 [[noreturn]]

// Cache line size (used for aligning to cache line)
#ifndef FORGE_CACHE_LINE_SIZE
	#define FORGE_CACHE_LINE_SIZE 64
#endif

// Define macro to get current function name
#if FORGE_COMPILER == FORGE_COMPILER_MSVC || FORGE_COMPILER == FORGE_COMPILER_CLANG
	#define FORGE_FUNCTION_NAME __FUNCTION__
#else
	#error Undefined function name
#endif

#define FORGE_BUILD_RELEASE 0
#define FORGE_BUILD_DEBUG	1
#ifdef _DEBUG
	#define FORGE_BUILD FORGE_BUILD_DEBUG
#else
	#define FORGE_BUILD FORGE_BUILD_RELEASE
#endif

#define FORGE_THREAD_LOCAL __declspec(thread)

// Macro to indicate that a parameter / variable is unused
#define FORGE_UNUSED(x) (void)x

namespace Forge::Core {
	void trap();
	FORGE_NO_RETURN void abort();
	FORGE_NO_RETURN void exit(int status);

	template <typename F>
	struct Defer {
		F f;
		Defer(F f) : f(f) {}
		~Defer() { f(); }
	};

	template <typename F>
	Defer<F> defer_func(F f) {
		return Defer<F>(f);
	}
} // namespace Forge::Core

#define _FORGE_DEFER_1(x, y) x##y
#define _FORGE_DEFER_2(x, y) _FORGE_DEFER_1(x, y)
#define _FORGE_DEFER_3(x)	 _FORGE_DEFER_2(x, __COUNTER__)
#define FORGE_DEFER(code)	 auto _FORGE_DEFER_3(_defer_) = Forge::Core::defer_func([&]() { code; })

#define FORGE_NO_COPY(Class)                                                                                           \
	Class(const Class&) = delete;                                                                                      \
	Class& operator=(const Class&) = delete

#define FORGE_NO_MOVE(Class)                                                                                           \
	Class(Class&&) = delete;                                                                                           \
	Class& operator=(Class&&) = delete

#if FORGE_COMPILER == FORGE_COMPILER_MSVC
	#define FORGE_MSVC_DISABLE_WARNINGS_PUSH __pragma(warning(push))
	#define FORGE_MSVC_DISABLE_WARNINGS_POP	 __pragma(warning(pop))
	#define FORGE_MSVC_DISABLE_WARNING(x)	 __pragma(warning(disable : x))
#elif FORGE_COMPILER == FORGE_COMPILER_CLANG
	#define FORGE_CLANG_DISABLE_WARNINGS_PUSH _Pragma("clang diagnostic push")
	#define FORGE_CLANG_DISBALE_WARNINGS_POP  _Pragma("clang diagnostic pop")
	#define FORGE_CLANG_DO_PRAGMA(x)		  _Pragma(#x)
	#define FORGE_CLANG_DISABLE_WARNING(x)	  FORGE_CLANG_DO_PRAGMA(clang diagnostic ignored x)
#else
	#error Undefined warning suppress
#endif