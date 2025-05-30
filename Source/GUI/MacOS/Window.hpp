/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <GUI/Window.hpp>

#if MACH_LANGUAGE == MACH_LANGUAGE_OBJCPP
	#import <AppKit/AppKit.h>

namespace Mach::GUI {
	class MacOSWindow final : public Window {
	public:
		explicit MacOSWindow(NSWindow* window, UniquePtr<GPU::Swapchain>&& swapchain)
			: m_window{ window }
			, m_swapchain(Mach::forward<UniquePtr<GPU::Swapchain>>(swapchain)) {}

		// Window interface
		bool close() final;
		bool show() final;
		bool hide() final;
		bool maximize() final;
		bool minimize() final;
		GPU::Swapchain& swapchain() const final;
		Bounds viewport() const final;
		Point cursor_position() const final;
		Real scale_factor() const final;
		// ~Window interface

	private:
		NSWindow* m_window;
		UniquePtr<GPU::Swapchain> m_swapchain;
	};
} // namespace Mach::GUI

#endif // MACH_LANGUAGE == MACH_LANGUAGE_OBJCPP
