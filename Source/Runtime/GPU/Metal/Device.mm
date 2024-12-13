/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <GPU/Metal/Device.hpp>

#include <GPU/Metal/Buffer.hpp>
#include <GPU/Metal/CommandList.hpp>
#include <GPU/Metal/Swapchain.hpp>
#include <GPU/Metal/Texture.hpp>

#import <AppKit/AppKit.h>
#import <Metal/Metal.h>

namespace Grizzly::GPU {
	Shared<Device> create_metal_device(Device::CreateInfo const& create_info) {
		@autoreleasepool {
			id<MTLDevice> device = MTLCreateSystemDefaultDevice();

			MTLCommandQueueDescriptor* descriptor = [[MTLCommandQueueDescriptor alloc] init];
			[descriptor setMaxCommandBufferCount:32];
			id<MTLCommandQueue> command_queue = [device newCommandQueueWithDescriptor:descriptor];

			return Shared<MetalDevice>::create(device, command_queue);
		}
	}

	Unique<Swapchain> MetalDevice::create_swapchain(Swapchain::Owner owner) {
		@autoreleasepool {
			CAMetalLayer* layer = [CAMetalLayer layer];
			layer.device = *m_device;
			layer.pixelFormat = MTLPixelFormatBGRA8Unorm;
			layer.framebufferOnly = YES;
			layer.contentsScale = [NSScreen mainScreen].backingScaleFactor;
			auto* window = (NSWindow*)owner;
			layer.frame = window.contentView.bounds;
			[window.contentView setLayer:layer];
			[window.contentView setWantsLayer:YES];

			return Unique<MetalSwapchain>::create(layer);
		}
	}

	Shared<Buffer> MetalDevice::create_buffer(Buffer::CreateInfo const& create_info) {
		@autoreleasepool {
			MTLResourceOptions options = MTLResourceStorageModeManaged;
			if (create_info.heap == Buffer::Heap::Storage) {
				options = MTLResourceStorageModePrivate;
			}

			id<MTLBuffer> buffer = [m_device newBufferWithLength:create_info.len * create_info.stride options:options];

			return Shared<MetalBuffer>::create(create_info, buffer);
		}
	}

	Shared<Texture> MetalDevice::create_texture(Texture::CreateInfo const& create_info) {
		@autoreleasepool {
			MTLTextureDescriptor* descriptor = [[MTLTextureDescriptor alloc] init];
			// TODO: Create a conversion function for Texture::Format
			descriptor.pixelFormat = MTLPixelFormatBGRA8Unorm;
			descriptor.width = create_info.size.x;
			descriptor.height = create_info.size.y;
			descriptor.depth = create_info.size.z;
			descriptor.usage = MTLTextureUsageShaderRead | MTLTextureUsageShaderWrite;
			descriptor.mipmapLevelCount = create_info.mip_levels;
			descriptor.storageMode = MTLStorageModePrivate;

			id<MTLTexture> texture = [m_device newTextureWithDescriptor:descriptor];

			return Shared<MetalTexture>::create(create_info, texture);
		}
	}

	// Shared<GraphicsPipeline> MetalDevice::create_graphics_pipeline(GraphicsPipeline::CreateInfo const& create_info)
	// {}

	Shared<CommandList> MetalDevice::record(FunctionRef<void(CommandRecorder&)> f) {
		@autoreleasepool {
			// Create a command buffer
			id<MTLCommandBuffer> command_buffer = [m_command_queue commandBuffer];

			MetalCommandRecorder recorder(command_buffer);
			f(recorder);
			[command_buffer retain];

			return Shared<MetalCommandList>::create(command_buffer);
		}
	}

} // namespace Grizzly::GPU
