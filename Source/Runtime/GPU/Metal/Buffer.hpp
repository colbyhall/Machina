/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * this software is released under the mit license.
 */

#pragma once

#import <Metal/Metal.h>

#include <Core/ObjectiveC/Protocol.hpp>
#include <GPU/Buffer.hpp>

namespace Grizzly::GPU {
	class MetalBuffer final : public Buffer {
	public:
		explicit MetalBuffer(Buffer::CreateInfo const& create_info, id<MTLBuffer> buffer)
			: Buffer(create_info)
			, m_buffer(buffer) {}

		void map(FunctionRef<void(Slice<u8>)> f) const final;

		GRIZZLY_ALWAYS_INLINE Core::Protocol buffer() const { return m_buffer; }

	private:
		Core::Protocol m_buffer; // MTLBuffer
	};
} // namespace Grizzly::GPU
