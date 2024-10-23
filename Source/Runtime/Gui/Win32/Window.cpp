/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Gui/Win32/Window.h>

namespace Grizzly::Gui {
	Win32Window::~Win32Window() {
		if (m_handle != nullptr) {
			::DestroyWindow(m_handle);
		}
	}

	bool Win32Window::close() { return ::CloseWindow(m_handle) > 0; }

	bool Win32Window::show(Visibility visibility) {
		int input = SW_SHOW;
		switch (visibility) {
		case Visibility::Maximized:
			input = SW_SHOWMAXIMIZED;
			break;
		case Visibility::Minimized:
			input = SW_SHOWMINIMIZED;
			break;
		case Visibility::Invisible:
			input = SW_HIDE;
			break;
		default:
			break;
		}
		return ::ShowWindow(m_handle, input) > 0;
	}
} // namespace Grizzly::Gui
