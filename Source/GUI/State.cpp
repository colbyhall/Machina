/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <GUI/State.hpp>

#include <Core/Hash.hpp>
#include <GPU/Device.hpp>
#include <GPU/GraphicsPipeline.hpp>

namespace Mach::GUI {
	Id::Id(u32 item, u32 index) : m_value((static_cast<u64>(item) << 32) | static_cast<u64>(index)) {}

	Id Id::from_string(StringView s) {
		Core::FNV1Hasher hasher;
		Mach::hash(hasher, s);
		return Id(hasher.finish());
	}

	State::State(const GPU::Device& device) : m_device(device), m_pipeline(), m_active(), m_hover() {
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
		const auto library = device.create_library_from_source({
			.language = GPU::ShaderLanguage::MSL,
			.text = shader_source,
		});
		const auto vertex_shader = library->create_vertex_shader(u8"vertex_main"_sv);
		const auto fragment_shader = library->create_fragment_shader(u8"fragment_main"_sv);

		m_pipeline = device.create_graphics_pipeline({
			.vertex_shader = *vertex_shader,
			.fragment_shader = *fragment_shader,
			.color_attachments = {
				{ .format = GPU::Format::BGRA_U8_SRGB },
			},
		});
	}

	void State::increment_frame() {
		// TODO: Cleanup unused windows
		m_frame += 1;
	}

	State::WindowContext& State::get_or_create_window(Id id, StringView title, Size default_size) {
		for (auto& context : m_windows) {
			if (context.id == id) {
				context.last_frame = m_frame;
				return context;
			}
		}

		auto window = Window::create({ .title = title, .size = default_size, .device = m_device });
		window->show();
		const auto index = m_windows.push(WindowContext{
			.id = id,
			.last_frame = m_frame,
			.window = Mach::move(window),
		});
		return m_windows[index];
	}
} // namespace Mach::GUI
