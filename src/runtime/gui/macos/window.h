/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <gui/window.h>
#import <AppKit/AppKit.h>

namespace grizzly::gui {
	class MacOSWindow final : public Window {
	public:
		explicit MacOSWindow(NSWindow* window) : m_window{ window } {}
		MacOSWindow(const MacOSWindow&) = delete;
		MacOSWindow& operator=(const MacOSWindow&) = delete;
		MacOSWindow(MacOSWindow&& move) : m_window{ move.m_window } {}
		MacOSWindow& operator=(MacOSWindow&& move) {
			auto to_destroy = grizzly::move(*this);
			GRIZZLY_UNUSED(to_destroy);

			m_window = move.m_window;

			return *this;
		}
		bool close() final;
		bool show(Visibility) final;
		~MacOSWindow() final;

	private:
		NSWindow* m_window;
	};
} // namespace grizzly::gui
