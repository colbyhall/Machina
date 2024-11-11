/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Containers/Option.hpp>
#include <Core/Containers/Slice.hpp>

namespace Grizzly::Core {
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
		GRIZZLY_ALWAYS_INLINE constexpr StringView() = default;
		GRIZZLY_ALWAYS_INLINE StringView(const Slice<UTF8Char const>& bytes) : m_bytes(bytes) {}
		GRIZZLY_ALWAYS_INLINE constexpr StringView(const UTF8Char* ptr) : m_bytes(ptr, constexpr_strlen(ptr)) {}
		GRIZZLY_ALWAYS_INLINE constexpr explicit StringView(const UTF8Char* ptr, usize size) : m_bytes(ptr, size) {}

		GRIZZLY_ALWAYS_INLINE explicit operator Slice<UTF8Char const>() const { return m_bytes; }
		GRIZZLY_ALWAYS_INLINE const UTF8Char* operator*() const { return &m_bytes[0]; }

		GRIZZLY_ALWAYS_INLINE StringView substring(usize start, usize end) const {
			if (start == end) {
				return StringView{};
			}
			GRIZZLY_ASSERT(start <= end);
			return StringView{ &m_bytes[start], end - start };
		}

		GRIZZLY_NO_DISCARD GRIZZLY_ALWAYS_INLINE usize len() const { return m_bytes.len(); }
		GRIZZLY_NO_DISCARD CharsIterator chars() const;
		bool operator==(const StringView& right) const;
		bool operator!=(const StringView& right) const;

	private:
		Slice<UTF8Char const> m_bytes;
	};

	class CharsIterator {
	public:
		GRIZZLY_ALWAYS_INLINE explicit CharsIterator(const StringView& string)
			: m_string{ string }
			, m_byte_index{ 0 }
			, m_char_index{ 0 }
			, m_decoder_state{ 0 }
			, m_codepoint{ 0 } {}

		GRIZZLY_ALWAYS_INLINE explicit operator bool() const { return should_continue(); }
		GRIZZLY_ALWAYS_INLINE CharsIterator& operator++() {
			next();
			return *this;
		}
		GRIZZLY_ALWAYS_INLINE Char operator*() const { return get(); }
		GRIZZLY_NO_DISCARD GRIZZLY_ALWAYS_INLINE usize index() const { return m_char_index; }
		GRIZZLY_NO_DISCARD GRIZZLY_ALWAYS_INLINE usize byte_offset() const { return m_byte_index; }

	private:
		GRIZZLY_NO_DISCARD bool should_continue() const;
		void next();
		GRIZZLY_NO_DISCARD Char get() const;

		StringView m_string;
		usize m_byte_index;
		usize m_char_index;
		u32 m_decoder_state;
		Char m_codepoint;
	};
} // namespace Grizzly::Core

namespace Grizzly {
	using Core::StringView;
} // namespace Grizzly
