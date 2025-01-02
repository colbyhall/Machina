/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Debug/Assertions.hpp>
#include <Core/Time.hpp>
#include <GUI/Application.hpp>
#include <GUI/MacOS/Window.hpp>

#import <AppKit/AppKit.h>

@interface GrizzlyApplicationDelegate : NSObject <NSApplicationDelegate>
@end

@implementation GrizzlyApplicationDelegate

@end // GrizzlyApplicationDelegate
namespace Grizzly::GUI {
	Application Application::create(const GPU::Device& device) {
		@autoreleasepool {
			// Create the shared application
			[NSApplication sharedApplication];

			// Applications need to be activated so their window shows above others.
			[NSApp activate];

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

			return Application(device);
		};
	}

	int Application::run(FunctionRef<void(f64 delta_time)> tick) {
		auto last = Core::Instant::now();
		while (m_running) {
			const auto now = Core::Instant::now();
			const auto delta_time = now.since(last).as_secs_f64();
			last = now;

			poll_input();
			tick(delta_time);
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
} // namespace Grizzly::GUI
