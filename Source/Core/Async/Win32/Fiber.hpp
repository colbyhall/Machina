/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Async/Fiber.hpp>
#include <Core/Windows.hpp>

namespace Mach::Core {
	class Win32Fiber final : public Fiber {
	public:
		explicit Win32Fiber(LPVOID handle, bool thread_owned = false)
			: m_thread_owned(thread_owned)
			, m_handle(handle) {}

		Win32Fiber(const Win32Fiber&) = delete;
		Win32Fiber& operator=(const Win32Fiber&) = delete;
		Win32Fiber(Win32Fiber&& m) : m_thread_owned(m.m_thread_owned), m_handle(m.m_handle) {
			m.m_thread_owned = false;
			m.m_handle = NULL;
		}
		Win32Fiber& operator=(Win32Fiber&& m) {
			auto to_destroy = Mach::move(*this);
			MACH_UNUSED(to_destroy);
			m_thread_owned = m.m_thread_owned;
			m_handle = m.m_handle;
			m.m_thread_owned = false;
			m.m_handle = NULL;
			return *this;
		}
		~Win32Fiber() final;

		// Fiber Interface
		void switch_to() const final;
		// ~Fiber Interface

	private:
		bool m_thread_owned = false;
		LPVOID m_handle;
	};
} // namespace Mach::Core
