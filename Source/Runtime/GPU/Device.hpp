/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * this software is released under the mit license.
 */

#pragma once

#include <Core/Containers/StringView.hpp>
#include <Core/Containers/Unique.hpp>

#include <GPU/Buffer.hpp>
#include <GPU/CommandList.hpp>
#include <GPU/GraphicsPipeline.hpp>
#include <GPU/Shader.hpp>
#include <GPU/Swapchain.hpp>
#include <GPU/Texture.hpp>

namespace Forge::GPU {
	/**
	 * Defines the supported graphic api backends.
	 */
	enum class Backend : u8 {
		Metal,
		DirectX,
		Vulkan,
	};

	template <typename T>
	using Handle = Arc<T>;

	/**
	 * An abstraction for interacting with a graphics API.
	 *
	 * Use this class to utilize the graphics cards for rendering or computation.
	 */
	class Device : public ArcFromThis<Device> {
	public:
		struct CreateInfo {
			Backend backend;
		};

		/**
		 * @brief Create a device with the given create info.
		 *
		 * @param create_info The create info for the device.
		 * @return Arc<Device> The created device.
		 */
		FORGE_NO_DISCARD static Arc<Device> create(CreateInfo const& create_info);

		/**
		 * @brief Create a swapchain for the given owner.
		 *
		 * @param owner The owner of the swapchain.
		 * @return Unique<Swapchain> The created swapchain.
		 */
		FORGE_NO_DISCARD virtual Unique<Swapchain> create_swapchain(Swapchain::Owner owner) const = 0;

		/**
		 * @brief Creates a buffer.
		 *
		 * @param info The data structure containg the immutable details of the buffer.
		 * @return Handle<Buffer> The created buffer.
		 */
		FORGE_NO_DISCARD virtual Handle<Buffer> create_buffer(Buffer::CreateInfo const& info) const = 0;

		/**
		 * @brief Creates a texture.
		 *
		 * @param info The data structure containg the immutable details of the texture.
		 * @return Handle<Texture> The created texture.
		 */
		FORGE_NO_DISCARD virtual Handle<Texture> create_texture(Texture::CreateInfo const& info) const = 0;

		FORGE_NO_DISCARD virtual Handle<Library> create_library_from_source(ShaderSource const& source) const = 0;

		FORGE_NO_DISCARD virtual Handle<GraphicsPipeline>
		create_graphics_pipeline(GraphicsPipeline::CreateInfo const& info) const = 0;

		FORGE_NO_DISCARD virtual Handle<CommandList> record(FunctionRef<void(CommandRecorder&)> f) const = 0;

		/**
		 * @brief Get the backend of the device.
		 *
		 * @return Backend The backend of the device.
		 */
		FORGE_NO_DISCARD virtual Backend backend() const = 0;

		virtual ~Device() {}
	};
} // namespace Forge::GPU
