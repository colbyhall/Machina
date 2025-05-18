/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * this software is released under the mit license.
 */

#pragma once

#import <Metal/Metal.h>

#include <Core/ObjectiveC/Protocol.hpp>
#include <GPU/Buffer.hpp>

namespace Forge::GPU {
	class MetalBuffer final : public Buffer {
	public:
		explicit MetalBuffer(Usage usage, Heap heap, usize size, id<MTLBuffer> buffer)
			: Buffer(usage, heap, size)
			, m_buffer(buffer) {}

		FORGE_ALWAYS_INLINE Core::Protocol buffer() const { return m_buffer; }

	private:
		Core::Protocol m_buffer; // MTLBuffer
	};
} // namespace Forge::GPU
