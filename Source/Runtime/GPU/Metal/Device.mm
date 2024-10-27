/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <GPU/Metal/Device.hpp>

namespace Grizzly::GPU {
	Shared<Device> create_metal_device(Device::CreateInfo const& create_info) {
		@autoreleasepool {
			id<MTLDevice> device = MTLCreateSystemDefaultDevice();
			[device retain];
			return Shared<MetalDevice>::create(device);
		}
	}

	MetalDevice::~MetalDevice() {
		@autoreleasepool {
			if (m_device) {
				[m_device release];
				m_device = nil;
			}
		}
	}
} // namespace Grizzly::GPU
