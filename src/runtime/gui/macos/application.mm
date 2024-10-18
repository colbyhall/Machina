/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <gui/application.h>
#include <gui/macos/window.h>

#import <AppKit/AppKit.h>

namespace grizzly::gui {
	Application::Application(int argc, char** argv) { [NSApplication sharedApplication]; }

	Shared<Window> Application::spawn_window(const Window::SpawnInfo& spawn_info) {
		@autoreleasepool {
			const NSRect size = NSMakeRect(0.f, 0.f, spawn_info.size.as<f32>().x, spawn_info.size.as<f32>().y);
			NSWindow* window =
				[[NSWindow alloc] initWithContentRect:size
											styleMask:(NSWindowStyleMaskTitled | NSWindowStyleMaskClosable |
													   NSWindowStyleMaskResizable | NSWindowStyleMaskMiniaturizable)
											  backing:NSBackingStoreBuffered
												defer:NO];

			[window setTitle:@"Foo"];
			[window makeKeyAndOrderFront:nil];

			return Shared<MacOSWindow>::create(window);
		}
	}

	int Application::run() {
		while (m_running) {
			poll_input();
			tick();
		}
		return 0;
	}

	void Application::poll_input() {
		@autoreleasepool {
			NSEvent* ev;
			do {
				ev = [NSApp nextEventMatchingMask:NSEventMaskAny untilDate:nil inMode:NSDefaultRunLoopMode dequeue:YES];
				if (ev) {
					// handle events here
					[NSApp sendEvent:ev];
				}
			} while (ev);
		}
	}

	void Application::tick() {}
} // namespace grizzly::gui