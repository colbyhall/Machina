/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Containers/Shared.hpp>

namespace Forge::GUI {
	class View : public RcFromThis<View> {
	public:
		virtual ~View() {}
	};
} // namespace Forge::GUI
