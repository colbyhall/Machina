/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

// Define supported platforms
#define MACH_OS_UNKNOWN 0
#define MACH_OS_WINDOWS 1
#define MACH_OS_MACOS	2
#define MACH_OS_LINUX	3

// Determine platform
#if defined(_WIN32) || defined(_WIN64)
	#include <winapifamily.h>
	#if WINAPI_FAMILY == WINAPI_FAMILY_APP
		#error UWP is not supported
	#endif
	#define MACH_OS MACH_OS_WINDOWS
#elif __APPLE__
	#define MACH_OS MACH_OS_MACOS
#endif

#ifndef MACH_OS
	#define MACH_OS MACH_OS_UNKNOWN
#endif

#if MACH_OS == MACH_OS_UNKNOWN
	#error Unsupported platform
#endif

// Define supported compilers
#define MACH_COMPILER_UNKNOWN 0
#define MACH_COMPILER_MSVC	  1
#define MACH_COMPILER_CLANG	  2

// Determine the compiler
#if defined(_MSC_VER)
	#define MACH_COMPILER MACH_COMPILER_MSVC
#elif defined(__clang__)
	#define MACH_COMPILER MACH_COMPILER_CLANG
#endif

#ifndef MACH_COMPILER
	#define MACH_COMPILER MACH_COMPILER_UNKNOWN
#endif

#if MACH_COMPILER == MACH_COMPILER_UNKNOWN
	#error Unsupported compiler
#endif

// Determine what language this translation unit is
#define MACH_LANGUAGE_C		 0
#define MACH_LANGUAGE_CPP	 1
#define MACH_LANGUAGE_OBJC	 2
#define MACH_LANGUAGE_OBJCPP 3

#if defined(__cplusplus) && defined(__OBJC__)
	#define MACH_LANGUAGE MACH_LANGUAGE_OBJCPP
#elif defined(__cplusplus)
	#define MACH_LANGUAGE MACH_LANGUAGE_CPP
#elif defined(__OBJC__)
	#define MACH_LANGUAGE MACH_LANGUAGE_OBJC
#else
	#define MACH_LANGUAGE MACH_LANGUAGE_C
#endif

// Define supported CPU architectures
#define MACH_CPU_UNKNOWN 0
#define MACH_CPU_X86	 1
#define MACH_CPU_ARM	 2

// Determine the CPU architecture
#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
	// X86 CPU architecture
	#define MACH_CPU MACH_CPU_X86
	#if !defined(__x86_64__) && !defined(_M_X64)
		#error Only support 64 bit architecture
	#endif
#elif __arm64__
	#define MACH_CPU MACH_CPU_ARM
#endif

#ifndef MACH_CPU_X86
	#define MACH_CPU MACH_CPU_UNKNOWN
#endif

#if MACH_CPU == MACH_CPU_UNKNOWN
	#error Unsupported CPU architecture
#endif

// Pragmas to store / restore the warning state and to disable individual
// warnings
#if MACH_COMPILER == MACH_COMPILER_MSVC
	#define MACH_PRAGMA(x)				  __pragma(x)
	#define MACH_SUPPRESS_WARNING_PUSH	  MACH_PRAGMA(warning(push))
	#define MACH_SUPPRESS_WARNING_POP	  MACH_PRAGMA(warning(pop))
	#define MACH_MSVC_SUPPRESS_WARNING(w) MACH_PRAGMA(warning(disable : w))
#else
	#define MACH_MSVC_SUPPRESS_WARNING(w)
#endif

#if MACH_COMPILER == MACH_COMPILER_MSVC
	#define MACH_ALWAYS_INLINE __forceinline
#elif MACH_COMPILER == MACH_COMPILER_CLANG
	#define MACH_ALWAYS_INLINE __attribute__((always_inline))
#else
	#error Undefined inline
#endif

#define MACH_NO_DISCARD [[nodiscard]]
#define MACH_NO_RETURN	[[noreturn]]

// Cache line size (used for aligning to cache line)
#ifndef MACH_CACHE_LINE_SIZE
	#define MACH_CACHE_LINE_SIZE 64
#endif

// Define macro to get current function name
#if MACH_COMPILER == MACH_COMPILER_MSVC || MACH_COMPILER == MACH_COMPILER_CLANG
	#define MACH_FUNCTION_NAME __FUNCTION__
#else
	#error Undefined function name
#endif

#define MACH_BUILD_RELEASE 0
#define MACH_BUILD_DEBUG   1
#ifdef _DEBUG
	#define MACH_BUILD MACH_BUILD_DEBUG
#else
	#define MACH_BUILD MACH_BUILD_RELEASE
#endif

#define MACH_THREAD_LOCAL __declspec(thread)

// Macro to indicate that a parameter / variable is unused
#define MACH_UNUSED(x) (void)x

namespace Mach::Core {
	void trap();
	MACH_NO_RETURN void abort();
	MACH_NO_RETURN void exit(int status);

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
} // namespace Mach::Core

#define _MACH_DEFER_1(x, y) x##y
#define _MACH_DEFER_2(x, y) _MACH_DEFER_1(x, y)
#define _MACH_DEFER_3(x)	_MACH_DEFER_2(x, __COUNTER__)
#define MACH_DEFER(code)	auto _MACH_DEFER_3(_defer_) = Mach::Core::defer_func([&]() { code; })

#define MACH_NO_COPY(Class)                                                                                            \
	Class(const Class&) = delete;                                                                                      \
	Class& operator=(const Class&) = delete

#define MACH_NO_MOVE(Class)                                                                                            \
	Class(Class&&) = delete;                                                                                           \
	Class& operator=(Class&&) = delete

#if MACH_COMPILER == MACH_COMPILER_MSVC
	#define MACH_MSVC_DISABLE_WARNINGS_PUSH __pragma(warning(push))
	#define MACH_MSVC_DISABLE_WARNINGS_POP	__pragma(warning(pop))
	#define MACH_MSVC_DISABLE_WARNING(x)	__pragma(warning(disable : x))
#elif MACH_COMPILER == MACH_COMPILER_CLANG
	#define MACH_CLANG_DISABLE_WARNINGS_PUSH _Pragma("clang diagnostic push")
	#define MACH_CLANG_DISBALE_WARNINGS_POP	 _Pragma("clang diagnostic pop")
	#define MACH_CLANG_DO_PRAGMA(x)			 _Pragma(#x)
	#define MACH_CLANG_DISABLE_WARNING(x)	 MACH_CLANG_DO_PRAGMA(clang diagnostic ignored x)
#else
	#error Undefined warning suppress
#endif
