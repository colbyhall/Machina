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
		explicit MetalLibrary(id<MTLLibrary> library) : m_library(library) {}

		Shared<VertexShader> create_vertex_shader(StringView function_name) final;
		Shared<PixelShader> create_pixel_shader(StringView function_name) final;

	private:
		Core::Protocol m_library; // MTLLibrary
	};

	class MetalVertexShader final : public VertexShader {
	public:
		explicit MetalVertexShader(id<MTLFunction> function) : m_function(function) {}

	private:
		Core::Protocol m_function; // MTLFunction
	};

	class MetalPixelShader final : public PixelShader {
	public:
		explicit MetalPixelShader(id<MTLFunction> function) : m_function(function) {}

	private:
		Core::Protocol m_function; // MTLFunction
	};
} // namespace Grizzly::GPU
