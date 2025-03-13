/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * this software is released under the mit license.
 */

#include <Core/Debug/Log.hpp>
#include <GPU/Metal/Buffer.hpp>

namespace Forge::GPU {
	void MetalBuffer::map(FunctionRef<void(Slice<u8>)> f) const {
		FORGE_ASSERT(m_heap != Heap::Storage);
		@autoreleasepool {
			void* const contents = [*m_buffer contents];
			f(Slice<u8>(static_cast<u8*>(contents), m_len * m_stride));
		}
	}
} // namespace Forge::GPU
