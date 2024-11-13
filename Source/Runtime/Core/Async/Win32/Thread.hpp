/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Async/Thread.hpp>

#include <windows.h>

namespace Grizzly::Core {
	class Win32Thread final : public Thread {
	public:
		explicit Win32Thread(HANDLE thread, DWORD id) : m_thread(thread), m_id(id) {}
		Win32Thread(const Win32Thread&) = delete;
		Win32Thread& operator=(const Win32Thread&) = delete;
		Win32Thread(Win32Thread&& move) : m_thread(move.m_thread), m_id(move.m_id) { move.m_thread = nullptr; }
		Win32Thread& operator=(Win32Thread&& move) {
			auto to_destroy = Grizzly::move(*this);
			GRIZZLY_UNUSED(to_destroy);
			m_thread = move.m_thread;
			m_id = move.m_id;
			move.m_thread = nullptr;
			return *this;
		}

		// Thread interface
		void join() final;
		void detach() final;
		~Win32Thread() final;
		// ~Thread interface

	private:
		HANDLE m_thread;
		DWORD m_id;
	};
} // namespace Grizzly::Core