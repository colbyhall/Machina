/**
 * copyright (c) 2024 colby hall <me@cobeh.com>
 *
 * this software is released under the mit license.
 */

#pragma once

#import <Metal/Metal.h>

#include <Core/ObjectiveC/Protocol.hpp>
#include <GPU/Shader.hpp>

namespace Grizzly::GPU {
	class MetalLibrary final : public Library {
	public:
	private:
		Core::Protocol m_library; // MTLLibrary
	};

	class MetalVertexShader final : public VertexShader {
	public:
	private:
		Core::Protocol m_function; // MTLFunction
	};

	class MetalPixelShader final : public PixelShader {
	public:
	private:
		Core::Protocol m_function; // MTLFunction
	};
} // namespace Grizzly::GPU