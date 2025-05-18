/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <GPU/Device.hpp>

namespace Forge::GPU {
	// Ideally the api users would create a GPU::Device by using `Arc<GPU::BackendDevice>::create(info)`. Metals headers
	// currently contain Objective-C code that if leaked to a non Objective-C++ translation unit would cause a
	// compilation fail. The goal is to not need Objective-C code outside of platform abstractions.
	UniquePtr<Device> create_metal_device(Device::CreateInfo const& create_info);

	UniquePtr<Device> Device::create(const CreateInfo& create_info) {
		UniquePtr<Device> result(nullptr);
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
			FORGE_UNREACHABLE;
			break;
		}
		return result;
	}
} // namespace Forge::GPU
