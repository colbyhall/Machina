/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#import <Metal/Metal.h>

#include <Core/ObjectiveC/Protocol.hpp>
#include <GPU/Device.hpp>

namespace Grizzly::GPU {
	class MetalDevice final : public Device {
	public:
		explicit MetalDevice(id<MTLDevice> device, id<MTLCommandQueue> command_queue)
			: m_device(device)
			, m_command_queue(command_queue) {}

		// Device Interace
		Unique<Swapchain> create_swapchain(Swapchain::Owner owner) final;
		Rc<Buffer> create_buffer(Buffer::CreateInfo const& info) final;
		Rc<Texture> create_texture(Texture::CreateInfo const& info) final;
		Rc<GraphicsPipeline> create_graphics_pipeline(GraphicsPipeline::CreateInfo const& info) final;
		Rc<Library> create_library_from_source(StringView info) final;
		Rc<CommandList> record(FunctionRef<void(CommandRecorder&)> f) final;
		Backend backend() const final { return Backend::Metal; }
		// ~Device Interface

	private:
		Core::Protocol m_device;		// MTLDevice
		Core::Protocol m_command_queue; // MTLCommandQueue
	};

	Rc<Device> create_metal_device(Device::CreateInfo const& create_info);
} // namespace Grizzly::GPU
