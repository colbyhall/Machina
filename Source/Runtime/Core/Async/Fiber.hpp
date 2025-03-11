/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Containers/Function.hpp>
#include <Core/Containers/Option.hpp>
#include <Core/Containers/Shared.hpp>
#include <Core/Containers/Unique.hpp>

namespace Grizzly::Core {
	class Fiber final : public ArcFromThis<Fiber> {
	public:
		using Function = Function<void()>;
		struct SpawnInfo {
			usize stack_size = 1024 * 1024;
		};
		static Arc<Fiber> spawn(Function&& f);
		static Arc<Fiber> spawn(Function&& f, SpawnInfo const& info);
		static Fiber const& current();

		Fiber(const Fiber&) = delete;
		Fiber& operator=(const Fiber&) = delete;
		Fiber(Fiber&&) = default;
		Fiber& operator=(Fiber&&) = default;

		void switch_to() const;

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
		enum class State : u8 {
			InUse,
			Switching,
			Dormant,
		};
		explicit Fiber(Registers&& registers) : m_registers(Grizzly::move(registers)), m_state{ State::InUse } {}
		explicit Fiber(Registers&& registers, Unique<u8[]>&& stack)
			: m_registers(Grizzly::move(registers))
			, m_stack(Grizzly::move(stack))
			, m_state{ State::Dormant } {}

		Registers m_registers;
		Option<Unique<u8[]>> m_stack;
		Atomic<State> m_state{ State::Dormant };
	};
} // namespace Grizzly::Core
