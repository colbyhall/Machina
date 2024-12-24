/**
 * copyright (c) 2024 colby hall <me@cobeh.com>
 *
 * this software is released under the mit license.
 */

#include <Core/Debug/Log.hpp>
#include <GPU/Metal/Buffer.hpp>

namespace Grizzly::GPU {
	void MetalBuffer::map(FunctionRef<void(Slice<u8>)> f) const {
		GRIZZLY_ASSERT(m_heap != Buffer::Heap::Storage);
		@autoreleasepool {
			void* const contents = [*m_buffer contents];
			f(Slice<u8>(static_cast<u8*>(contents), m_len * m_stride));
		}
	}
} // namespace Grizzly::GPU
