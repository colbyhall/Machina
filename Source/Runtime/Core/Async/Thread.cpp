/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Async/Thread.hpp>

namespace Grizzly::Core {
	Arc<Thread> Thread::spawn(Function&& f) { return Thread::spawn(Grizzly::forward<Function>(f), {}); }
} // namespace Grizzly::Core
