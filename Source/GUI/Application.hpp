/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Containers/Function.hpp>
#include <Core/Containers/SharedPtr.hpp>

#include <GUI/Builder.hpp>

namespace Mach {
	namespace Core {
		class Scheduler;
	}
	namespace GPU {
		class Device;
	}
} // namespace Mach

namespace Mach::GUI {
	class Frame {
	public:
		explicit Frame(u64 index, f64 delta_time, State& state)
			: m_index(index)
			, m_delta_time(delta_time)
			, m_state(state) {}

		MACH_ALWAYS_INLINE u64 index() const { return m_index; }
		MACH_ALWAYS_INLINE f64 delta_time() const { return m_delta_time; }

		bool window(StringView title, FunctionRef<void(Builder&)> f);

	private:
		u64 m_index;
		f64 m_delta_time;
		State& m_state;
	};

	class Application {
	public:
		explicit Application(const Core::Scheduler& scheduler, const GPU::Device& device);

		MACH_ALWAYS_INLINE Core::Scheduler const& scheduler() const { return m_scheduler; }

		void pump_events();
		int run(FunctionRef<void(Frame&)> f);

	private:
		Core::Scheduler const& m_scheduler;
		State m_state;
	};

} // namespace Mach::GUI
