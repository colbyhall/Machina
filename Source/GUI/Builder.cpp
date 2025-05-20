/**
 * Copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <GUI/Builder.hpp>

namespace Forge::GUI {
	bool Builder::button(StringView text) {
		FORGE_UNUSED(text);
		return false;
	}
} // namespace Forge::GUI
