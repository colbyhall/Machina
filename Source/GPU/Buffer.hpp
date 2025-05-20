/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * this software is released under the mit license.
 */

#pragma once

#include <Core/Containers/Function.hpp>
#include <Core/Containers/SharedPtr.hpp>
#include <Core/Containers/Slice.hpp>
#include <GPU/Resource.hpp>

namespace Forge::GPU {
	enum class Heap : u8 {
		Storage,
		Upload,
		Download,
	};

	class Buffer : public Resource, public SharedPtrFromThis<Buffer> {
	public:
		enum class Usage : u8 {
			TransferSrc = (1 << 0),
			TransferDst = (1 << 1),
			Vertex = (1 << 2),
			Index = (1 << 3),
			Constant = (1 << 4),
		};

		FORGE_ALWAYS_INLINE Usage usage() const { return m_usage; }
		FORGE_ALWAYS_INLINE Heap heap() const { return m_heap; }
		FORGE_ALWAYS_INLINE usize len() const { return m_len; }

	protected:
		explicit Buffer(Usage usage, Heap heap, usize len) : m_usage(usage), m_heap(heap), m_len(len) {}

		Usage m_usage;
		Heap m_heap;
		usize m_len;
	};
	FORGE_ENUM_CLASS_BITFIELD(Buffer::Usage)
} // namespace Forge::GPU
