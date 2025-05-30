/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <GUI/MacOS/Window.hpp>

#include <GPU/Device.hpp>
#include <GPU/Swapchain.hpp>
#include <GUI/Application.hpp>
#include <GUI/Window.hpp>

namespace Mach::GUI {
	UniquePtr<Window> Window::create(CreateInfo const& create_info) {
		@autoreleasepool {
			const NSRect size = NSMakeRect(0.f, 0.f, create_info.size.x, create_info.size.y);
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

			auto swapchain = create_info.device.create_swapchain(window);
			return UniquePtr<MacOSWindow>::create(window, Mach::move(swapchain));
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

	Bounds MacOSWindow::viewport() const {
		@autoreleasepool {
			const CGSize viewport_size = [[m_window contentView] frame].size;
			const auto inv_scale_factor = CGFloat(1); // / [m_window backingScaleFactor];
			return Bounds{ .min = Point(0),
						   .max =
							   Point(viewport_size.width * inv_scale_factor, viewport_size.height * inv_scale_factor) };
		}
	}

	GPU::Swapchain& MacOSWindow::swapchain() const { return *m_swapchain; }

	Point MacOSWindow::cursor_position() const {
		@autoreleasepool {
			NSPoint globalPoint = [NSEvent mouseLocation];
			NSPoint windowPoint = [m_window convertPointFromScreen:globalPoint];
			return { static_cast<Real>(windowPoint.x), static_cast<Real>(windowPoint.y) };
		}
	}

	Real MacOSWindow::scale_factor() const {
		const auto scale_factor = [m_window backingScaleFactor];
		return static_cast<Real>(scale_factor);
	}
} // namespace Mach::GUI
