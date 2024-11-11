/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Containers/Shared.hpp>

namespace Grizzly::GUI {
	class View : public SharedFromThis<View> {
	public:
		virtual ~View() {}
	};
} // namespace Grizzly::GUI
