/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#import <AppKit/AppKit.h>

#include <Core/Containers/Unique.hpp>
#include <Core/ObjectiveC/Interface.hpp>
#include <GPU/Swapchain.hpp>
#include <GUI/Window.hpp>

namespace Grizzly::GUI {
	class MacOSWindow final : public Window {
	public:
		explicit MacOSWindow(NSWindow* window, Unique<GPU::Swapchain>&& swapchain)
			: m_window{ window }
			, m_swapchain(Grizzly::forward<Unique<GPU::Swapchain>>(swapchain)) {}

		// Window interface
		bool close() final;
		bool show() final;
		bool hide() final;
		bool maximize() final;
		bool minimize() final;
		GPU::Swapchain& swapchain() final { return *m_swapchain; }
		// ~Window interface

	private:
		Core::Interface<NSWindow> m_window;
		Unique<GPU::Swapchain> m_swapchain;
	};
} // namespace Grizzly::GUI
