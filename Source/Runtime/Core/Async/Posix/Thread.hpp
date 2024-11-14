/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Async/Thread.hpp>

#include <pthread.h>

namespace Grizzly::Core {
	class PosixThread final : public Thread {
	public:
		explicit PosixThread(pthread_t thread) : m_thread(thread) {}
		PosixThread(const PosixThread&) = delete;
		PosixThread& operator=(const PosixThread&) = delete;
		PosixThread(PosixThread&& move) : m_thread(move.m_thread) { move.m_thread = nullptr; }
		PosixThread& operator=(PosixThread&& move) {
			auto to_destroy = Grizzly::move(*this);
			GRIZZLY_UNUSED(to_destroy);

			m_thread = move.m_thread;
			move.m_thread = nullptr;

			return *this;
		}

		void join() final;
		void detach() final;
		~PosixThread() final;

	private:
		pthread_t m_thread;
	};
} // namespace Grizzly::Core
