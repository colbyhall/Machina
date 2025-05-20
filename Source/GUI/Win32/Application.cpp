/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <GUI/Application.hpp>

#include <Core/Async/Scheduler.hpp>
#include <Core/Windows.hpp>

namespace Forge::GUI {
	static LRESULT CALLBACK window_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
		return DefWindowProcW(hwnd, msg, wparam, lparam);
	}

	const LPCWSTR window_class_name = L"WindowClassName";

	Application::Application(const Core::Scheduler& scheduler, const GPU::Device& device)
		: m_scheduler(scheduler)
		, m_device(device) {
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

	int Application::run(FunctionRef<void(Frame&)> tick) {
		static Core::Atomic<bool> running{ false };
		FORGE_ASSERT(running.exchange(true) == false);

		auto last = Core::Instant::now();
		u64 frame_count = 0;
		while (running.load()) {
			const auto now = Core::Instant::now();
			const auto delta_time = now.since(last).as_secs_f64();
			last = now;

			// poll input
			MSG msg = {};
			while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE)) {
				TranslateMessage(&msg);
				DispatchMessageA(&msg);
			}

			Frame frame{
				.index = frame_count,
				.delta_time = delta_time,
			};
			tick(frame);

			frame_count += 1;
		}

		return 0;
	}
} // namespace Forge::GUI
