/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Gui/Window.hpp>

#include <windows.h>

namespace Grizzly::Gui {
	class Win32Window final : public Window {
	public:
		explicit Win32Window(HWND handle) : m_handle{ handle } {}
		Win32Window(const Win32Window&) = delete;
		Win32Window& operator=(const Win32Window&) = delete;
		Win32Window(Win32Window&& move) : m_handle{ move.m_handle } { move.m_handle = nullptr; }
		Win32Window& operator=(Win32Window&& move) {
			auto to_destroy = Grizzly::move(*this);
			GRIZZLY_UNUSED(to_destroy);

			m_handle = move.m_handle;
			move.m_handle = nullptr;

			return *this;
		}
		~Win32Window() final;

		bool close() final;
		bool show(Visibility visibility) final;

	private:
		HWND m_handle;
	};
} // namespace Grizzly::Gui
