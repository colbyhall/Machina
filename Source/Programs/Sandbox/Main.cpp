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
	const auto device = GPU::Device::create({
		.backend = GPU::Backend::Metal,
	});
	auto app = GUI::Application::create(*device);

	const auto window = app.create<GUI::Window>({
		.title = u8"Sandbox",
		.size = { 1280, 720 },
	});
	window->show();

	const auto buffer = device->create_buffer({
		.usage = GPU::Buffer::Usage::TransferSrc,
		.heap = GPU::Buffer::Heap::Upload,
		.len = 1024,
		.stride = 4,
	});

	app.run([&]() {
		const auto command_list = device->record([&](auto& recorder) {
			const auto back_buffer = window->swapchain().next_back_buffer();
			recorder.copy_buffer_to_buffer(*buffer, *buffer)
				.copy_buffer_to_buffer(*buffer, *buffer)
				.render_pass({}, [&](auto& rp) { rp.clear_color({ 0.0f, 0.0f, 0.0f, 1.0f }).draw(*buffer, 0, 0); });
		});
	});
	return 0;
}
