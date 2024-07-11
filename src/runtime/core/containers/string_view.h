/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include "core/containers/option.h"
#include "core/containers/slice.h"

namespace op::core {
	class CharsIterator;

	template <typename T>
	constexpr usize constexpr_strlen(const T* string) {
		usize result = 0;
		while (string[result] != 0) {
			result += 1;
		}
		return result;
	}

	using Char = u32;
	constexpr Char EOS = 0;
	constexpr Char EOL = '\n';
	constexpr Char UTF8_BOM = 0xfeff;
	using UTF8Char = char8_t;

	class StringView {
	public:
		OP_ALWAYS_INLINE constexpr StringView() = default;
		OP_ALWAYS_INLINE StringView(const Slice<UTF8Char const>& bytes) : m_bytes(bytes) {}
		OP_ALWAYS_INLINE constexpr StringView(const UTF8Char* ptr) : m_bytes(ptr, constexpr_strlen(ptr)) {}
		OP_ALWAYS_INLINE constexpr explicit StringView(const UTF8Char* ptr, usize size) : m_bytes(ptr, size) {}

		OP_ALWAYS_INLINE explicit operator Slice<UTF8Char const>() const { return m_bytes; }
		OP_ALWAYS_INLINE const UTF8Char* operator*() const { return &m_bytes[0]; }

		OP_NO_DISCARD OP_ALWAYS_INLINE usize len() const { return m_bytes.len(); }
		OP_NO_DISCARD CharsIterator chars() const;
		bool operator==(const StringView& right) const;
		bool operator!=(const StringView& right) const;

	private:
		Slice<UTF8Char const> m_bytes;
	};

	class CharsIterator {
	public:
		OP_ALWAYS_INLINE explicit CharsIterator(const StringView& string)
			: m_string{ string }
			, m_byte_index{ 0 }
			, m_char_index{ 0 }
			, m_decoder_state{ 0 }
			, m_codepoint{ 0 } {}

		OP_ALWAYS_INLINE explicit operator bool() const { return should_continue(); }
		OP_ALWAYS_INLINE CharsIterator& operator++() {
			next();
			return *this;
		}
		OP_ALWAYS_INLINE Char operator*() const { return get(); }
		OP_NO_DISCARD OP_ALWAYS_INLINE usize index() const { return m_char_index; }

	private:
		OP_NO_DISCARD bool should_continue() const;
		void next();
		OP_NO_DISCARD Char get() const;

		StringView m_string;
		usize m_byte_index;
		usize m_char_index;
		u32 m_decoder_state;
		Char m_codepoint;
	};

} // namespace op::core