/**
 * copyright (c) 2024 colby hall <me@cobeh.com>
 *
 * this software is released under the mit license.
 */

#pragma once

#include <Core/Containers/Shared.hpp>
#include <Core/Containers/Slice.hpp>

namespace Grizzly::GPU {
	class Library : public SharedFromThis<Library> {
	public:
		virtual ~Library() {}
	};

	class Shader : public SharedFromThis<Shader> {
	public:
		virtual ~Shader() {}
		virtual Library const& library() const = 0;
	};

	class VertexShader : public Shader {};

	class PixelShader : public Shader {};
} // namespace Grizzly::GPU
