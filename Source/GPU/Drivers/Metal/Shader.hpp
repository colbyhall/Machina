/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * this software is released under the mit license.
 */

#pragma once

#import <Metal/Metal.h>

#include <Core/ObjectiveC/Protocol.hpp>
#include <GPU/Shader.hpp>

namespace Mach::GPU {
	class MetalLibrary final : public Library {
	public:
		explicit MetalLibrary(id<MTLLibrary> library) : m_library(library) {}

		SharedPtr<VertexShader> create_vertex_shader(StringView function_name) const final;
		SharedPtr<FragmentShader> create_fragment_shader(StringView function_name) const final;

	private:
		Core::Protocol m_library; // MTLLibrary
	};

	class MetalVertexShader final : public VertexShader {
	public:
		explicit MetalVertexShader(id<MTLFunction> function) : m_function(function) {}

		MACH_ALWAYS_INLINE id<MTLFunction> function() const { return m_function; }

	private:
		Core::Protocol m_function; // MTLFunction
	};

	class MetalFragmentShader final : public FragmentShader {
	public:
		explicit MetalFragmentShader(id<MTLFunction> function) : m_function(function) {}

		MACH_ALWAYS_INLINE id<MTLFunction> function() const { return m_function; }

	private:
		Core::Protocol m_function; // MTLFunction
	};
} // namespace Mach::GPU
