/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include "core/core.h"

// Only allow asserts in debug builds
#if OP_BUILD_DEBUG
	#define OP_ENABLE_ASSERTS 1
#else
	#define OP_ENABLE_ASSERTS 0
#endif

#if OP_ENABLE_ASSERTS
	// Crashes application if expression evaluates to false. Usage:
	// ASSERT(condition) or ASSERT(condition, message)
	#define OP_ASSERT(expression, ...)                                                                                 \
		do {                                                                                                           \
			if (!(expression)) {                                                                                       \
				OP_CRASH;                                                                                              \
			}                                                                                                          \
		} while (false)

	// Halts application if expression evaluates to false but can resume. Usage:
	// ENSURE(condition) or ENSURE(condition, message)
	#define OP_ENSURE(expression, ...)                                                                                 \
		do {                                                                                                           \
			if (!(expression)) OP_DEBUGBREAK;                                                                          \
		} while (false)
#else
	#define OP_ASSERT(...) ((void)0)
	#define OP_ENSURE(...) ((void)0)
#endif // OP_ENABLE_ASSERTS

#define OP_PANIC(msg)	 OP_ASSERT(false, msg)
#define OP_UNIMPLEMENTED OP_PANIC("Unimplemented!")
#define OP_UNREACHABLE	 OP_PANIC("Invalid code path!")
