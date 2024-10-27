/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Core.hpp>
#include <Core/Primitives.hpp>

namespace Grizzly::GPU {
	enum class Backend : u8;
	class Device;
	class Swapchain;
	using SwapchainOwner = void*;

	class MetalDevice;
	class MetalSwapchain;
} // namespace Grizzly::GPU
