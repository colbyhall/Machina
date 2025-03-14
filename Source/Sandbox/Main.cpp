/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Async/Mutex.hpp>
#include <Core/Async/Scheduler.hpp>
#include <Core/Debug/Log.hpp>
#include <Core/Math/Matrix4.hpp>
#include <Core/Math/Vector3.hpp>
#include <Core/Memory.hpp>
#include <GPU/Device.hpp>
#include <GUI/Application.hpp>
#include <GUI/Window.hpp>

using namespace Forge;

int main(int argc, char** argv) {
	auto scheduler = Core::Scheduler::create({
		.thread_count = 10,
		.fiber_count = 512,
		.waiting_count = 1024,
	});
	auto device = GPU::Device::create({
		.backend = GPU::Backend::Metal,
	});
	auto app = Arc<GUI::Application>::create(*scheduler, *device);

	scheduler->enqueue([app, &scheduler = *scheduler]() {
		dbgln(u8"Hello World1"sv);
		auto future = scheduler.enqueue<u32>(Core::Scheduler::Priority::Normal, []() {
			dbgln(u8"Hello World2"sv);
			return 0;
		});
		scheduler.wait_for(*future);
	});

	const auto window = app->create<GUI::Window>({
		.title = u8"Hello World"sv,
		.size = { 1280, 720 },
	});
	window->show();

	const StringView shader_source = u8R"(
		#include <metal_stdlib>
		using namespace metal;

		struct FragmentIn {
			float4 position [[position]];
			float3 color;
		};

		struct Uniforms {
			float4x4 view;
		};

		vertex FragmentIn
		vertex_main(uint vertexID [[vertex_id]],
					 constant simd::float3* vertexPositions,
					 constant Uniforms& uniforms [[buffer(1)]])
		{
			float4 vertexOutPositions = float4(vertexPositions[vertexID][0],
											   vertexPositions[vertexID][1],
											   vertexPositions[vertexID][2],
											   1.0f);
			float3 colors[3] = {
				float3(1.0, 0.0, 0.0),
				float3(0.0, 1.0, 0.0),
				float3(0.0, 0.0, 1.0)
			};
			FragmentIn result;
			result.position = uniforms.view * vertexOutPositions;
			result.color = colors[vertexID % 3];
			return result;
		}

		fragment float4 fragment_main(FragmentIn fragment_in [[stage_in]]) {
			return float4(fragment_in.color, 1.0);
		}
	)"sv;
	const auto library = device->create_library_from_source({
		.language = GPU::ShaderLanguage::MSL,
		.text = shader_source,
	});
	const auto vertex_shader = library->create_vertex_shader(u8"vertex_main"sv);
	const auto fragment_shader = library->create_fragment_shader(u8"fragment_main"sv);

	const auto graphics_pipeline = device->create_graphics_pipeline({
		.vertex_shader = *vertex_shader,
		.fragment_shader = *fragment_shader,
		.color_attachments = {
			{ .format = GPU::Format::BGRA_U8_SRGB },
		},
	});

	f64 time = 0;
	return GUI::run(*app, [&](f64 delta_time) {
		time += delta_time;

		const auto backbuffer = window->swapchain().next_back_buffer();
		const auto command_list = device->record([&](auto& cr) {
			const auto vertices = device->create_buffer({
				.usage = GPU::Buffer::Usage::Vertex,
				.heap = GPU::Heap::Upload,
				.len = 3,
				.stride = sizeof(Vector3<f32>),
			});
			vertices->map([&](auto slice) {
				const Slice<Vector3<f32> const> vertex_slice = {
					{ 0.f, 0.f, 0.f },
					{ 0.f, 500.f, 0.f },
					{ 500.f, 500.f, 0.f },
				};
				Memory::copy(slice.begin(), vertex_slice.begin(), slice.len());
			});

			struct Uniforms {
				Matrix4<f32> view;
			};
			const auto uniforms = device->create_buffer({
				.usage = GPU::Buffer::Usage::Constant,
				.heap = GPU::Heap::Upload,
				.len = 1,
				.stride = sizeof(Uniforms),
			});
			uniforms->map([&](auto slice) {
				const auto viewport = backbuffer->texture().size().xy().as<f32>();
				const auto projection = Matrix4<f32>::orthographic(viewport.x, viewport.y, 0.01f, 5.f);
				const auto view = Matrix4<f32>::translate({ -viewport.x / 2.f, -viewport.y / 2.f, 0.f });
				const auto uniform = Uniforms{
					.view = projection * view,
				};
				Memory::copy(slice.begin(), &uniform, slice.len());
			});

			const auto triangle_pass = GPU::RenderPass{
				.color_attachments = {
					{
						.texture = backbuffer->texture(),
						.load_action = GPU::LoadAction::Clear,
						.store_action = GPU::StoreAction::Store,
						.clear_color = { 0.2f, 0.2f, 0.2f, 1.f},
					},
				},
			};
			cr.render_pass(triangle_pass, [&](auto& rpr) {
				rpr.set_pipeline(*graphics_pipeline);
				rpr.set_vertices(*vertices);
				rpr.set_constant(0, *uniforms);
				rpr.draw(vertices->len(), 0);
			});
		});
		const auto receipt = command_list->submit();
		backbuffer->present(*receipt);
	});
}
