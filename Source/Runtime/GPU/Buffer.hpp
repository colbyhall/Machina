/**
 * copyright (c) 2024 colby hall <me@cobeh.com>
 *
 * this software is released under the mit license.
 */

#pragma once

#include <Core/Containers/Function.hpp>
#include <Core/Containers/Shared.hpp>
#include <Core/Containers/Slice.hpp>
#include <GPU/Resource.hpp>

namespace Grizzly::GPU {
	class Buffer : public Resource, public SharedFromThis<Buffer> {
	public:
		enum class Usage : u8 {
			TransferSrc = (1 << 0),
			TransferDst = (1 << 1),
			Vertex = (1 << 2),
			Index = (1 << 3),
			Constant = (1 << 4),
		};

		enum class Heap : u8 {
			Storage,
			Upload,
			Download,
		};

		struct CreateInfo {
			Usage usage;
			Heap heap;
			usize len;
			usize stride;
		};

		GRIZZLY_ALWAYS_INLINE Usage usage() const { return m_usage; }
		GRIZZLY_ALWAYS_INLINE Heap heap() const { return m_heap; }
		GRIZZLY_ALWAYS_INLINE usize len() const { return m_len; }
		GRIZZLY_ALWAYS_INLINE usize stride() const { return m_stride; }

		virtual void map(FunctionRef<void(Slice<u8>)> f) const = 0;

	protected:
		Usage m_usage;
		Heap m_heap;
		usize m_len;
		usize m_stride;
	};
	GRIZZLY_ENUM_CLASS_BITFIELD(Buffer::Usage)
} // namespace Grizzly::GPU
