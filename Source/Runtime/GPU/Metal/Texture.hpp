/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * this software is released under the mit license.
 */

#pragma once

#import <Metal/Metal.h>

#include <Core/ObjectiveC/Protocol.hpp>
#include <GPU/Texture.hpp>

namespace Grizzly::GPU {
	class MetalTexture final : public Texture {
	public:
		explicit MetalTexture(Texture::CreateInfo const& create_info, id<MTLTexture> texture)
			: Texture(create_info)
			, m_texture(texture) {}

		Core::Protocol const& texture() const { return m_texture; }

	private:
		Core::Protocol m_texture; // MTLTexture
	};
} // namespace Grizzly::GPU