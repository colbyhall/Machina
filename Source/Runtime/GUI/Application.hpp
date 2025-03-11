/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Async/Scheduler.hpp>
#include <Core/Containers/Function.hpp>
#include <Core/Containers/Shared.hpp>
#include <Core/Containers/Unique.hpp>
#include <GPU/Device.hpp>

namespace Grizzly::GUI {
	class Application {
	public:
		explicit Application(const Core::Scheduler& scheduler, const GPU::Device& device);

		GRIZZLY_ALWAYS_INLINE Core::Scheduler const& scheduler() const { return m_scheduler; }
		GRIZZLY_ALWAYS_INLINE GPU::Device const& device() const { return m_device; }

		template <typename T>
		GRIZZLY_NO_DISCARD GRIZZLY_ALWAYS_INLINE Rc<T> create(T::CreateInfo const& create_info) const {
			return T::create(*this, create_info);
		}

	private:
		Core::Scheduler const& m_scheduler;
		GPU::Device const& m_device;
	};

	int run(Application const& application, FunctionRef<void(float delta_time)> tick);
} // namespace Grizzly::GUI
