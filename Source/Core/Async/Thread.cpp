/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Async/Thread.hpp>

namespace Mach::Core {
	Mach::SharedPtr<Thread> Thread::spawn(Function&& f) { return Thread::spawn(Mach::forward<Function>(f), {}); }
} // namespace Mach::Core
