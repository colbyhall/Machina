/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Containers/StringView.hpp>
#include <Core/Math/Vector2.hpp>
#include <GPU/Forward.hpp>
#include <GUI/View.hpp>

namespace Forge::GUI {
	class Application;

	class Window : public View {
	public:
		struct CreateInfo {
			StringView title;
			Vector2<u32> size;

			bool resizable = true;
			bool minimizable = true;
			bool maximizable = true;
		};
		static Rc<Window> create(Application const& app, CreateInfo const& create_info);

		/**
		 * @brief Closes the window so it may never be opened again.
		 *
		 * @return true if the window closed succesfully.
		 * @return false if the window was already closed.
		 */
		virtual bool close() = 0;
		virtual bool show() = 0;
		virtual bool hide() = 0;
		virtual bool maximize() = 0;
		virtual bool minimize() = 0;
		virtual GPU::Swapchain& swapchain() = 0;
		virtual Vector2<f32> cursor_position() const = 0;
	};
} // namespace Forge::GUI
