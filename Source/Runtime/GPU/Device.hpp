/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <GPU/Forward.hpp>

namespace Grizzly::GPU {
	enum class Backend : u8 {
		Metal,
		DirectX,
		Vulkan,
	};

	class Device : public SharedFromThis<Device> {
	public:
		struct CreateInfo {
			Backend backend;
		};
		static Shared<Device> create(CreateInfo const& create_info);

		virtual Backend backend() const = 0;
		virtual ~Device() {}
	};
} // namespace Grizzly::GPU
