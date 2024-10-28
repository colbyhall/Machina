/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include "Core/Debug/Assertions.hpp"
#include <GUI/Application.hpp>
#include <GUI/MacOS/Window.hpp>

#import <AppKit/AppKit.h>

@interface GrizzlyApplicationDelegate : NSObject <NSApplicationDelegate>
@end

@implementation GrizzlyApplicationDelegate

@end // GrizzlyApplicationDelegate
namespace Grizzly::GUI {
	Application* Application::s_instance = nullptr;

	Unique<Application> Application::create(const GPU::Device& device) {
		// Allow only one instance of Application
		GRIZZLY_ASSERT(s_instance == nullptr);

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

			Application app(device);
			auto result = Unique<Application>::create(Grizzly::move(app));
			s_instance = result;
			return result;
		};
	}

	Application& Application::the() {
		GRIZZLY_ASSERT(s_instance != nullptr);
		return *s_instance;
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
} // namespace Grizzly::GUI
