/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Containers/Shared.hpp>
#include <Core/Containers/StringView.hpp>
#include <Core/Math/Vector2.hpp>

namespace Grizzly::GUI {
	class Window : public SharedFromThis<Window> {
	public:
		struct SpawnInfo {
			StringView title;
			Vector2<u32> size;

			bool resizable = true;
			bool minimizable = true;
			bool maximizable = true;
		};

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

		virtual ~Window() {}
	};
} // namespace Grizzly::GUI
