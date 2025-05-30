/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <GPU/CommandList.hpp>
#include <GPU/Drivers/Metal/Swapchain.hpp>
#include <GPU/Drivers/Metal/Texture.hpp>

namespace Mach::GPU {
	UniquePtr<Backbuffer> MetalSwapchain::next_back_buffer() {
		@autoreleasepool {
			id<CAMetalDrawable> drawable = [m_layer nextDrawable];
			auto texture = SharedPtr<MetalTexture>::create(
				Texture::CreateInfo{
					.usage = Texture::Usage::Swapchain,
					.format = Format::BGRA_U8_SRGB,
					.size = { static_cast<u32>(drawable.texture.width),
							  static_cast<u32>(drawable.texture.height),
							  static_cast<u32>(drawable.texture.depth) },
				},
				drawable.texture);
			return UniquePtr<MetalBackbuffer>::create(Mach::move(texture), drawable);
		}
	}

	Texture const& MetalBackbuffer::texture() const { return *m_texture; }

	void MetalBackbuffer::present(Receipt const& wait_on) {
		@autoreleasepool {
			wait_on.wait_until_complete();
			[m_drawable present];
		}
	}
} // namespace Mach::GPU
