/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Async/Fiber.hpp>
#include <Core/Debug/Log.hpp>

namespace Grizzly::Core {
	thread_local Option<Arc<Fiber>> g_current_fiber = nullopt;

	static void fiber_entry(Fiber::Function* f) {
		(*f)();
		Memory::free(f);
	}

	Arc<Fiber> Fiber::spawn(Function&& f) { return Fiber::spawn(Grizzly::move(f), SpawnInfo{}); }

	Arc<Fiber> Fiber::spawn(Function&& f, SpawnInfo const& spawn_info) {
		const auto stack_size = spawn_info.stack_size;
		auto stack = Unique<u8[]>::create(stack_size);

		auto param = Memory::alloc(Memory::Layout::single<Function>());
		Memory::emplace<Function>(param, Grizzly::move(f));

		// Setup the stack to call the function in spawn info
		auto registers = Registers{};
		registers.sp = reinterpret_cast<u64>(stack.begin()) + stack_size;
		registers.pc = reinterpret_cast<u64>(&fiber_entry);
		registers.x[0] = reinterpret_cast<u64>(*param);

		Fiber fiber{ Grizzly::move(registers), Grizzly::move(stack) };
		return Arc<Fiber>::create(Grizzly::move(fiber));
	}

	Fiber const& Fiber::current() {
		if (!g_current_fiber.is_set()) {
			Fiber fiber{ Registers{} };
			g_current_fiber = Arc<Fiber>::create(Grizzly::move(fiber));
		}
		return *g_current_fiber.as_ref().unwrap();
	}

	void Fiber::switch_to() {
		auto& current_fiber = current();
		g_current_fiber = to_shared();
		auto* current_registers = &current_fiber.m_registers;
		auto* next_registers = &m_registers;
#if GRIZZLY_CPU == GRIZZLY_CPU_ARM
		asm volatile(
			R"(
			stp x0, x1, [%0, #0]
			stp x2, x3, [%0, #16]
			stp x4, x5, [%0, #32]
			stp x6, x7, [%0, #48]
			str x8, [%0, #64]
			mov x19, sp
			str x19, [%0, #72]
			adrp x19, next@PAGE
			add x19, x19, next@PAGEOFF
			str x19, [%0, #80]
			ldp x0, x1, [%1, #0]
			ldp x2, x3, [%1, #16]
			ldp x4, x5, [%1, #32]
			ldp x6, x7, [%1, #48]
			ldr x8, [%1, #64]
			ldr x19, [%1, #72]
			mov sp, x19
			ldr x30, [%1, #80]
			br x30
			next:
			)"
			: /* No outputs */
			: "r"(current_registers), "r"(next_registers)
			: "x19", "memory");
#elif GRIZZLY_CPU == GRIZZLY_CPU_X86
#else
	#error "Unsupported CPU architecture"
#endif
	}
} // namespace Grizzly::Core
