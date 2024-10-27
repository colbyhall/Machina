/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <GPU/Device.hpp>
#import <Metal/Metal.h>

namespace Grizzly::GPU {
	class MetalDevice final : public Device {
	public:
		explicit MetalDevice(id<MTLDevice> device) : m_device(device) {}
		MetalDevice(const MetalDevice&) = delete;
		MetalDevice& operator=(const MetalDevice&) = delete;
		MetalDevice(MetalDevice&& move) : m_device(move.m_device) { move.m_device = nil; }
		MetalDevice& operator=(MetalDevice&& move) {
			auto to_destroy = Grizzly::move(*this);
			m_device = move.m_device;
			move.m_device = nil;
			return *this;
		}

		~MetalDevice() final;
		Backend backend() const final { return Backend::Metal; }

	private:
		id<MTLDevice> m_device;
	};

	Shared<Device> create_metal_device(Device::CreateInfo const& create_info);
} // namespace Grizzly::GPU
