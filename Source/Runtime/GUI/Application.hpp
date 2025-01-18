/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Containers/Function.hpp>
#include <Core/Containers/Shared.hpp>
#include <Core/Containers/Unique.hpp>
#include <GPU/Device.hpp>

namespace Grizzly::GUI {
	class Application {
	public:
		static Application create(GPU::Device const& device);

		template <typename T>
		GRIZZLY_NO_DISCARD GRIZZLY_ALWAYS_INLINE Rc<T> create(T::CreateInfo const& info) {
			return T::create(*this, info);
		}

		GRIZZLY_ALWAYS_INLINE GPU::Device& device() { return *m_device; }
		int run(FunctionRef<void(f64 delta_time)> tick);

	private:
		explicit Application(GPU::Device const& device) : m_device(device.to_shared()) {}
		void poll_input();

		bool m_running = true;
		Rc<GPU::Device> m_device;
	};
} // namespace Grizzly::GUI
