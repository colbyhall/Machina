/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Async/Fiber.hpp>
#include <Core/Debug/Log.hpp>

namespace Mach::Core {
	Mach::SharedPtr<Fiber> Fiber::spawn(Function&& f) { return Fiber::spawn(Mach::move(f), SpawnInfo{}); }
} // namespace Mach::Core
