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

	class Library : public SharedFromThis<Library> {
	public:
		GRIZZLY_NO_DISCARD virtual Shared<VertexShader> create_vertex_shader(StringView function_name) = 0;
		GRIZZLY_NO_DISCARD virtual Shared<FragmentShader> create_fragment_shader(StringView function_name) = 0;

		virtual ~Library() {}
	};

	class VertexShader : public SharedFromThis<VertexShader> {
	public:
		virtual ~VertexShader() {}
	};

	class FragmentShader : public SharedFromThis<FragmentShader> {
	public:
		virtual ~FragmentShader() {}
	};
} // namespace Grizzly::GPU
