/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

// Define supported platforms
#define OP_PLATFORM_UNKNOWN 0
#define OP_PLATFORM_WINDOWS 1

// Determine platform
#if defined(_WIN32) || defined(_WIN64)
	#include <winapifamily.h>
	#if WINAPI_FAMILY == WINAPI_FAMILY_APP
		#error UWP is not supported
	#endif
	#define OP_PLATFORM OP_PLATFORM_WINDOWS
#endif

#ifndef OP_PLATFORM
	#define OP_PLATFORM OP_PLATFORM_UNKNOWN
#endif

#if OP_PLATFORM == OP_PLATFORM_UNKNOWN
	#error Unsupported platform
#endif

// Define supported compilers
#define OP_COMPILER_UNKNOWN 0
#define OP_COMPILER_MSVC	1

// Determine the compiler
#if defined(_MSC_VER)
	#define OP_COMPILER OP_COMPILER_MSVC
#endif

#ifndef OP_COMPILER
	#define OP_COMPILER OP_COMPILER_UNKNOWN
#endif

#if OP_COMPILER == OP_COMPILER_UNKNOWN
	#error Unsupported compiler
#endif

// Define supported CPU architectures
#define OP_CPU_UNKNOWN 0
#define OP_CPU_X86	   1

// Determine the CPU architecture
#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
	// X86 CPU architecture
	#define OP_CPU OP_CPU_X86
	#if !defined(__x86_64__) && !defined(_M_X64)
		#error Only support 64 bit architecture
	#endif
#endif

#ifndef OP_CPU_X86
	#define OP_CPU OP_CPU_UNKNOWN
#endif

#if OP_CPU == OP_CPU_UNKNOWN
	#error Unsupported CPU architecture
#endif

// Pragmas to store / restore the warning state and to disable individual
// warnings
#if OP_COMPILER == OP_COMPILER_MSVC
	#define OP_PRAGMA(x)				__pragma(x)
	#define OP_SUPPRESS_WARNING_PUSH	OP_PRAGMA(warning(push))
	#define OP_SUPPRESS_WARNING_POP		OP_PRAGMA(warning(pop))
	#define OP_MSVC_SUPPRESS_WARNING(w) OP_PRAGMA(warning(disable : w))
#else
	#define OP_MSVC_SUPPRESS_WARNING(w)
#endif

#if OP_COMPILER == OP_COMPILER_MSVC
	#define OP_ALWAYS_INLINE __forceinline
#else
	#error Undefined inline
#endif

#define OP_NO_DISCARD [[nodiscard]]

// Cache line size (used for aligning to cache line)
#ifndef OP_CACHE_LINE_SIZE
	#define OP_CACHE_LINE_SIZE 64
#endif

// Define macro to get current function name
#if OP_COMPILER == OP_COMPILER_MSVC
	#define OP_FUNCTION_NAME __FUNCTION__
#else
	#error Undefined function name
#endif

#if OP_COMPILER == OP_COMPILER_MSVC
	#define OP_DEBUGBREAK __debugbreak()
#else
	#error Unknown debug break
#endif

#define OP_BUILD_RELEASE 0
#define OP_BUILD_DEBUG	 1
#ifdef _DEBUG
	#define OP_BUILD OP_BUILD_DEBUG
#else
	#define OP_BUILD OP_BUILD_RELEASE
#endif

#define OP_THREAD_LOCAL __declspec(thread)

// Macro to indicate that a parameter / variable is unused
#define OP_UNUSED(x) (void)x

// Macro that will crash the program
#define OP_CRASH                                                                                                       \
	do {                                                                                                               \
		int* _ptr = nullptr;                                                                                           \
		*_ptr = 0;                                                                                                     \
	} while (false)
