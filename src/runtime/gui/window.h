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

		virtual ~Window() {}

	protected:
	};
} // namespace grizzly::gui
