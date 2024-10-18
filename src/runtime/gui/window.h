/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <core/containers/string_view.h>
#include <core/math/vector2.h>

namespace grizzly::gui {
	class Window {
	public:
		struct SpawnInfo {
			StringView title;
			Vector2<u32> size;

			bool resizable	 : 1 = true;
			bool minimizable : 1 = true;
			bool maximizable : 1 = true;
		};

		/**
		 * @brief Closes the window so it may never be opened again.
		 *
		 * @return true if the window closed succesfully.
		 * @return false if the window was already closed.
		 */
		virtual bool close() = 0;

		enum class Visibility : u8 {
			Visible,
			Maximized,
			Minimized,
			Invisible,
		};
		virtual bool show(Visibility visibility) = 0;

		virtual ~Window() {}

	protected:
	};
} // namespace grizzly::gui
