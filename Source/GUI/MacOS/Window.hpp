/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#import <AppKit/AppKit.h>

#include <Core/Containers/UniquePtr.hpp>
#include <Core/ObjectiveC/Interface.hpp>
#include <GPU/Swapchain.hpp>
#include <GUI/Window.hpp>

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
		GPU::Swapchain& swapchain() final { return *m_swapchain; }
		Vector2<f32> cursor_position() const final;
		// ~Window interface

	private:
		Core::Interface<NSWindow> m_window;
		UniquePtr<GPU::Swapchain> m_swapchain;
	};
} // namespace Forge::GUI
