/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Async/Win32/Fiber.hpp>
#include <Core/Debug/Log.hpp>

namespace Mach::Core {
	thread_local Option<Mach::SharedPtr<Fiber>> g_current_fiber = nullopt;

	static void fiber_entry(LPVOID param) {
		Fiber::Function* const f = static_cast<Fiber::Function*>(param);
		(*f)();
		Memory::free(f);
	}

	Mach::SharedPtr<Fiber> Fiber::spawn(Function&& f, SpawnInfo const& spawn_info) {
		auto param = Memory::alloc(Memory::Layout::single<Function>());
		Memory::emplace<Function>(param, Mach::move(f));

		LPVOID handle = ::CreateFiber(static_cast<SIZE_T>(spawn_info.stack_size), fiber_entry, NULL);
		Win32Fiber fiber(handle);
		return Mach::SharedPtr<Win32Fiber>::create(Mach::move(fiber));
	}

	Fiber const& Fiber::current() {
		if (!g_current_fiber.is_set()) {
			LPVOID handle = ::ConvertThreadToFiber(NULL);

			Win32Fiber fiber(handle, true);
			g_current_fiber = Mach::SharedPtr<Win32Fiber>::create(Mach::move(fiber));
		}
		return *g_current_fiber.as_ref().unwrap();
	}

	Win32Fiber::~Win32Fiber() {
		if (m_handle != NULL && !m_thread_owned) {
			::DeleteFiber(m_handle);
			m_handle = NULL;
		}
	}

	void Win32Fiber::switch_to() const { ::SwitchToFiber(m_handle); }
} // namespace Mach::Core
