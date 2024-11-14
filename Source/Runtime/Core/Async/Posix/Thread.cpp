/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Async/Posix/Thread.hpp>
#include <Core/Debug/Log.hpp>

namespace Grizzly::Core {
	static void* posix_thread_main(void* arg) {
		auto* param = static_cast<Thread::SpawnInfo::Function*>(arg);
		(*param)();
		param->~Function();
		Memory::free(param);
		return nullptr;
	}

	AtomicShared<Thread> Thread::spawn(SpawnInfo&& info) {
		auto param = Memory::alloc(Memory::Layout::single<SpawnInfo::Function>());
		Memory::emplace<SpawnInfo::Function>(param, Grizzly::move(info.f));

		pthread_t thread;
		const int result = pthread_create(&thread, nullptr, posix_thread_main, param);
		// TODO: Error handling
		GRIZZLY_UNUSED(result);
		return AtomicShared<PosixThread>::create(thread);
	}

	void PosixThread::join() {
		const int result = pthread_join(m_thread, nullptr);
		// TODO: Error handling
		GRIZZLY_UNUSED(result);
		m_thread = nullptr;
	}

	void PosixThread::detach() {
		const int result = pthread_detach(m_thread);
		GRIZZLY_UNUSED(result);
		m_thread = nullptr;
	}

	PosixThread::~PosixThread() {
		if (m_thread != nullptr) {
			join();
		}
	}
} // namespace Grizzly::Core
