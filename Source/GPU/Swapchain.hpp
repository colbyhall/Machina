/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Containers/SharedPtr.hpp>
#include <Core/Containers/UniquePtr.hpp>
#include <GPU/Forward.hpp>
#include <GPU/Texture.hpp>

namespace Mach::GPU {
	class Backbuffer;

	class Swapchain {
	public:
		using Owner = void*;

		MACH_NO_DISCARD virtual UniquePtr<Backbuffer> next_back_buffer() = 0;

		virtual ~Swapchain() {}
	};

	class Backbuffer {
	public:
		MACH_NO_DISCARD virtual Texture const& texture() const = 0;
		virtual void present(Receipt const& wait_on) = 0;

		virtual ~Backbuffer() {}
	};
} // namespace Mach::GPU
