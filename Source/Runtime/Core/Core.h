/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

// Define supported platforms
#define GRIZZLY_PLATFORM_UNKNOWN 0
#define GRIZZLY_PLATFORM_WINDOWS 1
#define GRIZZLY_PLATFORM_MACOS	2

// Determine platform
#if defined(_WIN32) || defined(_WIN64)
	#include <winapifamily.h>
	#if WINAPI_FAMILY == WINAPI_FAMILY_APP
		#error UWP is not supported
	#endif
	#define GRIZZLY_PLATFORM GRIZZLY_PLATFORM_WINDOWS
#elif __APPLE__
	#define GRIZZLY_PLATFORM GRIZZLY_PLATFORM_MACOS
#endif

#ifndef GRIZZLY_PLATFORM
	#define GRIZZLY_PLATFORM GRIZZLY_PLATFORM_UNKNOWN
#endif

#if GRIZZLY_PLATFORM == GRIZZLY_PLATFORM_UNKNOWN
	#error Unsupported platform
#endif

// Define supported compilers
#define GRIZZLY_COMPILER_UNKNOWN 0
#define GRIZZLY_COMPILER_MSVC	1
#define GRIZZLY_COMPILER_CLANG	2

// Determine the compiler
#if defined(_MSC_VER)
	#define GRIZZLY_COMPILER GRIZZLY_COMPILER_MSVC
#elif defined(__clang__)
	#define GRIZZLY_COMPILER GRIZZLY_COMPILER_CLANG
#endif

#ifndef GRIZZLY_COMPILER
	#define GRIZZLY_COMPILER GRIZZLY_COMPILER_UNKNOWN
#endif

#if GRIZZLY_COMPILER == GRIZZLY_COMPILER_UNKNOWN
	#error Unsupported compiler
#endif

// Define supported CPU architectures
#define GRIZZLY_CPU_UNKNOWN 0
#define GRIZZLY_CPU_X86	   1
#define GRIZZLY_CPU_ARM	   2

// Determine the CPU architecture
#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
	// X86 CPU architecture
	#define GRIZZLY_CPU GRIZZLY_CPU_X86
	#if !defined(__x86_64__) && !defined(_M_X64)
		#error Only support 64 bit architecture
	#endif
#elif __arm64__
	#define GRIZZLY_CPU GRIZZLY_CPU_ARM
#endif

#ifndef GRIZZLY_CPU_X86
	#define GRIZZLY_CPU GRIZZLY_CPU_UNKNOWN
#endif

#if GRIZZLY_CPU == GRIZZLY_CPU_UNKNOWN
	#error Unsupported CPU architecture
#endif

// Pragmas to store / restore the warning state and to disable individual
// warnings
#if GRIZZLY_COMPILER == GRIZZLY_COMPILER_MSVC
	#define GRIZZLY_PRAGMA(x)				__pragma(x)
	#define GRIZZLY_SUPPRESS_WARNING_PUSH	GRIZZLY_PRAGMA(warning(push))
	#define GRIZZLY_SUPPRESS_WARNING_POP		GRIZZLY_PRAGMA(warning(pop))
	#define GRIZZLY_MSVC_SUPPRESS_WARNING(w) GRIZZLY_PRAGMA(warning(disable : w))
#else
	#define GRIZZLY_MSVC_SUPPRESS_WARNING(w)
#endif

#if GRIZZLY_COMPILER == GRIZZLY_COMPILER_MSVC
	#define GRIZZLY_ALWAYS_INLINE __forceinline
#elif GRIZZLY_COMPILER == GRIZZLY_COMPILER_CLANG
	#define GRIZZLY_ALWAYS_INLINE __attribute__((always_inline))
#else
	#error Undefined inline
#endif

#define GRIZZLY_NO_DISCARD [[nodiscard]]

// Cache line size (used for aligning to cache line)
#ifndef GRIZZLY_CACHE_LINE_SIZE
	#define GRIZZLY_CACHE_LINE_SIZE 64
#endif

// Define macro to get current function name
#if GRIZZLY_COMPILER == GRIZZLY_COMPILER_MSVC || GRIZZLY_COMPILER == GRIZZLY_COMPILER_CLANG
	#define GRIZZLY_FUNCTION_NAME __FUNCTION__
#else
	#error Undefined function name
#endif

#if GRIZZLY_COMPILER == GRIZZLY_COMPILER_MSVC
	#define GRIZZLY_DEBUGBREAK __debugbreak()
#elif GRIZZLY_COMPILER == GRIZZLY_COMPILER_CLANG
	#define GRIZZLY_DEBUGBREAK __builtin_trap()
#else
	#error Unknown debug break
#endif

#define GRIZZLY_BUILD_RELEASE 0
#define GRIZZLY_BUILD_DEBUG	 1
#ifdef _DEBUG
	#define GRIZZLY_BUILD GRIZZLY_BUILD_DEBUG
#else
	#define GRIZZLY_BUILD GRIZZLY_BUILD_RELEASE
#endif

#define GRIZZLY_THREAD_LOCAL __declspec(thread)

// Macro to indicate that a parameter / variable is unused
#define GRIZZLY_UNUSED(x) (void)x

// Macro that will crash the program
#define GRIZZLY_CRASH                                                                                                       \
	do {                                                                                                               \
		int* _ptr = nullptr;                                                                                           \
		*_ptr = 0;                                                                                                     \
	} while (false)
