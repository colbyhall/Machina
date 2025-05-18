/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Async/Fiber.hpp>
#include <Core/Windows.hpp>

namespace Forge::Core {
	class Win32Fiber final : public Fiber {
	public:
		explicit Win32Fiber(LPVOID handle) : m_handle(handle) {}

		Win32Fiber(const Win32Fiber&) = delete;
		Win32Fiber& operator=(const Win32Fiber&) = delete;
		Win32Fiber(Win32Fiber&& m) : m_handle(m.m_handle) { m_handle = NULL; }
		Win32Fiber& operator=(Win32Fiber&& m) {
			auto to_destroy = Forge::move(*this);
			FORGE_UNUSED(to_destroy);
			m_handle = m.m_handle;
			m.m_handle = NULL;
			return *this;
		}
		~Win32Fiber() final;

		// Fiber Interface
		void switch_to() const final;
		// ~Fiber Interface

	private:
		LPVOID m_handle;
	};
} // namespace Forge::Core
