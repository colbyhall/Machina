/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Containers/SharedPtr.hpp>

namespace Mach::GPU {
	template <typename T>
	using Handle = SharedPtr<T>;

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
} // namespace Mach::GPU
