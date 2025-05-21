/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <GUI/Application.hpp>

#include <Core/Time.hpp>
#include <GPU/Device.hpp>
#include <GPU/Swapchain.hpp>

namespace Forge::GUI {
	bool Frame::window(StringView title, FunctionRef<void(Builder&)> f) {
		const auto default_size = Size(1280, 720);
		const auto id = Id::from_string(title);

		auto& context = m_state.get_or_create_window(id, title, default_size);

		// Create a new builder for the window.
		Builder builder(m_state, Layout(Layout::Direction::UpToDown, context.window->viewport()));

		// Call the function to build the window.
		f(builder);

#if 0
		const auto backbuffer = context.window->swapchain().next_back_buffer();
		const auto command_list = m_state.device().record([&](auto& cr) {
			const auto cursor_position = window->cursor_position();

			const auto cursor_size = 50.f;
			const auto half_size = cursor_size / 2.f;

			const auto bl = cursor_position - half_size;
			const auto tr = cursor_position + half_size;
			const auto tl = bl + Vector2<f32>{ 0.f, cursor_size };
			const auto br = bl + Vector2<f32>{ cursor_size, 0.f };

			const Slice<Vector3<f32> const> vertex_slice = {
				{ bl, 0.f }, { tl, 0.f }, { br, 0.f }, { br, 0.f }, { tl, 0.f }, { tr, 0.f },
			};
			using BufferUsage = GPU::Buffer::Usage;
			const auto vertices = device->create_upload_buffer(BufferUsage::Vertex, vertex_slice.as_bytes());

			struct Uniforms {
				Matrix4<f32> view;
			};
			const auto viewport = backbuffer->texture().size().xy().as<f32>();
			const auto projection = Matrix4<f32>::orthographic(viewport.x, viewport.y, 0.01f, 5.f);
			const auto view = Matrix4<f32>::translate({ -viewport.x / 2.f, -viewport.y / 2.f, 0.f });
			const auto uniform = Uniforms{
				.view = projection * view,
			};
			const auto uniforms =
				device->create_upload_buffer(BufferUsage::Constant, Slice<Uniforms const>(uniform).as_bytes());

			const auto triangle_pass = GPU::RenderPass{
					.color_attachments = {
						{
							.texture = backbuffer->texture(),
							.load_action = GPU::ColorLoadAction::Clear{ .r = 0.2f, .g = 0.2f, .b = 0.2f, .a = 1.f,},
							.store_action = GPU::StoreAction::Store,
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
#endif

		// TODO: Figure out the best thing to return from this function. Either a bool for if showing or an enum for
		// show/close events
		return true;
	}

	int Application::run(FunctionRef<void(Frame&)> tick) {
		static Core::Atomic<bool> running{ false };
		FORGE_ASSERT(running.exchange(true) == false);

		auto last = Core::Instant::now();
		u64 frame_count = 0;
		while (running.load()) {
			const auto now = Core::Instant::now();
			const auto delta_time = now.since(last).as_secs_f64();
			last = now;

			pump_events();
			auto frame = Frame(frame_count, delta_time, m_state);
			tick(frame);

			frame_count += 1;
		}

		return 0;
	}
} // namespace Forge::GUI
