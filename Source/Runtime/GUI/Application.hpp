/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Containers/Shared.hpp>
#include <Core/Containers/Unique.hpp>
#include <GPU/Device.hpp>

namespace Grizzly::GUI {
	class Application {
	public:
		static Unique<Application> create(GPU::Device const& device);
		static Application& the();

		GRIZZLY_ALWAYS_INLINE GPU::Device& device() { return *m_device; }
		int run();

	private:
		explicit Application(GPU::Device const& device) : m_device(device.to_shared()) {}
		void poll_input();
		void tick();

		static Application* s_instance;

		bool m_running = true;
		Shared<GPU::Device> m_device;
	};
} // namespace Grizzly::GUI
