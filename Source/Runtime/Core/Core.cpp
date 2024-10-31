/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Core.hpp>
#include <cstdlib>

namespace Grizzly::Core {
	void trap() {
#if GRIZZLY_COMPILER == GRIZZLY_COMPILER_MSVC
		__debugbreak();
#elif GRIZZLY_COMPILER == GRIZZLY_COMPILER_CLANG
		__builtin_trap();
#else
	#error Unknown trap
#endif
	}

	void abort() { std::abort(); }
	void exit(int status) { std::exit(status); }
} // namespace Grizzly::Core
