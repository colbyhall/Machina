/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#import <AppKit/AppKit.h>
#include <GUI/Window.hpp>

namespace Grizzly::GUI {
	class MacOSWindow final : public Window {
	public:
		explicit MacOSWindow(NSWindow* window) : m_window{ window } {}
		MacOSWindow(const MacOSWindow&) = delete;
		MacOSWindow& operator=(const MacOSWindow&) = delete;
		MacOSWindow(MacOSWindow&& move) : m_window{ move.m_window } {}
		MacOSWindow& operator=(MacOSWindow&& move) {
			auto to_destroy = Grizzly::move(*this);
			GRIZZLY_UNUSED(to_destroy);

			m_window = move.m_window;

			return *this;
		}
		~MacOSWindow() final;

		bool close() final;
		bool show() final;
		bool hide() final;
		bool maximize() final;
		bool minimize() final;

	private:
		NSWindow* m_window;
	};
} // namespace Grizzly::GUI
