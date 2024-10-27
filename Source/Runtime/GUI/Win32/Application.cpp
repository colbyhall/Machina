/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Containers/WString.hpp>
#include <GUI/Application.hpp>
#include <GUI/Win32/Window.hpp>

namespace Grizzly::GUI {

	static LRESULT CALLBACK window_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
		return DefWindowProcW(hwnd, msg, wparam, lparam);
	}

	const LPCWSTR window_class_name = L"WindowClassName";

	Application::Application(int argc, char** argv) {
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
	}

	Shared<Window> Application::spawn_window(const Window::SpawnInfo& spawn_info) {
		const auto dpi = static_cast<f32>(::GetDpiForSystem()) / 96.f;

		const DWORD dwStyle = WS_OVERLAPPEDWINDOW;
		RECT adjusted_rect = {
			.left = 0,
			.top = 0,
			.right = (LONG)((f32)spawn_info.size.x * dpi),
			.bottom = (LONG)((f32)spawn_info.size.y * dpi),
		};
		AdjustWindowRect(&adjusted_rect, dwStyle, 0);

		const LONG width = adjusted_rect.right - adjusted_rect.left;
		const LONG height = adjusted_rect.bottom - adjusted_rect.top;

		const int monitor_width = GetSystemMetrics(SM_CXSCREEN);
		const int monitor_height = GetSystemMetrics(SM_CYSCREEN);

		const int x = monitor_width / 2 - width / 2;
		const int y = monitor_height / 2 - height / 2;

		auto wtitle = WString::from(spawn_info.title);

		HWND handle = ::CreateWindowExW(
			0,
			window_class_name,
			wtitle.begin(),
			dwStyle,
			x,
			y,
			spawn_info.size.x,
			spawn_info.size.y,
			nullptr,
			nullptr,
			::GetModuleHandleW(nullptr),
			nullptr);
		// TODO: Error handling

		auto result = Shared<Win32Window>::create(handle);
		::SetWindowLongPtrW(handle, GWLP_USERDATA, (LONG_PTR) & *result);
		return result;
	}

	int Application::run() {
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

	void Application::tick() {}
} // namespace Grizzly::GUI
