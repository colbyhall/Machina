/**
 * copyright (c) 2024 colby hall <me@cobeh.com>
 *
 * this software is released under the mit license.
 */

#pragma once

#include <Core/Containers/Option.hpp>
#include <Core/Primitives.hpp>

namespace Grizzly::GPU {
	class Resource {
	public:
		using BindlessIndex = u32;
		virtual Option<BindlessIndex> bindless() const = 0;

		virtual ~Resource() {}
	};
} // namespace Grizzly::GPU
