/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Async/Thread.hpp>
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

	const auto thread = Thread::spawn({ .f = []() { dbgln(u8"Hello World"); } });
	thread->join();

	const auto buffer = device->create_buffer({
		.usage = GPU::Buffer::Usage::TransferSrc,
		.heap = GPU::Buffer::Heap::Upload,
		.len = 1024,
		.stride = 4,
	});

	app.run([&]() {});
	return 0;
}
