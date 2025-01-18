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

namespace Grizzly::GPU {
	/**
	 * Defines the supported graphic api backends.
	 */
	enum class Backend : u8 {
		Metal,
		DirectX,
		Vulkan,
	};

	/**
	 * An abstraction for interacting with a graphics API.
	 *
	 * Use this class to utilize the graphics cards for rendering or computation.
	 */
	class Device : public RcFromThis<Device> {
	public:
		struct CreateInfo {
			Backend backend;
		};

		/**
		 * @brief Create a device with the given create info.
		 *
		 * @param create_info The create info for the device.
		 * @return Rc<Device> The created device.
		 */
		GRIZZLY_NO_DISCARD static Rc<Device> create(CreateInfo const& create_info);

		/**
		 * @brief Create a swapchain for the given owner.
		 *
		 * @param owner The owner of the swapchain.
		 * @return Unique<Swapchain> The created swapchain.
		 */
		GRIZZLY_NO_DISCARD virtual Unique<Swapchain> create_swapchain(Swapchain::Owner owner) = 0;

		/**
		 * @brief Creates a buffer.
		 *
		 * @param info The data structure containg the immutable details of the buffer.
		 * @return Rc<Buffer> The created buffer.
		 */
		GRIZZLY_NO_DISCARD virtual Rc<Buffer> create_buffer(Buffer::CreateInfo const& info) = 0;

		/**
		 * @brief Creates a texture.
		 *
		 * @param info The data structure containg the immutable details of the texture.
		 * @return Rc<Texture> The created texture.
		 */
		GRIZZLY_NO_DISCARD virtual Rc<Texture> create_texture(Texture::CreateInfo const& info) = 0;

		GRIZZLY_NO_DISCARD virtual Rc<Library> create_library_from_source(StringView source) = 0;

		GRIZZLY_NO_DISCARD virtual Rc<GraphicsPipeline>
		create_graphics_pipeline(GraphicsPipeline::CreateInfo const& info) = 0;

		GRIZZLY_NO_DISCARD virtual Rc<CommandList> record(FunctionRef<void(CommandRecorder&)> f) = 0;

		/**
		 * @brief Get the backend of the device.
		 *
		 * @return Backend The backend of the device.
		 */
		GRIZZLY_NO_DISCARD virtual Backend backend() const = 0;

		virtual ~Device() {}
	};
} // namespace Grizzly::GPU
