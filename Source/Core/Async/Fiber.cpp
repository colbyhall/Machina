/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Async/Fiber.hpp>
#include <Core/Debug/Log.hpp>

namespace Forge::Core {
	Arc<Fiber> Fiber::spawn(Function&& f) { return Fiber::spawn(Forge::move(f), SpawnInfo{}); }
} // namespace Forge::Core
