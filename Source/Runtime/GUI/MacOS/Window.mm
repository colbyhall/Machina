/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <GPU/Device.hpp>
#include <GUI/Application.hpp>
#include <GUI/MacOS/Window.hpp>
#include <GUI/Window.hpp>

namespace Grizzly::GUI {
	Shared<Window> Window::create(const CreateInfo& create_info) {
		@autoreleasepool {
			const NSRect size = NSMakeRect(0.f, 0.f, create_info.size.as<f32>().x, create_info.size.as<f32>().y);
			NSWindow* window =
				[[NSWindow alloc] initWithContentRect:size
											styleMask:(NSWindowStyleMaskTitled | NSWindowStyleMaskClosable |
													   NSWindowStyleMaskResizable | NSWindowStyleMaskMiniaturizable)
											  backing:NSBackingStoreBuffered
												defer:NO];

			const char* ctitle = (const char*)*create_info.title;
			NSString* title = [NSString stringWithUTF8String:ctitle];
			[window setTitle:title];
			[window center];
			[window setTabbingMode:NSWindowTabbingModeDisallowed];

			auto swapchain = Application::the().device().create_swapchain(window);

			// Result will take ownership of window
			[window retain];
			return Shared<MacOSWindow>::create(window, Grizzly::move(swapchain));
		}
	}

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
