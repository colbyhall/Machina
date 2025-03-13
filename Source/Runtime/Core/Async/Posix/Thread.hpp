/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Async/Thread.hpp>

#include <pthread.h>

namespace Forge::Core {
	class PosixThread final : public Thread {
	public:
		explicit PosixThread(pthread_t thread) : m_thread(thread) {}
		PosixThread(const PosixThread&) = delete;
		PosixThread& operator=(const PosixThread&) = delete;
		PosixThread(PosixThread&& move) : m_thread(move.m_thread) { move.m_thread = nullptr; }
		PosixThread& operator=(PosixThread&& move) {
			auto to_destroy = Forge::move(*this);
			FORGE_UNUSED(to_destroy);

			m_thread = move.m_thread;
			move.m_thread = nullptr;

			return *this;
		}

		void join() final;
		void detach() final;
		Id id() const final;
		~PosixThread() final;
		FORGE_ALWAYS_INLINE bool is_ready() const { return m_ready.load(); }

	private:
		friend class Thread;
		pthread_t m_thread;
		Atomic<bool> m_ready{ false };
	};
} // namespace Forge::Core
