/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <gui/application.h>
#include <gui/macos/window.h>

#import <AppKit/AppKit.h>

@interface GrizzlyApplicationDelegate : NSObject <NSApplicationDelegate>
@end

@implementation GrizzlyApplicationDelegate

@end // GrizzlyApplicationDelegate

namespace grizzly::gui {
	Application::Application(int argc, char** argv) {
		@autoreleasepool {
			[NSApplication sharedApplication];

			NSMenu* bar = [[NSMenu alloc] init];
			[NSApp setMainMenu:bar];

			NSMenuItem* app_menu_item = [bar addItemWithTitle:@"" action:NULL keyEquivalent:@""];
			NSMenu* app_menu = [[NSMenu alloc] initWithTitle:@"Sandbox"];
			[app_menu_item setSubmenu:app_menu];

			NSMenuItem* window_menu_item = [bar addItemWithTitle:@"" action:NULL keyEquivalent:@""];
			[bar release];
			NSMenu* window_menu = [[NSMenu alloc] initWithTitle:@"Window"];
			[NSApp setWindowsMenu:window_menu];
			[window_menu_item setSubmenu:window_menu];

			[window_menu addItemWithTitle:@"Minimize" action:@selector(performMiniaturize:) keyEquivalent:@"m"];
			[window_menu addItemWithTitle:@"Zoom" action:@selector(performZoom:) keyEquivalent:@""];
			[window_menu addItem:[NSMenuItem separatorItem]];
			[window_menu addItemWithTitle:@"Bring All to Front" action:@selector(arrangeInFront:) keyEquivalent:@""];

			[NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
		};
	}

	Shared<Window> Application::spawn_window(const Window::SpawnInfo& spawn_info) {
		@autoreleasepool {
			const NSRect size = NSMakeRect(0.f, 0.f, spawn_info.size.as<f32>().x, spawn_info.size.as<f32>().y);
			NSWindow* window =
				[[NSWindow alloc] initWithContentRect:size
											styleMask:(NSWindowStyleMaskTitled | NSWindowStyleMaskClosable |
													   NSWindowStyleMaskResizable | NSWindowStyleMaskMiniaturizable)
											  backing:NSBackingStoreBuffered
												defer:NO];

			const char* ctitle = (const char*)*spawn_info.title;
			NSString* title = [NSString stringWithUTF8String:ctitle];
			[window setTitle:title];
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
