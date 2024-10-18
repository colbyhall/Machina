/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <gui/macos/window.h>

namespace grizzly::gui {
	MacOSWindow::~MacOSWindow() {
		@autoreleasepool {
			[m_window release];
		}
	}
} // namespace grizzly::gui