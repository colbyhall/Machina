/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#import <QuartzCore/CAMetalLayer.h>

#include <Core/ObjectiveC/Interface.hpp>
#include <GPU/Swapchain.hpp>

namespace Grizzly::GPU {
	class MetalSwapchain final : public Swapchain {
	public:
		explicit MetalSwapchain(CAMetalLayer* layer) : m_layer(layer) {}

		Shared<Texture> next_back_buffer() final;

	private:
		Core::Interface<CAMetalLayer> m_layer;
	};
} // namespace Grizzly::GPU
