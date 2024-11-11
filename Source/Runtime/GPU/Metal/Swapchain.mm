/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <GPU/Metal/Swapchain.hpp>
#include <GPU/Metal/Texture.hpp>

namespace Grizzly::GPU {
	Shared<Texture> MetalSwapchain::next_back_buffer() {
		@autoreleasepool {
			id<CAMetalDrawable> drawable = [m_layer nextDrawable];
			id<MTLTexture> texture = drawable.texture;
			return Shared<MetalTexture>::create(
				Texture::CreateInfo{
					.usage = Texture::Usage::Swapchain,
					.format = Texture::Format::BGR_U8_SRGB,
					.size = { static_cast<u32>(drawable.texture.width),
							  static_cast<u32>(drawable.texture.height),
							  static_cast<u32>(drawable.texture.depth) },
				},
				texture);
		}
	}
} // namespace Grizzly::GPU
