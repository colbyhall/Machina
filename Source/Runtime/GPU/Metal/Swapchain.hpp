/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <GPU/Swapchain.hpp>
#import <QuartzCore/CAMetalLayer.h>

namespace Grizzly::GPU {
	class MetalSwapchain final : public Swapchain {
	public:
		explicit MetalSwapchain(CAMetalLayer* layer) : m_layer(layer) {}
		MetalSwapchain(const MetalSwapchain&) = delete;
		MetalSwapchain& operator=(const MetalSwapchain&) = delete;
		MetalSwapchain(MetalSwapchain&& move) : m_layer(move.m_layer) { move.m_layer = nullptr; }
		MetalSwapchain& operator=(MetalSwapchain&& move) {
			auto to_destroy = Grizzly::move(*this);
			GRIZZLY_UNUSED(to_destroy);

			m_layer = move.m_layer;
			move.m_layer = nullptr;
			return *this;
		}

		// Swapchain interace
		~MetalSwapchain() final;
		// ~Swapchain interface

	private:
		CAMetalLayer* m_layer;
	};
} // namespace Grizzly::GPU
