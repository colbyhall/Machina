/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Containers/Array.hpp>
#include <Core/Format.hpp>
#include <GUI/Window.hpp>

namespace Mach::GUI {
	class Id {
	public:
		explicit Id() : m_value(0) {}
		explicit Id(u64 value) : m_value(value) {}
		explicit Id(u32 item, u32 index);

		static Id from_string(StringView s);

		bool is_null() const { return m_value == 0; }
		bool operator==(const Id& other) const = default;
		u64 operator*() const { return m_value; }

	private:
		u64 m_value;
	};

	class State {
	public:
		struct WindowContext {
			Id id;
			u64 last_frame;
			UniquePtr<Window> window;
		};

		explicit State(const GPU::Device& device);

		GPU::Device const& device() const { return m_device; }
		GPU::GraphicsPipeline const& pipeline() const { return *m_pipeline; }

		WindowContext& get_or_create_window(Id id, StringView title, Size default_size);

		/** Increments the frame counter and then releases any unused windows. */
		void increment_frame();

	private:
		GPU::Device const& m_device;

		GPU::Handle<GPU::GraphicsPipeline> m_pipeline;

		u64 m_frame = 0;
		Id m_active;
		Id m_hover;
		Array<WindowContext> m_windows;
	};
} // namespace Mach::GUI

namespace Mach {
	template <>
	struct TypeFormatter<GUI::Id> {
		usize format(Core::Writer& writer, const GUI::Id& value) {
			TypeFormatter<u64> formatter;
			const auto bytes_written = formatter.format(writer, *value);
			return bytes_written;
		}
	};

} // namespace Mach
