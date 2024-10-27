/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include "Core/Debug/Assertions.hpp"
#include <GPU/Device.hpp>

namespace Grizzly::GPU {
	// GPU/Metal/Device.hpp contains Objective-C. We can't use a static method to create the device
	// so forwrds declare the function here.
	Shared<Device> create_metal_device(Device::CreateInfo const& create_info);

	Shared<Device> Device::create(const CreateInfo& create_info) {
		Option<Shared<Device>> result = nullopt;
		switch (create_info.backend) {
		case Backend::Metal:
			result = create_metal_device(create_info);
			break;
		case Backend::DirectX:
			// DirectXDevice::create();
			break;
		case Backend::Vulkan:
			// VulkanDevice::create();
			break;
		default:
			GRIZZLY_UNREACHABLE;
			break;
		}
		return result.unwrap();
	}
} // namespace Grizzly::GPU
