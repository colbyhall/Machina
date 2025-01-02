/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <GPU/CommandList.hpp>
#include <GPU/Metal/Swapchain.hpp>
#include <GPU/Metal/Texture.hpp>

namespace Grizzly::GPU {
	Unique<Backbuffer> MetalSwapchain::next_back_buffer() {
		@autoreleasepool {
			id<CAMetalDrawable> drawable = [m_layer nextDrawable];
			auto texture = Shared<MetalTexture>::create(
				Texture::CreateInfo{
					.usage = Texture::Usage::Swapchain,
					.format = Texture::Format::BGRA_U8_SRGB,
					.size = { static_cast<u32>(drawable.texture.width),
							  static_cast<u32>(drawable.texture.height),
							  static_cast<u32>(drawable.texture.depth) },
				},
				drawable.texture);
			return Unique<MetalBackbuffer>::create(Grizzly::move(texture), drawable);
		}
	}

	Texture const& MetalBackbuffer::texture() const { return *m_texture; }

	void MetalBackbuffer::present(Receipt const& wait_on) {
		@autoreleasepool {
			wait_on.wait_until_complete();
			[m_drawable present];
		}
	}
} // namespace Grizzly::GPU
