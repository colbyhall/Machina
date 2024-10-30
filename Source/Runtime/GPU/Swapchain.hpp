/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Containers/Shared.hpp>

namespace Grizzly::GPU {
	class Swapchain : public SharedFromThis<Swapchain> {
	public:
		using Owner = void*;

		virtual ~Swapchain() {}
	};
} // namespace Grizzly::GPU
