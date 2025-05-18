/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Containers/Function.hpp>
#include <Core/Containers/Shared.hpp>

namespace Forge {
	namespace Core {
		class Scheduler;
	}
	namespace GPU {
		class Device;
	}
} // namespace Forge

namespace Forge::GUI {
	struct Frame {
		u64 index;
		f64 delta_time;
	};

	class Application {
	public:
		explicit Application(const Core::Scheduler& scheduler, const GPU::Device& device);

		FORGE_ALWAYS_INLINE Core::Scheduler const& scheduler() const { return m_scheduler; }
		FORGE_ALWAYS_INLINE GPU::Device const& device() const { return m_device; }

		int run(FunctionRef<void(Frame&)> f);

	private:
		Core::Scheduler const& m_scheduler;
		GPU::Device const& m_device;
	};

} // namespace Forge::GUI
