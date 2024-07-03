/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

// Determine platform
#if defined(_WIN32) || defined(_WIN64)
	#include <winapifamily.h>
	#if WINAPI_FAMILY == WINAPI_FAMILY_APP
		#define OP_PLATFORM_WINDOWS_UWP
	#endif
	#define OP_PLATFORM_WINDOWS 1
#else
	#error Unsupported platform
#endif
#ifndef OP_PLATFORM_WINDOWS
	#define OP_PLATFORM_WINDOWS 0
#endif // OP_PLATFORM_WINDOWS

// Determine compiler
#if defined(_MSC_VER)
	#define OP_COMPILER_MSVC 1
#else
	#error Unsupported compiler
#endif
#ifndef OP_COMPILER_MSVC
	#define OP_COMPILER_MSVC 0
#endif

// Detect CPU architecture
#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
	// X86 CPU architecture
	#define OP_CPU_X86 1
	#if defined(__x86_64__) || defined(_M_X64)
		#define OP_CPU_ADDRESS_BITS 64
	#else
		#error Only support x86_64
	#endif
#else
	#error Unsupported CPU architecture
#endif
#ifndef OP_GPU_X86
	#define OP_GPU_X86 0
#endif

// Pragmas to store / restore the warning state and to disable individual
// warnings
#if OP_COMPILER_MSVC
	#define OP_PRAGMA(x)				__pragma(x)
	#define OP_SUPPRESS_WARNING_PUSH	OP_PRAGMA(warning(push))
	#define OP_SUPPRESS_WARNING_POP		OP_PRAGMA(warning(pop))
	#define OP_MSVC_SUPPRESS_WARNING(w) OP_PRAGMA(warning(disable : w))
#else
	#define OP_MSVC_SUPPRESS_WARNING(w)
#endif

#if OP_COMPILER_MSVC
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
#if OP_COMPILER_MSVC
	#define OP_FUNCTION_NAME __FUNCTION__
#else
	#error Undefined function name
#endif

#if OP_COMPILER_MSVC
	#define OP_DEBUGBREAK __debugbreak()
#else
	#error Unknown debug break
#endif

#ifdef _DEBUG
	#define OP_BUILD_DEBUG 1
#else
	#define OP_BUILD_DEBUG 0
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