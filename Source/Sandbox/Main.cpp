/**
 * Copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include "GPU/Buffer.hpp"
#include <Core/Async/Mutex.hpp>
#include <Core/Async/Scheduler.hpp>
#include <Core/Debug/Log.hpp>
#include <Core/Math/Matrix4.hpp>
#include <Core/Math/Vector3.hpp>
#include <Core/Memory.hpp>
#include <GPU/Device.hpp>
#include <GUI/Application.hpp>
#include <GUI/Window.hpp>

namespace Forge {
	int main() {
		Core::Scheduler scheduler{};
		scheduler.init({
			.thread_count = 10,
			.fiber_count = 512,
			.waiting_count = 1024,
		});

#if FORGE_OS == FORGE_OS_MACOS
		const auto backend = GPU::Backend::Metal;
#elif FORGE_OS == FORGE_OS_WINDOWS
		const auto backend = GPU::Backend::Vulkan;
#endif

		const auto device = GPU::Device::create({
			.backend = backend,
		});
		auto app = GUI::Application(scheduler, *device);

#if 0
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
		)"_sv;
		const auto library = device->create_library_from_source({
			.language = GPU::ShaderLanguage::MSL,
			.text = shader_source,
		});
		const auto vertex_shader = library->create_vertex_shader(u8"vertex_main"_sv);
		const auto fragment_shader = library->create_fragment_shader(u8"fragment_main"_sv);

		const auto graphics_pipeline = device->create_graphics_pipeline({
			.vertex_shader = *vertex_shader,
			.fragment_shader = *fragment_shader,
			.color_attachments = {
				{ .format = GPU::Format::BGRA_U8_SRGB },
			},
		});
#endif

		f64 time = 0;
		return app.run([&](auto& frame) {
			time += frame.delta_time();

			frame.window(u8"Hello World"_sv, [&](auto& builder) {
				if (builder.button(u8"Hello World"_sv)) {
					dbgln(u8"Hello World"_sv);
				}
			});
		});
	}
} // namespace Forge
