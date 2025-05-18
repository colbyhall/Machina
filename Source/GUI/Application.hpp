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
	class Application {
	public:
		explicit Application(const Core::Scheduler& scheduler, const GPU::Device& device);

		FORGE_ALWAYS_INLINE Core::Scheduler const& scheduler() const { return m_scheduler; }
		FORGE_ALWAYS_INLINE GPU::Device const& device() const { return m_device; }

		template <typename T>
		FORGE_NO_DISCARD FORGE_ALWAYS_INLINE Rc<T> create(T::CreateInfo const& create_info) const {
			return T::create(*this, create_info);
		}

	private:
		Core::Scheduler const& m_scheduler;
		GPU::Device const& m_device;
	};

	int run(Application const& application, FunctionRef<void(float delta_time)> tick);
} // namespace Forge::GUI
