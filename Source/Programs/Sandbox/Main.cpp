/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Debug/Log.hpp>
#include <GPU/Device.hpp>
#include <GUI/Application.hpp>
#include <GUI/Window.hpp>

using namespace Grizzly;
using namespace Core;

int main(int argc, char** argv) {
	const auto device = GPU::Device::create({ .backend = GPU::Backend::Metal });
	auto app = GUI::Application::create(*device);
	const auto window = app.create<GUI::Window>({
		.title = u8"Hello World",
		.size = { 1280, 720 },
	});
	window->show();
	return app.run([&]() {
		const auto backbuffer = window->swapchain().next_back_buffer();
		const auto command_list = device->record([&](auto& cr) {
			const auto triangle_pass = GPU::RenderPass{
					.color_attachments = {
						GPU::ColorAttachment{
							.texture = backbuffer->texture(),
							.load_action = GPU::LoadAction::Clear,
							.store_action = GPU::StoreAction::Store,
							.clear_color = { 1.f, 0.f, 1.f, 1.f},
						},
					},
				};
			cr.render_pass(triangle_pass, [&](auto& rpr) {});
		});
		const auto receipt = command_list->submit();
		backbuffer->present(*receipt);
	});
}
