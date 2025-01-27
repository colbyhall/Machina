/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include "Core/Debug/Assertions.hpp"
#include <GPU/Device.hpp>

namespace Grizzly::GPU {
	// Ideally the api users would create a GPU::Device by using `Arc<GPU::BackendDevice>::create(info)`. Metals headers
	// currently contain Objective-C code that if leaked to a non Objective-C++ translation unit would cause a
	// compilation fail. The goal is to not need Objective-C code outside of platform abstractions.
	Arc<Device> create_metal_device(Device::CreateInfo const& create_info);

	Arc<Device> Device::create(const CreateInfo& create_info) {
		Option<Arc<Device>> result = nullopt;
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
