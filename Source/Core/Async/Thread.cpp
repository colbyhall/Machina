/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Async/Thread.hpp>

namespace Forge::Core {
	Arc<Thread> Thread::spawn(Function&& f) { return Thread::spawn(Forge::forward<Function>(f), {}); }
} // namespace Forge::Core
