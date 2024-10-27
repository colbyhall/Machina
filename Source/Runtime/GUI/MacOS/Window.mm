/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <GUI/MacOS/Window.hpp>
#include <GUI/Window.hpp>

namespace Grizzly::GUI {
	MacOSWindow::~MacOSWindow() {
		@autoreleasepool {
			[m_window release];
		}
	}

	bool MacOSWindow::close() {
		@autoreleasepool {
			[m_window close];
		}
		return true;
	}

	bool MacOSWindow::show() {
		@autoreleasepool {
			[m_window makeKeyAndOrderFront:nil];
		}
		return true;
	}

	bool MacOSWindow::hide() {
		@autoreleasepool {
			[m_window orderOut:nil];
		}
		return true;
	}

	bool MacOSWindow::minimize() {
		@autoreleasepool {
			[m_window miniaturize:nil];
		}
		return true;
	}

	bool MacOSWindow::maximize() {
		@autoreleasepool {
			[m_window zoom:nil];
		}
		return true;
	}
} // namespace Grizzly::GUI
