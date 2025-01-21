/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Async/TaskManager.hpp>
#include <Core/Containers/Function.hpp>
#include <Core/Containers/Shared.hpp>
#include <Core/Containers/Unique.hpp>
#include <GPU/Device.hpp>

namespace Grizzly::GUI {
	class Application {
	public:
		explicit Application(Arc<Core::TaskManager>&& task_manager, Arc<GPU::Device>&& device);

		GRIZZLY_ALWAYS_INLINE Core::TaskManager const& task_manager() const { return *m_task_manager; }
		GRIZZLY_ALWAYS_INLINE GPU::Device const& device() const { return *m_device; }

		template <typename T>
		GRIZZLY_NO_DISCARD GRIZZLY_ALWAYS_INLINE Rc<T> create(T::CreateInfo const& create_info) const {
			return T::create(*this, create_info);
		}

	private:
		Arc<Core::TaskManager> m_task_manager;
		Arc<GPU::Device> m_device;
	};

	int run(Application const& application, FunctionRef<void(float delta_time)> tick);
} // namespace Grizzly::GUI
