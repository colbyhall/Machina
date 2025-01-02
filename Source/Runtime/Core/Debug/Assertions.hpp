/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Core.hpp>

// Only allow asserts in debug builds
#if GRIZZLY_BUILD_DEBUG
	#define GRIZZLY_ENABLE_ASSERTS 1
#else
	#define GRIZZLY_ENABLE_ASSERTS 0
#endif

#if GRIZZLY_ENABLE_ASSERTS
	// Crashes application if expression evaluates to false. Usage:
	// ASSERT(condition) or ASSERT(condition, message)
	#define GRIZZLY_ASSERT(expression, ...)                                                                            \
		do {                                                                                                           \
			if (!(expression)) Grizzly::Core::abort();                                                                 \
		} while (false)

	// Halts application if expression evaluates to false but can resume. Usage:
	// ENSURE(condition) or ENSURE(condition, message)
	#define GRIZZLY_ENSURE(expression, ...)                                                                            \
		do {                                                                                                           \
			if (!(expression)) Grizzly::Core::trap();                                                                  \
		} while (false)
#else
	#define GRIZZLY_ASSERT(...) ((void)0)
	#define GRIZZLY_ENSURE(...) ((void)0)
#endif // GRIZZLY_ENABLE_ASSERTS

#define GRIZZLY_PANIC(msg)	  GRIZZLY_ASSERT(false, msg)
#define GRIZZLY_UNIMPLEMENTED GRIZZLY_PANIC("Unimplemented!")
#define GRIZZLY_UNREACHABLE	  GRIZZLY_PANIC("Invalid code path!")
