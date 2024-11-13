/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Containers/Unique.hpp>
#include <GPU/Swapchain.hpp>
#include <GUI/Window.hpp>

#include <windows.h>

namespace Grizzly::GUI {
	class Win32Window final : public Window {
	public:
		explicit Win32Window(HWND handle, Unique<GPU::Swapchain>&& swapchain)
			: m_handle{ handle }
			, m_swapchain{ Grizzly::move(swapchain) } {}
		Win32Window(const Win32Window&) = delete;
		Win32Window& operator=(const Win32Window&) = delete;
		Win32Window(Win32Window&& move) : m_handle{ move.m_handle }, m_swapchain{ Grizzly::move(move.m_swapchain) } {
			move.m_handle = nullptr;
		}
		Win32Window& operator=(Win32Window&& move) {
			auto to_destroy = Grizzly::move(*this);
			GRIZZLY_UNUSED(to_destroy);

			m_handle = move.m_handle;
			m_swapchain = Grizzly::move(move.m_swapchain);
			move.m_handle = nullptr;

			return *this;
		}

		// Window interface
		bool close() final;
		bool show() final;
		bool hide() final;
		bool maximize() final;
		bool minimize() final;
		GPU::Swapchain& swapchain() final { return *m_swapchain; }
		~Win32Window() final;
		// ~Window interface

	private:
		HWND m_handle;
		Unique<GPU::Swapchain> m_swapchain;
	};
} // namespace Grizzly::GUI
