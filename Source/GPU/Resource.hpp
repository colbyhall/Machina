/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * this software is released under the mit license.
 */

#pragma once

#include <Core/Containers/Option.hpp>
#include <Core/Primitives.hpp>

namespace Forge::GPU {
	class Resource {
	public:
		using BindlessIndex = u32;
		FORGE_ALWAYS_INLINE Option<BindlessIndex> bindless() const { return m_bindless; }

		virtual ~Resource() {}

	protected:
		Option<BindlessIndex> m_bindless = nullopt;
	};
} // namespace Forge::GPU
