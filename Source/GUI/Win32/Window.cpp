/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <GUI/Win32/Window.hpp>

#include <Core/Containers/WString.hpp>
#include <GPU/Device.hpp>
#include <GUI/Application.hpp>

namespace Forge::GUI {
	Rc<Window> Window::create(Application const& app, CreateInfo const& create_info) {
		const auto dpi = static_cast<f32>(::GetDpiForSystem()) / 96.f;

		const DWORD dwStyle = WS_OVERLAPPEDWINDOW;
		RECT adjusted_rect = {
			.left = 0,
			.top = 0,
			.right = (LONG)((f32)create_info.size.x * dpi),
			.bottom = (LONG)((f32)create_info.size.y * dpi),
		};
		AdjustWindowRect(&adjusted_rect, dwStyle, 0);

		const LONG width = adjusted_rect.right - adjusted_rect.left;
		const LONG height = adjusted_rect.bottom - adjusted_rect.top;

		const int monitor_width = GetSystemMetrics(SM_CXSCREEN);
		const int monitor_height = GetSystemMetrics(SM_CYSCREEN);

		const int x = monitor_width / 2 - width / 2;
		const int y = monitor_height / 2 - height / 2;

		auto wtitle = WString::from(create_info.title);

		HWND handle = ::CreateWindowExW(
			0,
			L"WindowClassName",
			wtitle.begin(),
			dwStyle,
			x,
			y,
			static_cast<int>(create_info.size.x),
			static_cast<int>(create_info.size.y),
			nullptr,
			nullptr,
			::GetModuleHandleW(nullptr),
			nullptr);
		// TODO: Error handling

		auto swapchain = app.device().create_swapchain(handle);
		auto result = Rc<Win32Window>::create(handle, Forge::move(swapchain));
		::SetWindowLongPtrW(handle, GWLP_USERDATA, (LONG_PTR) & *result);
		return result;
	}

	Win32Window::~Win32Window() {
		if (m_handle != nullptr) {
			::DestroyWindow(m_handle);
		}
	}

	bool Win32Window::close() { return ::CloseWindow(m_handle) > 0; }
	bool Win32Window::show() { return ::ShowWindow(m_handle, SW_SHOW) > 0; }
	bool Win32Window::hide() { return ::ShowWindow(m_handle, SW_HIDE) > 0; }
	bool Win32Window::maximize() { return ::ShowWindow(m_handle, SW_MAXIMIZE) > 0; }
	bool Win32Window::minimize() { return ::ShowWindow(m_handle, SW_MINIMIZE) > 0; }
	Vector2<f32> Win32Window::cursor_position() const { return { 0, 0 }; }
} // namespace Forge::GUI
