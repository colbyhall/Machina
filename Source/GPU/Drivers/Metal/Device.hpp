/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <GPU/Device.hpp>

#if MACH_LANGUAGE == MACH_LANGUAGE_OBJCPP
	#include <Core/ObjectiveC/Protocol.hpp>
	#import <Metal/Metal.h>
#endif

namespace Mach::GPU {
#if MACH_LANGUAGE == MACH_LANGUAGE_OBJCPP
	class MetalDevice final : public Device {
	public:
		explicit MetalDevice(id<MTLDevice> device, id<MTLCommandQueue> command_queue)
			: m_device(device)
			, m_command_queue(command_queue) {}

		// Device Interace
		UniquePtr<Swapchain> create_swapchain(Swapchain::Owner owner) const final;
		Handle<Buffer> create_upload_buffer(Buffer::Usage usage, Slice<u8 const> bytes) const final;
		Handle<Buffer> create_storage_buffer(Buffer::Usage usage, usize size) const final;
		Handle<Texture> create_texture(Texture::CreateInfo const& info) const final;
		Handle<GraphicsPipeline> create_graphics_pipeline(GraphicsPipeline::CreateInfo const& info) const final;
		Handle<Library> create_library_from_source(ShaderSource const& info) const final;
		Handle<CommandList> record(FunctionRef<void(CommandRecorder&)> f) const final;
		// ~Device Interface

	private:
		Core::Protocol m_device;		// MTLDevice
		Core::Protocol m_command_queue; // MTLCommandQueue
	};
#endif

	UniquePtr<Device> create_metal_device();
} // namespace Mach::GPU
