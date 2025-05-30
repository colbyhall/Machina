/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Async/Win32/Thread.hpp>

#include <Core/Memory.hpp>

namespace Mach::Core {
	thread_local Option<Mach::SharedPtr<Thread>> g_current_thread = nullopt;

	Thread const& Thread::current() {
		if (!g_current_thread.is_set()) {
			Win32Thread thread(::GetCurrentThread(), ::GetCurrentThreadId());
			g_current_thread = Mach::SharedPtr<Win32Thread>::create(Mach::move(thread));
		}
		return *g_current_thread.as_ref().unwrap();
	}

	static DWORD WINAPI ThreadProc(_In_ LPVOID lpParameter) {
		auto* param = static_cast<Thread::Function*>(lpParameter);
		(*param)();
		param->~Function();
		Memory::free(param);
		return 0;
	}

	Mach::SharedPtr<Thread> Thread::spawn(Function&& f, const SpawnInfo& info) {
		DWORD flags = 0;
		if (info.start_suspended) {
			flags |= CREATE_SUSPENDED;
		}

		auto param = Memory::alloc(Memory::Layout::single<Thread::Function>());
		Memory::emplace<Thread::Function>(param, Mach::move(f));

		auto stack_size = info.stack_size.unwrap_or(0);

		DWORD id;
		const HANDLE thread = ::CreateThread(nullptr, stack_size, &ThreadProc, static_cast<LPVOID>(param), flags, &id);

		return Mach::SharedPtr<Win32Thread>::create(thread, id);
	}

	void Win32Thread::join() {
		WaitForSingleObject(m_thread, INFINITE);
		m_thread = nullptr;
	}

	void Win32Thread::detach() {
		CloseHandle(m_thread);
		m_thread = nullptr;
	}

	Thread::Id Win32Thread::id() const { return static_cast<Thread::Id>(m_id); }

	Win32Thread::~Win32Thread() {
		if (m_thread != nullptr) {
			join();
		}
	}
} // namespace Mach::Core
