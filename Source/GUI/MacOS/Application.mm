/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Async/Scheduler.hpp>
#include <Core/Debug/Assertions.hpp>
#include <GPU/Device.hpp>
#include <GUI/Application.hpp>
#include <GUI/MacOS/Window.hpp>

#import <AppKit/AppKit.h>

@interface GrizzlyApplicationDelegate : NSObject <NSApplicationDelegate>
@end

@implementation GrizzlyApplicationDelegate

@end // GrizzlyApplicationDelegate
namespace Mach::GUI {
	Application::Application(const Core::Scheduler& scheduler, const GPU::Device& device)
		: m_scheduler(scheduler)
		, m_state(device) {
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
		};
	}

	void Application::pump_events() {
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

} // namespace Mach::GUI
