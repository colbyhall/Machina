/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Containers/Shared.hpp>
#include <Core/Containers/UniquePtr.hpp>
#include <GPU/Forward.hpp>
#include <GPU/Texture.hpp>

namespace Forge::GPU {
	class Backbuffer;

	class Swapchain {
	public:
		using Owner = void*;

		FORGE_NO_DISCARD virtual UniquePtr<Backbuffer> next_back_buffer() = 0;

		virtual ~Swapchain() {}
	};

	class Backbuffer {
	public:
		FORGE_NO_DISCARD virtual Texture const& texture() const = 0;
		virtual void present(Receipt const& wait_on) = 0;

		virtual ~Backbuffer() {}
	};
} // namespace Forge::GPU
