/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#import <QuartzCore/CAMetalLayer.h>

#include <Core/ObjectiveC/Interface.hpp>
#include <Core/ObjectiveC/Protocol.hpp>
#include <GPU/Swapchain.hpp>

namespace Forge::GPU {
	class MetalSwapchain final : public Swapchain {
	public:
		explicit MetalSwapchain(CAMetalLayer* layer) : m_layer(layer) {}

		UniquePtr<Backbuffer> next_back_buffer() final;

	private:
		Core::Interface<CAMetalLayer> m_layer;
	};

	class MetalBackbuffer final : public Backbuffer {
	public:
		explicit MetalBackbuffer(SharedPtr<Texture>&& texture, id<CAMetalDrawable> drawable)
			: m_texture(Forge::move(texture))
			, m_drawable(drawable) {}

		Texture const& texture() const final;
		void present(Receipt const& wait_on) final;

	private:
		SharedPtr<Texture> m_texture;
		Core::Protocol m_drawable; // CAMetalDrawable
	};
} // namespace Forge::GPU
