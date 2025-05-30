/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * this software is released under the mit license.
 */

#pragma once

#include <Core/Containers/Option.hpp>
#include <Core/Primitives.hpp>

namespace Mach::GPU {
	using BindlessIndex = u32;

	class Resource {
	public:
		MACH_ALWAYS_INLINE Option<BindlessIndex> bindless() const { return m_bindless; }

		virtual ~Resource() {}

	protected:
		Option<BindlessIndex> m_bindless = nullopt;
	};
} // namespace Mach::GPU
