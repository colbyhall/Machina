/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <GUI/Window.hpp>

#if FORGE_LANGUAGE == FORGE_LANGUAGE_OBJCPP
	#import <AppKit/AppKit.h>

namespace Forge::GUI {
	class MacOSWindow final : public Window {
	public:
		explicit MacOSWindow(NSWindow* window, UniquePtr<GPU::Swapchain>&& swapchain)
			: m_window{ window }
			, m_swapchain(Forge::forward<UniquePtr<GPU::Swapchain>>(swapchain)) {}

		// Window interface
		bool close() final;
		bool show() final;
		bool hide() final;
		bool maximize() final;
		bool minimize() final;
		GPU::Swapchain& swapchain() const final;
		Bounds viewport() const final;
		Point cursor_position() const final;
		// ~Window interface

	private:
		NSWindow* m_window;
		UniquePtr<GPU::Swapchain> m_swapchain;
	};
} // namespace Forge::GUI

#endif // FORGE_LANGUAGE == FORGE_LANGUAGE_OBJCPP
