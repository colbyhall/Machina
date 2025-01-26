/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * this software is released under the mit license.
 */

#pragma once

#include <Core/Containers/Shared.hpp>
#include <Core/Containers/String.hpp>

namespace Grizzly::GPU {
	class VertexShader;
	class FragmentShader;

	enum class ShaderLanguage : u8 {
		MSL,
		GLSL,
		HLSL,
	};

	struct ShaderSource {
		ShaderLanguage language;
		StringView text;
		Option<StringView> path = nullopt;
	};

	class Library : public ArcFromThis<Library> {
	public:
		GRIZZLY_NO_DISCARD virtual Arc<VertexShader> create_vertex_shader(StringView function_name) const = 0;
		GRIZZLY_NO_DISCARD virtual Arc<FragmentShader> create_fragment_shader(StringView function_name) const = 0;

		virtual ~Library() {}
	};

	class VertexShader : public ArcFromThis<VertexShader> {
	public:
		virtual ~VertexShader() {}
	};

	class FragmentShader : public ArcFromThis<FragmentShader> {
	public:
		virtual ~FragmentShader() {}
	};
} // namespace Grizzly::GPU
