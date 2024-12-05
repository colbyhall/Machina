/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Containers/Function.hpp>
#include <Core/Containers/Option.hpp>
#include <Core/Containers/Shared.hpp>
#include <Core/Containers/Unique.hpp>

namespace Grizzly::Core {
	class Fiber final : public AtomicSharedFromThis<Fiber> {
	public:
		using Function = Function<void()>;
		struct SpawnInfo {
			usize stack_size = 1024 * 1024;
		};
		static AtomicShared<Fiber> spawn(Function&& f);
		static AtomicShared<Fiber> spawn(Function&& f, SpawnInfo const& info);
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
#else
	#error Unsupported CPU architecture
#endif
		};
		explicit Fiber(Registers&& registers) : m_registers(Grizzly::move(registers)) {}
		explicit Fiber(Registers&& registers, Unique<u8[]>&& stack)
			: m_registers(Grizzly::move(registers))
			, m_stack(Grizzly::move(stack)) {}

		Registers m_registers;
		Option<Unique<u8[]>> m_stack;
	};
} // namespace Grizzly::Core
