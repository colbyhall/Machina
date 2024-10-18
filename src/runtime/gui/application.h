/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <core/containers/shared.h>
#include <gui/window.h>

namespace grizzly::gui {
	class Application {
	public:
		explicit Application(int argc, char** argv);

		Shared<Window> spawn_window(const Window::SpawnInfo& spawn_info);

		int run();

	private:
		void poll_input();
		void tick();

		bool m_running = true;
	};
} // namespace grizzly::gui
