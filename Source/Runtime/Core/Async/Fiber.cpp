/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Async/Fiber.hpp>
#include <Core/Debug/Log.hpp>

namespace Grizzly::Core {
	thread_local Option<AtomicShared<Fiber>> g_current_fiber = nullopt;

	static void fiber_entry(Fiber::Function* f) {
		(*f)();
		Memory::free(f);
	}

	AtomicShared<Fiber> Fiber::spawn(Function&& f, SpawnInfo const& spawn_info) {
		const auto stack_size = spawn_info.stack_size.unwrap_or(1024 * 1024);
		auto stack_memory = Memory::alloc(Memory::Layout::array<u8>(stack_size));
		auto stack = Slice<u8>{ static_cast<u8*>(*stack_memory), stack_size };

		auto param = Memory::alloc(Memory::Layout::single<Function>());
		Memory::emplace<Function>(param, Grizzly::move(f));

		// Setup the stack to call the function in spawn info
		auto registers = Registers{};
		registers.sp = reinterpret_cast<u64>(stack.begin()) + stack_size;
		registers.pc = reinterpret_cast<u64>(&fiber_entry);
		registers.regs[0] = reinterpret_cast<u64>(*param);

		Fiber fiber{ Grizzly::move(registers), stack };
		return AtomicShared<Fiber>::create(Grizzly::move(fiber));
	}

	AtomicShared<Fiber> Fiber::current() {
		if (!g_current_fiber.is_set()) {
			Fiber fiber{ Registers{}, Slice<u8>{} };
			g_current_fiber = AtomicShared<Fiber>::create(Grizzly::move(fiber));
		}
		return g_current_fiber.as_ref().unwrap();
	}

	void Fiber::switch_to() {
		auto current_fiber = current();
		auto next_fiber = to_shared();
		g_current_fiber = next_fiber;
		auto* current_registers = &current_fiber->m_registers;
		auto* next_registers = &next_fiber->m_registers;
#if GRIZZLY_CPU == GRIZZLY_CPU_ARM
		asm volatile(
			// Save the current execution context into current_fiber registers
			"stp x0, x1, [%0, #0]      \n"
			"stp x2, x3, [%0, #16]     \n"
			"stp x4, x5, [%0, #32]     \n"
			"stp x6, x7, [%0, #48]     \n"
			"stp x8, x9, [%0, #64]     \n"
			"stp x10, x11, [%0, #80]   \n"
			"stp x12, x13, [%0, #96]   \n"
			"stp x14, x15, [%0, #112]  \n"
			"stp x16, x17, [%0, #128]  \n"
			"stp x18, x19, [%0, #144]  \n"
			"stp x20, x21, [%0, #160]  \n"
			"stp x22, x23, [%0, #176]  \n"
			"stp x24, x25, [%0, #192]  \n"
			"stp x26, x27, [%0, #208]  \n"
			"stp x28, x29, [%0, #224]  \n"
			// x19 is being used as a temp for the stack pointer and the instruction pointer
			"mov x19, sp 				\n"
			"str x19, [%0, #240] 		\n"
			// Make sure that the instruction pointer points to after we branch to next fibers current ip
			"adrp x19, next@PAGE		\n"
			"add x19, x19, next@PAGEOFF\n"
			"str x19, [%0, #248]		\n"
			// Restore x19 after we're done using it
			"ldr x19, [%0, #152]       \n"

			// Start loading next_fibers execution context into the registers
			"ldp x0, x1, [%1, #0]      \n"
			"ldp x2, x3, [%1, #16]     \n"
			"ldp x4, x5, [%1, #32]     \n"
			"ldp x6, x7, [%1, #48]     \n"
			// @HACK: x9 and x10 have been clobbered. I need to fix this before its finished
			"ldp x12, x13, [%1, #96]   \n"
			"ldp x14, x15, [%1, #112]  \n"
			"ldp x16, x17, [%1, #128]  \n"
			"ldp x18, x19, [%1, #144]  \n"
			"ldp x20, x21, [%1, #160]  \n"
			"ldp x22, x23, [%1, #176]  \n"
			"ldp x24, x25, [%1, #192]  \n"
			"ldp x26, x27, [%1, #208]  \n"
			"ldp x28, x29, [%1, #224]  \n"
			// @HACK: Patching x8 until earlier hack is finished
			"ldr x8, [%1, #64] 		\n"
			// Use x0 as a temp for the stack pointer
			"ldr x0, [%1, #240]		\n"
			"mov sp, x0       			\n"
			// Restore x0
			"ldr x0, [%1, #0]			\n"

			"ldr x30, [%1, #248]       \n"
			"br x30                    \n"
			"next: 					\n"
			: /* No outputs */
			: "r"(current_registers), "r"(next_registers)
			: "memory");
#elif GRIZZLY_CPU == GRIZZLY_CPU_X86
#else
	#error "Unsupported CPU architecture"
#endif
	}
} // namespace Grizzly::Core
