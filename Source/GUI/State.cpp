/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <GUI/State.hpp>

#include <Core/Hash.hpp>
#include <GPU/GraphicsPipeline.hpp>

namespace Forge::GUI {
	Id::Id(u32 item, u32 index) : m_value((static_cast<u64>(item) << 32) | static_cast<u64>(index)) {}

	Id Id::from_string(StringView s) {
		Core::FNV1Hasher hasher;
		Forge::hash(hasher, s);
		return Id(hasher.finish());
	}

	State::State(const GPU::Device& device) : m_device(device), m_pipeline(), m_active(), m_hover() {}

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
			.window = Forge::move(window),
		});
		return m_windows[index];
	}
} // namespace Forge::GUI
