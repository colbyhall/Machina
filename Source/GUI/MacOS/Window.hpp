/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <GUI/Window.hpp>

#include <Core/Containers/UniquePtr.hpp>
#include <Core/ObjectiveC/Interface.hpp>
#include <GPU/Swapchain.hpp>

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

		GPU::Swapchain& swapchain() const final { return *m_swapchain; }
		Bounds viewport() const final;
		Point cursor_position() const final;
		// ~Window interface

	private:
		Core::Interface<NSWindow> m_window;
		UniquePtr<GPU::Swapchain> m_swapchain;
	};
} // namespace Forge::GUI
