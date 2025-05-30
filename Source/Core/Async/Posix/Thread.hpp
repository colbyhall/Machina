/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Async/Thread.hpp>

#include <pthread.h>

namespace Mach::Core {
	class PosixThread final : public Thread {
	public:
		explicit PosixThread(pthread_t thread) : m_thread(thread) {}
		PosixThread(const PosixThread&) = delete;
		PosixThread& operator=(const PosixThread&) = delete;
		PosixThread(PosixThread&& move) : m_thread(move.m_thread) { move.m_thread = nullptr; }
		PosixThread& operator=(PosixThread&& move) {
			auto to_destroy = Mach::move(*this);
			MACH_UNUSED(to_destroy);

			m_thread = move.m_thread;
			move.m_thread = nullptr;

			return *this;
		}

		void join() final;
		void detach() final;
		Id id() const final;
		~PosixThread() final;
		MACH_ALWAYS_INLINE bool is_ready() const { return m_ready.load(); }

	private:
		friend class Thread;
		pthread_t m_thread;
		Atomic<bool> m_ready{ false };
	};
} // namespace Mach::Core
