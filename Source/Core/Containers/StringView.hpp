/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Containers/Option.hpp>
#include <Core/Containers/Slice.hpp>

namespace Mach::Core {
	class CharsIterator;

	class Char {
	public:
		Char(u32 codepoint = 0) : m_codepoint(codepoint) {}

		inline bool operator==(const Char& other) const { return m_codepoint == other.m_codepoint; }
		inline bool operator!=(const Char& other) const { return m_codepoint != other.m_codepoint; }
		inline bool operator<(const Char& other) const { return m_codepoint < other.m_codepoint; }
		inline bool operator>(const Char& other) const { return m_codepoint > other.m_codepoint; }
		inline bool operator<=(const Char& other) const { return m_codepoint <= other.m_codepoint; }
		inline bool operator>=(const Char& other) const { return m_codepoint >= other.m_codepoint; }
		inline u32 operator*() const { return m_codepoint; }

		inline bool is_ascii() const { return m_codepoint <= 0x7F; }
		inline bool is_ascii_alpha() const { return m_codepoint >= 0x41 && m_codepoint <= 0x7A; }
		inline bool is_ascii_digit() const { return m_codepoint >= 0x30 && m_codepoint <= 0x39; }
		bool is_whitespace() const;

	private:
		u32 m_codepoint;
	};
	using UTF8Char = char8_t;

	template <typename T>
	constexpr usize strlen(const T* string) {
		const T* current = string;
		for (;;) {
			if (current[0] == 0) {
				break;
			}
			current += 1;
		}
		return static_cast<usize>(current - string);
	}

	/**
	 * @brief A StringView represents an immutable view into a sequence of UTF-8 characters.
	 *
	 * It provides a lightweight way to handle strings without owning the underlying data.
	 * It is designed for performance and efficiency, especially in contexts where
	 * string ownership is not required.
	 */
	class StringView {
	public:
		MACH_ALWAYS_INLINE constexpr StringView() = default;
		MACH_ALWAYS_INLINE StringView(const Slice<UTF8Char const>& bytes) : m_bytes(bytes) {}
		MACH_ALWAYS_INLINE constexpr StringView(const UTF8Char* ptr, usize size) : m_bytes(ptr, size) {}
		MACH_ALWAYS_INLINE static StringView from_cstring(const char* ptr) {
			return StringView(reinterpret_cast<const UTF8Char*>(ptr), strlen(ptr));
		}

		MACH_ALWAYS_INLINE explicit operator Slice<UTF8Char const>() const { return m_bytes; }
		MACH_ALWAYS_INLINE const UTF8Char* operator*() const { return &m_bytes[0]; }

		MACH_ALWAYS_INLINE StringView substring(usize start, usize end) const {
			if (start == end) {
				return StringView{};
			}
			MACH_ASSERT(start <= end);
			return StringView{ &m_bytes[start], end - start };
		}

		MACH_NO_DISCARD MACH_ALWAYS_INLINE usize len() const { return m_bytes.len(); }
		MACH_NO_DISCARD CharsIterator chars() const;
		bool operator==(const StringView& right) const;
		bool operator!=(const StringView& right) const;

	private:
		Slice<UTF8Char const> m_bytes;
	};

	class CharsIterator {
	public:
		MACH_ALWAYS_INLINE explicit CharsIterator(const StringView& string)
			: m_string{ string }
			, m_byte_index{ 0 }
			, m_char_index{ 0 }
			, m_decoder_state{ 0 }
			, m_codepoint{ 0 } {}

		MACH_ALWAYS_INLINE explicit operator bool() const { return should_continue(); }
		MACH_ALWAYS_INLINE CharsIterator& operator++() {
			next();
			return *this;
		}
		MACH_ALWAYS_INLINE Char operator*() const { return get(); }
		MACH_NO_DISCARD MACH_ALWAYS_INLINE usize index() const { return m_char_index; }
		MACH_NO_DISCARD MACH_ALWAYS_INLINE usize byte_offset() const { return m_byte_index; }

	private:
		MACH_NO_DISCARD bool should_continue() const;
		void next();
		MACH_NO_DISCARD Char get() const;

		StringView m_string;
		usize m_byte_index;
		usize m_char_index;
		u32 m_decoder_state;
		Char m_codepoint;
	};
} // namespace Mach::Core

namespace Mach {
	using Core::StringView;

	template <typename Hasher>
	void hash(Hasher& hasher, const StringView& value) {
		const auto bytes = static_cast<Slice<Core::UTF8Char const>>(value).as_bytes();
		hasher.write(bytes);
	}
} // namespace Mach

constexpr Mach::StringView operator"" _sv(const Mach::Core::UTF8Char* literal, Mach::usize length) noexcept {
	return Mach::StringView(literal, length);
}
