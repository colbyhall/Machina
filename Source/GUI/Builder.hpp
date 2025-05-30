/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <GUI/Canvas.hpp>
#include <GUI/Layout.hpp>
#include <GUI/Math.hpp>
#include <GUI/State.hpp>

namespace Mach::GUI {
	class Builder {
	public:
		explicit Builder(State& state, const Layout& layout) : m_state(state), m_layout(layout) {}

		bool button(StringView text);

		MACH_ALWAYS_INLINE State& state() { return m_state; }
		MACH_ALWAYS_INLINE Layout& layout() { return m_layout; }
		MACH_ALWAYS_INLINE Canvas& canvas() { return m_canvas; }

	private:
		State& m_state;
		Layout m_layout;
		Canvas m_canvas;
	};
} // namespace Mach::GUI
