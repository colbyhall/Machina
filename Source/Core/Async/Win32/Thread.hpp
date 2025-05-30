/**
 * Copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Async/Thread.hpp>
#include <Core/Windows.hpp>

namespace Mach::Core {
	class Win32Thread final : public Thread {
	public:
		explicit Win32Thread(HANDLE thread, DWORD id) : m_thread(thread), m_id(id) {}

		MACH_NO_COPY(Win32Thread);
		Win32Thread(Win32Thread&& move) : m_thread(move.m_thread), m_id(move.m_id) { move.m_thread = nullptr; }
		Win32Thread& operator=(Win32Thread&& move) {
			auto to_destroy = Mach::move(*this);
			MACH_UNUSED(to_destroy);
			m_thread = move.m_thread;
			m_id = move.m_id;
			move.m_thread = nullptr;
			return *this;
		}

		// Thread interface
		void join() final;
		void detach() final;
		Id id() const final;
		~Win32Thread() final;
		// ~Thread interface

	private:
		HANDLE m_thread;
		DWORD m_id;
	};
} // namespace Mach::Core
