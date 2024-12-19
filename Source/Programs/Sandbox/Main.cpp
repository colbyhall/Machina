/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Debug/Log.hpp>
#include <Core/Math/Vector3.hpp>
#include <Core/Memory.hpp>
#include <GPU/Device.hpp>
#include <GUI/Application.hpp>
#include <GUI/Window.hpp>

using namespace Grizzly;

int main(int argc, char** argv) {
	const auto device = GPU::Device::create({ .backend = GPU::Backend::Metal });
	auto app = GUI::Application::create(device);

	const auto window = app.create<GUI::Window>({
		.title = u8"Hello World"sv,
		.size = { 1280, 720 },
	});
	window->show();

	const StringView shader_source = u8R"(
		#include <metal_stdlib>
		using namespace metal;

		vertex float4
		vertex_main(uint vertexID [[vertex_id]],
					 constant simd::float3* vertexPositions)
		{
			float4 vertexOutPositions = float4(vertexPositions[vertexID][0],
											   vertexPositions[vertexID][1],
											   vertexPositions[vertexID][2],
											   1.0f);
			return vertexOutPositions;
		}

		fragment float4 fragment_main(float4 vertexOutPositions [[stage_in]]) {
			return float4(182.0f/255.0f, 240.0f/255.0f, 228.0f/255.0f, 1.0f);
		}
	)"sv;
	const auto library = device->create_library_from_source(shader_source);
	const auto vertex_shader = library->create_vertex_shader(u8"vertex_main"sv);
	const auto pixel_shader = library->create_pixel_shader(u8"fragment_main"sv);

	const auto graphics_pipeline = device->create_graphics_pipeline({
		.vertex_shader = vertex_shader,
		.pixel_shader = pixel_shader,
		.color_attachments = { GPU::Texture::Format::BGR_U8_SRGB },
	});

	f64 time = 0;
	return app.run([&](f64 delta_time) {
		time += delta_time;

		const auto backbuffer = window->swapchain().next_back_buffer();
		const auto command_list = device->record([&](auto& cr) {
			const auto vertices = device->create_buffer({
				.usage = GPU::Buffer::Usage::Vertex,
				.heap = GPU::Buffer::Heap::Upload,
				.len = 3,
				.stride = sizeof(Vector3<f32>),
			});
			const f32 x = Math::cos(time * 5.f) * 0.5f;
			const f32 y = Math::sin(time * 5.f) * 0.5f;
			const auto offset = Vector3<f32>{ x, y, 0.f };
			vertices->map([&](auto slice) {
				const Slice<Vector3<f32> const> vertex_slice = {
					Vector3<f32>{ -0.5f, -0.5f, 0.f } + offset,
					Vector3<f32>{ 0.f, 0.5f, 0.f } + offset,
					Vector3<f32>{ 0.5f, -0.5f, 0.f } + offset,
				};
				Memory::copy(slice.begin(), vertex_slice.begin(), 3 * sizeof(Vector3<f32>));
			});

			const auto triangle_pass = GPU::RenderPass{
					.color_attachments = {
						GPU::ColorAttachment{
							.texture = backbuffer->texture(),
							.load_action = GPU::LoadAction::Clear,
							.store_action = GPU::StoreAction::Store,
							.clear_color = { 0.2f, 0.2f, 0.2f, 1.f},
						},
					},
				};
			cr.render_pass(triangle_pass, [&](auto& rpr) {
				rpr.set_pipeline(graphics_pipeline);
				rpr.set_vertices(vertices);
				rpr.draw(vertices->len(), 0);
			});
		});
		const auto receipt = command_list->submit();
		backbuffer->present(receipt);
	});
}
