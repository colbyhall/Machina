/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <GUI/Application.hpp>
#include <GUI/Win32/Window.hpp>

namespace Grizzly::GUI {
	static LRESULT CALLBACK window_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
		return DefWindowProcW(hwnd, msg, wparam, lparam);
	}

	const LPCWSTR window_class_name = L"WindowClassName";

	Application Application::create(const GPU::Device& device) {
		::WNDCLASSEXW window_class = {};
		window_class.cbSize = sizeof(::WNDCLASSEXW);
		window_class.style = CS_HREDRAW | CS_VREDRAW;
		window_class.lpfnWndProc = window_proc;
		window_class.hInstance = ::GetModuleHandleA(nullptr);
		window_class.hCursor = ::LoadCursor(nullptr, IDC_ARROW);
		window_class.lpszClassName = window_class_name;

		if (::RegisterClassExW(&window_class) == 0) {
			GRIZZLY_PANIC("Failure when registering window class.");
		}

		return Application(device);
	}

	int Application::run(FunctionRef<void()> tick) {
		while (m_running) {
			poll_input();
			tick();
		}
		return 0;
	}

	void Application::poll_input() {
		MSG msg = {};
		while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
	}
} // namespace Grizzly::GUI
