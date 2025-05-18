/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Async/Scheduler.hpp>
#include <Core/Debug/Assertions.hpp>
#include <Core/Time.hpp>
#include <GPU/Device.hpp>
#include <GUI/Application.hpp>
#include <GUI/MacOS/Window.hpp>

#import <AppKit/AppKit.h>

@interface GrizzlyApplicationDelegate : NSObject <NSApplicationDelegate>
@end

@implementation GrizzlyApplicationDelegate

@end // GrizzlyApplicationDelegate
namespace Forge::GUI {
	Application::Application(const Core::Scheduler& scheduler, const GPU::Device& device)
		: m_scheduler(scheduler)
		, m_device(device) {
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

	int Application::run(FunctionRef<void(Frame&)> tick) {
		static Core::Atomic<bool> running{ false };
		FORGE_ASSERT(running.exchange(true) == false);

		auto last = Core::Instant::now();
		u64 frame_count = 0;
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

			auto frame = Frame{
				.index = frame_count,
				.delta_time = delta_time,
			};
			tick(frame);

			frame_count += 1;
		}

		return 0;
	}
} // namespace Forge::GUI
