/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
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
	class Buffer;
	class Texture;
	class GraphicsPipeline;
	class CommandList;
	class Receipt;

	class MetalDevice;
	class MetalSwapchain;
} // namespace Grizzly::GPU
