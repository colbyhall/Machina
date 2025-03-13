/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Containers/Array.hpp>
#include <Core/Containers/StringView.hpp>

namespace Forge::Core {
	class Writer {
	public:
		virtual ~Writer() = default;
		virtual usize write(Slice<u8 const> bytes) = 0;
		FORGE_ALWAYS_INLINE usize write(StringView string) {
			return write(Slice<u8 const>((const u8*)*string, string.len()));
		}
	};

	class NullWriter final : public Writer {
	public:
		usize write(Slice<u8 const> bytes) final { return bytes.len(); }
	};

	template <usize Size = 4096>
	class BufferedWriter final : public Writer {
	public:
		explicit BufferedWriter(Writer& parent) : m_parent(parent) {}

		usize flush() {
			if (m_buffer.len() > 0) {
				const usize written = m_parent.write(m_buffer.as_const_slice());
				m_buffer.reset();
				return written;
			}
			return 0;
		}
		usize write(Slice<u8 const> bytes) final {
			// TODO: Make this less naive
			for (usize i = 0; i < bytes.len(); ++i) {
				if (m_buffer.len() == m_buffer.cap()) {
					flush();
				}
				m_buffer.push(bytes[i]);
			}
			return bytes.len();
		}

	private:
		Writer& m_parent;
		Array<u8, InlineAllocator<Size>> m_buffer;
	};
} // namespace Forge::Core
