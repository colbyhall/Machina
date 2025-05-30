/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Core.hpp>

// Only allow asserts in debug builds
#if MACH_BUILD_DEBUG
	#define MACH_ENABLE_ASSERTS 1
#else
	#define MACH_ENABLE_ASSERTS 0
#endif

#if MACH_ENABLE_ASSERTS
	// Crashes application if expression evaluates to false. Usage:
	// ASSERT(condition) or ASSERT(condition, message)
	#define MACH_ASSERT(expression, ...)                                                                               \
		do {                                                                                                           \
			if (!(expression)) Mach::Core::abort();                                                                    \
		} while (false)

	// Halts application if expression evaluates to false but can resume. Usage:
	// ENSURE(condition) or ENSURE(condition, message)
	#define MACH_ENSURE(expression, ...)                                                                               \
		do {                                                                                                           \
			if (!(expression)) Mach::Core::trap();                                                                     \
		} while (false)
#else
	#define MACH_ASSERT(...) ((void)0)
	#define MACH_ENSURE(...) ((void)0)
#endif // MACH_ENABLE_ASSERTS

#define MACH_PANIC(msg)	   MACH_ASSERT(false, msg)
#define MACH_UNIMPLEMENTED MACH_PANIC("Unimplemented!")
#define MACH_UNREACHABLE   MACH_PANIC("Invalid code path!")
