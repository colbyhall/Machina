/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include "Core/Async/TaskManager.hpp"
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
	Application::Application(Arc<Core::TaskManager>&& task_manager, Arc<GPU::Device>&& device)
		: m_task_manager(Grizzly::forward<Arc<Core::TaskManager>>(task_manager))
		, m_device(Grizzly::forward<Arc<GPU::Device>>(device)) {

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

	int run(Application const& application, FunctionRef<void(float deta_time)> tick) {
		static Core::Atomic<bool> running{ false };

		GRIZZLY_ASSERT(running.exchange(true) == false);
		auto last = Core::Instant::now();
		while (running.load()) {
			const auto now = Core::Instant::now();
			const auto delta_time = now.since(last).as_secs_f64();
			last = now;

			// poll input
			@autoreleasepool {
				NSEvent* ev;
				do {
					ev = [NSApp nextEventMatchingMask:NSEventMaskAny
											untilDate:nil
											   inMode:NSDefaultRunLoopMode
											  dequeue:YES];
					if (ev) {
						// handle events here
						[NSApp sendEvent:ev];
					}
				} while (ev);
			}

			tick(delta_time);
		}

		return 0;
	}
} // namespace Grizzly::GUI
