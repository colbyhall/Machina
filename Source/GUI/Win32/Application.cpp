/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <GUI/Application.hpp>

#include <Core/Async/Scheduler.hpp>
#include <Core/Windows.hpp>
#include <GPU/GraphicsPipeline.hpp>

namespace Forge::GUI {
	static LRESULT CALLBACK window_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
		return DefWindowProcW(hwnd, msg, wparam, lparam);
	}

	const LPCWSTR window_class_name = L"WindowClassName";

	Application::Application(const Core::Scheduler& scheduler, const GPU::Device& device)
		: m_scheduler(scheduler)
		, m_state(device) {
		::WNDCLASSEXW window_class = {};
		window_class.cbSize = sizeof(::WNDCLASSEXW);
		window_class.style = CS_HREDRAW | CS_VREDRAW;
		window_class.lpfnWndProc = window_proc;
		window_class.hInstance = ::GetModuleHandleA(nullptr);
		window_class.hCursor = ::LoadCursor(nullptr, IDC_ARROW);
		window_class.lpszClassName = window_class_name;

		if (::RegisterClassExW(&window_class) == 0) {
			FORGE_PANIC("Failure when registering window class.");
		}
	}

	void Application::pump_events() {
		MSG msg = {};
		while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
	}
} // namespace Forge::GUI
