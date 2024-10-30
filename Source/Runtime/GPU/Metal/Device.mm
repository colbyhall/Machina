/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <GPU/Metal/Device.hpp>
#include <GPU/Metal/Swapchain.hpp>

#import <AppKit/AppKit.h>

namespace Grizzly::GPU {
	Shared<Device> create_metal_device(Device::CreateInfo const& create_info) {
		@autoreleasepool {
			id<MTLDevice> device = MTLCreateSystemDefaultDevice();

			// Result will take ownership of device
			[device retain];
			return Shared<MetalDevice>::create(device);
		}
	}

	Unique<Swapchain> MetalDevice::create_swapchain(Swapchain::Owner owner) {
		@autoreleasepool {
			CAMetalLayer* layer = [CAMetalLayer layer];
			layer.device = m_device;
			layer.pixelFormat = MTLPixelFormatBGRA8Unorm;
			layer.framebufferOnly = YES;
			layer.contentsScale = [NSScreen mainScreen].backingScaleFactor;
			auto* window = (NSWindow*)owner;
			layer.frame = window.contentView.bounds;
			[window.contentView setLayer:layer];
			[window.contentView setWantsLayer:YES];

			// Result will take ownership of layer
			[layer retain];
			return Unique<MetalSwapchain>::create(layer);
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
