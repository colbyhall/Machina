/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <GPU/Drivers/D3D12/Device.hpp>

#include <Core/FileSystem/Library.hpp>

namespace Mach::GPU {
	UniquePtr<D3D12Device> D3D12Device::create() {
		auto result = UniquePtr<D3D12Device>::create();

		auto d3d12 = Core::Library::load(u8"d3d12.dll"_sv);
		MACH_ASSERT(d3d12.is_valid());

		const auto create_device = d3d12->find_symbol<CreateDevice>(u8"D3D12CreateDevice"_sv);
		MACH_ASSERT(create_device.is_valid());

		return result;
	}

	UniquePtr<Swapchain> D3D12Device::create_swapchain(Swapchain::Owner owner) const {
		MACH_UNUSED(owner);
		MACH_UNIMPLEMENTED;
		return UniquePtr<Swapchain>();
	}

	Handle<Buffer> D3D12Device::create_upload_buffer(Buffer::Usage usage, Slice<u8 const> bytes) const {
		MACH_UNUSED(usage);
		MACH_UNUSED(bytes);
		MACH_UNIMPLEMENTED;
		return Handle<Buffer>();
	}

	Handle<Buffer> D3D12Device::create_storage_buffer(Buffer::Usage usage, usize size) const {
		MACH_UNUSED(usage);
		MACH_UNUSED(size);
		MACH_UNIMPLEMENTED;
		return Handle<Buffer>();
	}

	Handle<Texture> D3D12Device::create_texture(Texture::CreateInfo const& info) const {
		MACH_UNUSED(info);
		MACH_UNIMPLEMENTED;
		return Handle<Texture>();
	}

	Handle<GraphicsPipeline> D3D12Device::create_graphics_pipeline(GraphicsPipeline::CreateInfo const& info) const {
		MACH_UNUSED(info);
		MACH_UNIMPLEMENTED;
		return Handle<GraphicsPipeline>();
	}

	Handle<Library> D3D12Device::create_library_from_source(ShaderSource const& info) const {
		MACH_UNUSED(info);
		MACH_UNIMPLEMENTED;
		return Handle<Library>();
	}

	Handle<CommandList> D3D12Device::record(FunctionRef<void(CommandRecorder&)> f) const {
		MACH_UNUSED(f);
		MACH_UNIMPLEMENTED;
		return Handle<CommandList>();
	}
} // namespace Mach::GPU
