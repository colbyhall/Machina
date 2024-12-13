/**
 * copyright (c) 2024 colby hall <me@cobeh.com>
 *
 * this software is released under the mit license.
 */

#pragma once

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
	class Device : public SharedFromThis<Device> {
	public:
		struct CreateInfo {
			Backend backend;
		};

		/**
		 * @brief Create a device with the given create info.
		 *
		 * @param create_info The create info for the device.
		 * @return Shared<Device> The created device.
		 */
		GRIZZLY_NO_DISCARD static Shared<Device> create(CreateInfo const& create_info);

		/**
		 * @brief Create a swapchain for the given owner.
		 *
		 * @param owner The owner of the swapchain.
		 * @return Unique<Swapchain> The created swapchain.
		 */
		GRIZZLY_NO_DISCARD virtual Unique<Swapchain> create_swapchain(Swapchain::Owner owner) = 0;

		GRIZZLY_NO_DISCARD virtual Shared<Buffer> create_buffer(Buffer::CreateInfo const& info) = 0;
		GRIZZLY_NO_DISCARD virtual Shared<Texture> create_texture(Texture::CreateInfo const& info) = 0;
		// virtual Shared<GraphicsPipeline> create_graphics_pipeline(GraphicsPipeline::CreateInfo const& info) = 0;
		GRIZZLY_NO_DISCARD virtual Shared<CommandList> record(FunctionRef<void(CommandRecorder&)> f) = 0;

		/**
		 * @brief Get the backend of the device.
		 *
		 * @return Backend The backend of the device.
		 */
		GRIZZLY_NO_DISCARD virtual Backend backend() const = 0;

		virtual ~Device() {}
	};
} // namespace Grizzly::GPU
