/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <GPU/Metal/Device.hpp>

#include <GPU/Metal/Buffer.hpp>
#include <GPU/Metal/CommandList.hpp>
#include <GPU/Metal/Conversion.hpp>
#include <GPU/Metal/GraphicsPipeline.hpp>
#include <GPU/Metal/Shader.hpp>
#include <GPU/Metal/Swapchain.hpp>
#include <GPU/Metal/Texture.hpp>

#import <AppKit/AppKit.h>
#import <Metal/Metal.h>

namespace Forge::GPU {
	UniquePtr<Device> create_metal_device(Device::CreateInfo const& create_info) {
		@autoreleasepool {
			id<MTLDevice> device = MTLCreateSystemDefaultDevice();

			MTLCommandQueueDescriptor* descriptor = [[MTLCommandQueueDescriptor alloc] init];
			[descriptor setMaxCommandBufferCount:32];
			id<MTLCommandQueue> command_queue = [device newCommandQueueWithDescriptor:descriptor];

			return UniquePtr<MetalDevice>::create(device, command_queue);
		}
	}

	UniquePtr<Swapchain> MetalDevice::create_swapchain(Swapchain::Owner owner) const {
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

			return UniquePtr<MetalSwapchain>::create(layer);
		}
	}

	Handle<Buffer> MetalDevice::create_upload_buffer(Buffer::Usage usage, Slice<u8 const> bytes) const {
		@autoreleasepool {
			id<MTLBuffer> buffer = [m_device newBufferWithBytes:bytes.begin()
														 length:(NSUInteger)bytes.len()
														options:MTLResourceStorageModeShared];

			return Handle<MetalBuffer>::create(usage, Heap::Upload, bytes.len(), buffer);
		}
	}

	Handle<Buffer> MetalDevice::create_storage_buffer(Buffer::Usage usage, usize size) const {
		@autoreleasepool {
			id<MTLBuffer> buffer = [m_device newBufferWithLength:size options:MTLResourceStorageModePrivate];

			return Handle<MetalBuffer>::create(usage, Heap::Storage, size, buffer);
		}
	}

	Handle<Texture> MetalDevice::create_texture(Texture::CreateInfo const& create_info) const {
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

			return Handle<MetalTexture>::create(create_info, texture);
		}
	}

	Handle<Library> MetalDevice::create_library_from_source(ShaderSource const& source) const {
		FORGE_ASSERT(source.language == ShaderLanguage::MSL);
		@autoreleasepool {
			NSString* objc_source = [[NSString alloc] initWithBytesNoCopy:(void*)*source.text
																   length:static_cast<NSUInteger>(source.text.len())
																 encoding:NSUTF8StringEncoding
															 freeWhenDone:NO];

			MTLCompileOptions* options = [[MTLCompileOptions alloc] init];
			NSError* error = nil;
			id<MTLLibrary> library = [m_device newLibraryWithSource:objc_source options:options error:&error];
			if (error != nil) {
				NSString* message = [error localizedDescription];
				NSLog(@"Error: %@", message);
				Forge::Core::abort();
			}
			return Handle<MetalLibrary>::create(library);
		}
	}

	Handle<GraphicsPipeline>
	MetalDevice::create_graphics_pipeline(GraphicsPipeline::CreateInfo const& create_info) const {
		@autoreleasepool {
			MTLRenderPipelineDescriptor* const pipeline_descriptor = [[MTLRenderPipelineDescriptor alloc] init];
			pipeline_descriptor.vertexFunction =
				static_cast<MetalVertexShader const&>(create_info.vertex_shader).function();
			pipeline_descriptor.fragmentFunction =
				static_cast<MetalFragmentShader const&>(create_info.fragment_shader).function();

			// Fill out the color attachments from create_info
			for (usize index = 0; index < create_info.color_attachments.len(); index += 1) {
				auto& color_attachment = create_info.color_attachments[index];

				pipeline_descriptor.colorAttachments[index].pixelFormat =
					GPU::format_to_mtl_pixel_format(color_attachment.format);

				pipeline_descriptor.colorAttachments[index].blendingEnabled = color_attachment.blend_enabled;

				pipeline_descriptor.colorAttachments[index].sourceRGBBlendFactor =
					blend_factor_to_mtl_blend_factor(color_attachment.src_color_blend_factor);
				pipeline_descriptor.colorAttachments[index].destinationRGBBlendFactor =
					blend_factor_to_mtl_blend_factor(color_attachment.dst_color_blend_factor);
				pipeline_descriptor.colorAttachments[index].rgbBlendOperation =
					blend_op_to_mtl_blend_operation(color_attachment.color_blend_op);

				pipeline_descriptor.colorAttachments[index].sourceAlphaBlendFactor =
					blend_factor_to_mtl_blend_factor(color_attachment.src_alpha_blend_factor);
				pipeline_descriptor.colorAttachments[index].destinationAlphaBlendFactor =
					blend_factor_to_mtl_blend_factor(color_attachment.dst_alpha_blend_factor);
				pipeline_descriptor.colorAttachments[index].alphaBlendOperation =
					blend_op_to_mtl_blend_operation(color_attachment.alpha_blend_op);

				MTLColorWriteMask write_mask = MTLColorWriteMaskNone;
				if ((color_attachment.color_write_mask & GraphicsPipeline::ColorComponents::R) ==
					GraphicsPipeline::ColorComponents::R) {
					write_mask |= MTLColorWriteMaskRed;
				}
				if ((color_attachment.color_write_mask & GraphicsPipeline::ColorComponents::G) ==
					GraphicsPipeline::ColorComponents::G) {
					write_mask |= MTLColorWriteMaskGreen;
				}
				if ((color_attachment.color_write_mask & GraphicsPipeline::ColorComponents::B) ==
					GraphicsPipeline::ColorComponents::B) {
					write_mask |= MTLColorWriteMaskBlue;
				}
				if ((color_attachment.color_write_mask & GraphicsPipeline::ColorComponents::A) ==
					GraphicsPipeline::ColorComponents::A) {
					write_mask |= MTLColorWriteMaskAlpha;
				}
				pipeline_descriptor.colorAttachments[index].writeMask = write_mask;
			}

			// TODO: Depth and Stencil Attachments

			pipeline_descriptor.inputPrimitiveTopology =
				draw_mode_to_mtl_primitive_topology_class(create_info.draw_mode);
			pipeline_descriptor.tessellationOutputWindingOrder = winding_to_mtl_winding(create_info.winding);

			NSError* error = nil;
			id<MTLRenderPipelineState> pipeline_state =
				[m_device newRenderPipelineStateWithDescriptor:pipeline_descriptor error:&error];
			FORGE_ASSERT(error == nil);
			return Handle<MetalGraphicsPipeline>::create(create_info, pipeline_state);
		}
	}

	Handle<CommandList> MetalDevice::record(FunctionRef<void(CommandRecorder&)> f) const {
		@autoreleasepool {
			// Create a command buffer
			id<MTLCommandBuffer> command_buffer = [m_command_queue commandBuffer];

			MetalCommandRecorder recorder(command_buffer);
			f(recorder);

			return Handle<MetalCommandList>::create(command_buffer);
		}
	}

} // namespace Forge::GPU
