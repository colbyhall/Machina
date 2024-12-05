/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Containers/Function.hpp>
#include <Core/Containers/Option.hpp>
#include <Core/Containers/Shared.hpp>
#include <Core/Containers/Slice.hpp>

namespace Grizzly::Core {
	class Fiber final : public AtomicSharedFromThis<Fiber> {
	public:
		using Function = Function<void()>;
		struct SpawnInfo {
			Option<usize> stack_size = nullopt;
		};
		static AtomicShared<Fiber> spawn(
			Function&& f,
			SpawnInfo const& spawn_info = {
				.stack_size = nullopt,
			});
		static AtomicShared<Fiber> current();

		Fiber(const Fiber&) = delete;
		Fiber& operator=(const Fiber&) = delete;
		Fiber(Fiber&&) = default;
		Fiber& operator=(Fiber&&) = default;

		void switch_to();

	private:
		struct Registers {
#if GRIZZLY_CPU == GRIZZLY_CPU_ARM
			u64 x[9];
			u64 sp;
			u64 pc;
#elif GRIZZLY_CPU == GRIZZLY_CPU_X86
			u64 rdi;
			u64 rsi;
			u64 rbx;
			u64 rbp;
			u64 r12;
			u64 r13;
			u64 r14;
			u64 r15;
			u64 rsp;
			u64 rip;
#else
	#error Unsupported CPU architecture
#endif
		};
		explicit Fiber(Registers&& registers, Slice<u8> stack)
			: m_registers(Grizzly::move(registers))
			, m_stack(stack) {}

		Registers m_registers;
		Slice<u8> m_stack;
	};
} // namespace Grizzly::Core
