/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Core.hpp>

// Only allow asserts in debug builds
#if FORGE_BUILD_DEBUG
	#define FORGE_ENABLE_ASSERTS 1
#else
	#define FORGE_ENABLE_ASSERTS 0
#endif

#if FORGE_ENABLE_ASSERTS
	// Crashes application if expression evaluates to false. Usage:
	// ASSERT(condition) or ASSERT(condition, message)
	#define FORGE_ASSERT(expression, ...)                                                                              \
		do {                                                                                                           \
			if (!(expression)) Forge::Core::abort();                                                                   \
		} while (false)

	// Halts application if expression evaluates to false but can resume. Usage:
	// ENSURE(condition) or ENSURE(condition, message)
	#define FORGE_ENSURE(expression, ...)                                                                              \
		do {                                                                                                           \
			if (!(expression)) Forge::Core::trap();                                                                    \
		} while (false)
#else
	#define FORGE_ASSERT(...) ((void)0)
	#define FORGE_ENSURE(...) ((void)0)
#endif // FORGE_ENABLE_ASSERTS

#define FORGE_PANIC(msg)	FORGE_ASSERT(false, msg)
#define FORGE_UNIMPLEMENTED FORGE_PANIC("Unimplemented!")
#define FORGE_UNREACHABLE	FORGE_PANIC("Invalid code path!")
