/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <GUI/Window.hpp>

#include <Core/Containers/UniquePtr.hpp>
#include <Core/Windows.hpp>

namespace Mach::GUI {
	class Win32Window final : public Window {
	public:
		explicit Win32Window(HWND handle, UniquePtr<GPU::Swapchain>&& swapchain)
			: m_handle{ handle }
			, m_swapchain{ Mach::move(swapchain) } {}

		MACH_NO_COPY(Win32Window);
		Win32Window(Win32Window&& move) : m_handle{ move.m_handle }, m_swapchain{ Mach::move(move.m_swapchain) } {
			move.m_handle = nullptr;
		}
		Win32Window& operator=(Win32Window&& move) {
			auto to_destroy = Mach::move(*this);
			MACH_UNUSED(to_destroy);

			m_handle = move.m_handle;
			m_swapchain = Mach::move(move.m_swapchain);
			move.m_handle = nullptr;

			return *this;
		}

		// Window interface
		bool close() final;
		bool show() final;
		bool hide() final;
		bool maximize() final;
		bool minimize() final;
		GPU::Swapchain& swapchain() const final { return *m_swapchain; }
		Point cursor_position() const final;
		Bounds viewport() const final;
		~Win32Window() final;
		// ~Window interface

	private:
		HWND m_handle;
		UniquePtr<GPU::Swapchain> m_swapchain;
	};
} // namespace Mach::GUI
