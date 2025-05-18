/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Async/Fiber.hpp>
#include <Core/Containers/Unique.hpp>

#if FORGE_CPU == FORGE_CPU_ARM
namespace Forge::Core {
	class AARCH64Fiber final : public Fiber {
	public:
		struct Registers {
			u64 x[9];
			u64 sp;
			u64 pc;
		};
		explicit AARCH64Fiber(Registers&& registers) : Fiber(), m_registers(Forge::move(registers)) {}
		explicit AARCH64Fiber(Registers&& registers, Unique<u8[]>&& stack)
			: Fiber()
			, m_registers(Forge::move(registers))
			, m_stack(Forge::move(stack)) {}

		// Fiber Interface
		void switch_to() const final;
		// ~Fiber Interface

	private:
		Registers m_registers;
		Option<Unique<u8[]>> m_stack;
	};
} // namespace Forge::Core
#endif
