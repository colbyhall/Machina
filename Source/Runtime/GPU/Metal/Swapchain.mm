/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <GPU/Metal/Swapchain.hpp>

namespace Grizzly::GPU {
	MetalSwapchain::~MetalSwapchain() {
		@autoreleasepool {
			[m_layer release];
		}
	}
} // namespace Grizzly::GPU
