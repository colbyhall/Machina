/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Async/Posix/Thread.hpp>
#include <Core/Debug/Log.hpp>

namespace Mach::Core {
	thread_local Option<Mach::SharedPtr<Thread>> g_current_thread = nullopt;

	Thread const& Thread::current() {
		if (!g_current_thread.is_set()) {
			PosixThread thread{ pthread_self() };
			g_current_thread = Mach::SharedPtr<PosixThread>::create(Mach::move(thread));
		}
		return *g_current_thread.as_ref().unwrap();
	}

	struct ThreadArg {
		Thread::Function f;
		Mach::SharedPtr<PosixThread> thread;
	};
	static void* posix_thread_main(void* arg) {
		auto* param = static_cast<ThreadArg*>(arg);
		g_current_thread = param->thread;

		// Spinlock until the thread is ready to execute. We have to wait until the m_thread value has been set in the
		// thread
		while (!param->thread->is_ready()) {
		}

		(param->f)();

		param->~ThreadArg();
		Memory::free(param);

		return nullptr;
	}

	Mach::SharedPtr<Thread> Thread::spawn(Function&& f, SpawnInfo const& info) {
		// TODO: Figure out how to use the data from info
		MACH_UNUSED(info);

		auto result = Mach::SharedPtr<PosixThread>::create(nullptr);
		auto& mut_result = result.unsafe_get_mut();

		auto param = Memory::alloc(Memory::Layout::single<ThreadArg>());
		Memory::emplace<ThreadArg>(param, ThreadArg{ .f = Mach::forward<Function>(f), .thread = result });

		pthread_t thread;
		const int create_result = pthread_create(&thread, nullptr, posix_thread_main, param);
		// TODO: Error handling
		MACH_UNUSED(create_result);

		mut_result.m_thread = thread;

		// Mark the thread as ready so it can execute.
		result->m_ready.store(true);

		return result;
	}

	void PosixThread::join() {
		const int result = pthread_join(m_thread, nullptr);
		// TODO: Error handling
		MACH_UNUSED(result);
		m_thread = nullptr;
	}

	void PosixThread::detach() {
		const int result = pthread_detach(m_thread);
		MACH_UNUSED(result);
		m_thread = nullptr;
	}

	Thread::Id PosixThread::id() const { return (Thread::Id)m_thread; }

	PosixThread::~PosixThread() {
		if (m_thread != nullptr) {
			// join();
		}
	}
} // namespace Mach::Core
