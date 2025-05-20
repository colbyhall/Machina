/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * this software is released under the mit license.
 */

#pragma once

#include <Core/Containers/SharedPtr.hpp>
#include <Core/Containers/String.hpp>

namespace Forge::GPU {
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

	class Library : public SharedPtrFromThis<Library> {
	public:
		FORGE_NO_DISCARD virtual SharedPtr<VertexShader> create_vertex_shader(StringView function_name) const = 0;
		FORGE_NO_DISCARD virtual SharedPtr<FragmentShader> create_fragment_shader(StringView function_name) const = 0;

		virtual ~Library() {}
	};

	class VertexShader : public SharedPtrFromThis<VertexShader> {
	public:
		virtual ~VertexShader() {}
	};

	class FragmentShader : public SharedPtrFromThis<FragmentShader> {
	public:
		virtual ~FragmentShader() {}
	};
} // namespace Forge::GPU
