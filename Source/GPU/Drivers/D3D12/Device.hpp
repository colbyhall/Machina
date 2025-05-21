/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <GPU/Device.hpp>

#include <GPU/Drivers/D3D12/D3D12.hpp>

namespace Forge::GPU {
	class D3D12Device final : public Device {
	public:
		using CreateDevice = Core::RemovePointer<PFN_D3D12_CREATE_DEVICE>;

		static UniquePtr<D3D12Device> create();

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
	};
} // namespace Forge::GPU
