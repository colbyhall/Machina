/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Containers/Shared.hpp>

namespace Grizzly::GUI {
	class Application {
	public:
		explicit Application(int argc, char** argv);

		int run();

	private:
		void poll_input();
		void tick();

		bool m_running = true;
	};
} // namespace Grizzly::GUI
