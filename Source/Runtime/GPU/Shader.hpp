/**
 * copyright (c) 2024 colby hall <me@cobeh.com>
 *
 * this software is released under the mit license.
 */

#pragma once

#include <Core/Containers/Shared.hpp>
#include <Core/Containers/StringView.hpp>

namespace Grizzly::GPU {
    class VertexShader;
    class PixelShader;

	class Library : public SharedFromThis<Library> {
	public:
        GRIZZLY_NO_DISCARD virtual Shared<VertexShader> create_vertex_shader(StringView function_name) = 0;
        GRIZZLY_NO_DISCARD virtual Shared<PixelShader> create_pixel_shader(StringView function_name) = 0;

		virtual ~Library() {}
	};

	class VertexShader : public SharedFromThis<VertexShader> {
	public:
		virtual ~VertexShader() {}
	};

	class PixelShader : public SharedFromThis<PixelShader> {
	public:
		virtual ~PixelShader() {}
	};
} // namespace Grizzly::GPU
