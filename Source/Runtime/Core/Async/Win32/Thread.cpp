/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Async/Win32/Thread.hpp>
#include <Core/Memory.hpp>

namespace Grizzly::Core {
	static DWORD WINAPI ThreadProc(_In_ LPVOID lpParameter) {
		auto* param = static_cast<Thread::SpawnInfo::Function*>(lpParameter);
		(*param)();
		param->~Function();
		Memory::free(param);
		return 0;
	}

	AtomicShared<Thread> Thread::spawn(SpawnInfo&& info) {
		auto flags = 0;
		if (info.start_suspended) {
			flags |= CREATE_SUSPENDED;
		}

		auto param = Memory::alloc(Memory::Layout::single<SpawnInfo::Function>());
		Memory::emplace<SpawnInfo::Function>(param, Grizzly::move(info.f));

		auto stack_size = info.stack_size.unwrap_or(0);

		DWORD id;
		const HANDLE thread = ::CreateThread(nullptr, stack_size, &ThreadProc, static_cast<LPVOID>(param), flags, &id);

		return AtomicShared<Win32Thread>::create(thread, id);
	}

	void Win32Thread::join() {
		WaitForSingleObject(m_thread, INFINITE);
		m_thread = nullptr;
	}

	void Win32Thread::detach() {
		CloseHandle(m_thread);
		m_thread = nullptr;
	}

	Win32Thread::~Win32Thread() {
		if (m_thread != nullptr) {
			join();
		}
	}
} // namespace Grizzly::Core
