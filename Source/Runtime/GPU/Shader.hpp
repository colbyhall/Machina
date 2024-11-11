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

	class VertexShader : public SharedFromThis<VertexShader> {
	public:
		virtual ~VertexShader() {}
	};

	class PixelShader : public SharedFromThis<PixelShader> {
	public:
		virtual ~PixelShader() {}
	};
} // namespace Grizzly::GPU
