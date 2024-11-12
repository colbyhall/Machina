/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Async/Posix/Thread.hpp>

namespace Grizzly::Core {
	static void* posix_thread_main(void* arg) { return nullptr; }

	AtomicShared<Thread> Thread::spawn(Function<void()> f) {
		pthread_t thread;
		const int result = pthread_create(&thread, nullptr, posix_thread_main, nullptr);
		// TODO: Error handling
		GRIZZLY_UNUSED(result);

		return AtomicShared<PosixThread>::create(thread);
	}

	void PosixThread::join() {
		const int result = pthread_join(m_thread, nullptr);
		// TODO: Error handling
		GRIZZLY_UNUSED(result);
	}

	void PosixThread::detach() {
		const int result = pthread_detach(m_thread);
		GRIZZLY_UNUSED(result);
	}
} // namespace Grizzly::Core
