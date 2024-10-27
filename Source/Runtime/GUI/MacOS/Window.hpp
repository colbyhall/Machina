/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Containers/Unique.hpp>
#include <GPU/Swapchain.hpp>
#include <GUI/Window.hpp>

#import <AppKit/AppKit.h>

namespace Grizzly::GUI {
	class MacOSWindow final : public Window {
	public:
		explicit MacOSWindow(NSWindow* window, Unique<GPU::Swapchain>&& swapchain)
			: m_window{ window }
			, m_swapchain(Grizzly::forward<Unique<GPU::Swapchain>>(swapchain)) {}
		MacOSWindow(const MacOSWindow&) = delete;
		MacOSWindow& operator=(const MacOSWindow&) = delete;
		MacOSWindow(MacOSWindow&& move) : m_window{ move.m_window }, m_swapchain(Grizzly::move(move.m_swapchain)) {}
		MacOSWindow& operator=(MacOSWindow&& move) {
			auto to_destroy = Grizzly::move(*this);
			GRIZZLY_UNUSED(to_destroy);

			m_window = move.m_window;
			m_swapchain = Grizzly::move(move.m_swapchain);

			return *this;
		}

		// Window interface
		~MacOSWindow() final;
		bool close() final;
		bool show() final;
		bool hide() final;
		bool maximize() final;
		bool minimize() final;
		GPU::Swapchain& swapchain() final { return *m_swapchain; }
		// ~Window interface

	private:
		NSWindow* m_window;
		Unique<GPU::Swapchain> m_swapchain;
	};
} // namespace Grizzly::GUI
