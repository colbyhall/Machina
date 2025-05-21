/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Containers/Array.hpp>
#include <GUI/Window.hpp>

namespace Forge::GUI {
	class Id {
	public:
		explicit Id() : m_value(0) {}
		explicit Id(u64 value) : m_value(value) {}
		explicit Id(u32 item, u32 index);

		static Id from_string(StringView s);

		FORGE_ALWAYS_INLINE bool is_null() const { return m_value == 0; }
		FORGE_ALWAYS_INLINE bool operator==(const Id& other) { return m_value == other.m_value; };
		FORGE_ALWAYS_INLINE bool operator!=(const Id& other) { return m_value != other.m_value; };

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

		FORGE_ALWAYS_INLINE GPU::Device const& device() const { return m_device; }

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
} // namespace Forge::GUI
