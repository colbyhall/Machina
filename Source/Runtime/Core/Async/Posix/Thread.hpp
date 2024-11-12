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

		void join() final;
		void detach() final;

	private:
		pthread_t m_thread;
	};
} // namespace Grizzly::Core