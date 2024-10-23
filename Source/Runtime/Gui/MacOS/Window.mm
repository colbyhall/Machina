/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Gui/MacOS/Window.h>

namespace Grizzly::Gui {
	MacOSWindow::~MacOSWindow() {
		@autoreleasepool {
			[m_window release];
		}
	}

	bool MacOSWindow::close() {
		// TODO: IMPLEMENT ME
		return false;
	}

	bool MacOSWindow::show(Visibility) {
		// TODO: IMPLEMENT ME
		return false;
	}
} // namespace Grizzly::Gui
