/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Containers/Function.hpp>
#include <Core/Containers/SharedPtr.hpp>

namespace Mach::Core {
	class Fiber : public Mach::SharedPtrFromThis<Fiber> {
	public:
		using Function = Function<void()>;
		struct SpawnInfo {
			usize stack_size = 1024 * 1024;
		};
		static Mach::SharedPtr<Fiber> spawn(Function&& f);
		static Mach::SharedPtr<Fiber> spawn(Function&& f, SpawnInfo const& info);
		static Fiber const& current();

		virtual void switch_to() const = 0;
		virtual ~Fiber() {}
	};
} // namespace Mach::Core
