/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * this software is released under the mit license.
 */

#pragma once

#include <Core/Containers/UniquePtr.hpp>

#include <GPU/Buffer.hpp>
#include <GPU/CommandList.hpp>
#include <GPU/Forward.hpp>
#include <GPU/GraphicsPipeline.hpp>
#include <GPU/Shader.hpp>
#include <GPU/Swapchain.hpp>
#include <GPU/Texture.hpp>

namespace Forge::GPU {
	class Device {
	public:
		FORGE_NO_DISCARD virtual UniquePtr<Swapchain> create_swapchain(Swapchain::Owner owner) const = 0;

		FORGE_NO_DISCARD virtual Handle<Buffer>
		create_upload_buffer(Buffer::Usage usage, Slice<u8 const> bytes) const = 0;

		FORGE_NO_DISCARD virtual Handle<Buffer> create_storage_buffer(Buffer::Usage usage, usize size) const = 0;

		FORGE_NO_DISCARD virtual Handle<Texture> create_texture(Texture::CreateInfo const& info) const = 0;

		FORGE_NO_DISCARD virtual Handle<Library> create_library_from_source(ShaderSource const& source) const = 0;

		FORGE_NO_DISCARD virtual Handle<GraphicsPipeline>
		create_graphics_pipeline(GraphicsPipeline::CreateInfo const& info) const = 0;

		FORGE_NO_DISCARD virtual Handle<CommandList> record(FunctionRef<void(CommandRecorder&)> f) const = 0;

		virtual ~Device() {}
	};
} // namespace Forge::GPU
