/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Containers/String.hpp>
#include <Core/Debug/Log.hpp>
#include <GPU/Device.hpp>
#include <GUI/Application.hpp>
#include <GUI/Window.hpp>

using namespace Grizzly;
using namespace Core;

int main(int argc, char** argv) {
	auto device = GPU::Device::create({
		.backend = GPU::Backend::Metal,
	});
	auto application = GUI::Application::create(*device);
	auto window = GUI::Window::create({
		.title = u8"Sandbox",
		.size = { 1280, 720 },
	});
	window->show();

	const auto command_list = device->record([](auto& c) {
		// f
		c.render_pass({}, [](auto& rg) {
			// c
			rg.clear_color(1);
		});
	});

	application->run();
	return 0;
}
