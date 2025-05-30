/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Containers/StringView.hpp>

namespace Mach::Core {
	bool Char::is_whitespace() const {
		return m_codepoint == 0x20 || m_codepoint == 0x09 || m_codepoint == 0x0A || m_codepoint == 0x0B ||
			   m_codepoint == 0x0C || m_codepoint == 0x0D;
	}

	CharsIterator StringView::chars() const { return CharsIterator(*this); }

	bool StringView::operator==(const StringView& right) const {
		// If our string are not the same length they can not be equal
		if (len() != right.len()) return false;

		// Compare bytes of strings
		for (usize i = 0; i < len(); i++) {
			if (m_bytes[i] != right.m_bytes[i]) return false;
		}

		return true;
	}

	bool StringView::operator!=(const StringView& right) const { return !(*this == right); }

	constexpr u32 utf8_accept = 0;
	constexpr u32 utf8_reject = 12;

	// clang-format off
	/**
	 * Created by Björn Höhrmann
	 *
	 * @see http://bjoern.hoehrmann.de/utf-8/decoder/dfa/
	 */
	static const u8 utf8d[] = {
		// The first part of the table maps bytes to character classes that
		// to reduce the size of the transition table and create bitmasks.
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,  9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
		7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,  7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
		8,8,2,2,2,2,2,2,2,2,2,2,2,2,2,2,  2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
		10,3,3,3,3,3,3,3,3,3,3,3,3,4,3,3, 11,6,6,6,5,8,8,8,8,8,8,8,8,8,8,8,

		// The second part is a transition table that maps a combination
		// of a state of the automaton and a character class to a state.
		0,12,24,36,60,96,84,12,12,12,48,72, 12,12,12,12,12,12,12,12,12,12,12,12,
		12, 0,12,12,12,12,12, 0,12, 0,12,12, 12,24,12,12,12,12,12,24,12,24,12,12,
		12,12,12,12,12,12,12,24,12,12,12,12, 12,24,12,12,12,12,12,12,12,24,12,12,
		12,12,12,12,12,12,12,36,12,36,12,12, 12,36,12,12,12,12,12,36,12,36,12,12,
		12,36,12,12,12,12,12,12,12,12,12,12,
	};
	// clang-format on

	static u32 utf8_decode(u32* state, u32* code_p, u32 byte) {
		const u32 type = utf8d[static_cast<size_t>(byte)];

		*code_p = (*state != utf8_accept) ? (byte & 0x3fu) | (*code_p << 6) : (0xff >> type) & (byte);

		*state = utf8d[256 + *state + type];
		return *state;
	}

	bool CharsIterator::should_continue() const {
		return m_string.len() > 0 && m_byte_index < m_string.len() && m_decoder_state != utf8_reject;
	}

	void CharsIterator::next() {
		MACH_ASSERT(should_continue());

		for (; m_byte_index < m_string.len(); m_byte_index += 1) {
			const UTF8Char c = (*m_string)[m_byte_index];
			utf8_decode(&m_decoder_state, (u32*)&m_codepoint, static_cast<u32>(c));

			if (m_decoder_state == utf8_reject) return;
			if (m_decoder_state != utf8_accept) continue;

			break;
		}

		m_byte_index += 1;
		m_char_index += 1;
	}

	Char CharsIterator::get() const {
		usize get_index = m_byte_index;
		u32 get_state = m_decoder_state;
		u32 get_codepoint = *m_codepoint;
		for (; get_index < m_string.len(); get_index += 1) {
			const UTF8Char c = (*m_string)[get_index];
			utf8_decode(&get_state, &get_codepoint, static_cast<u32>(c));

			if (get_state == utf8_reject) return 0xfffd;
			if (get_state != utf8_accept) continue;

			break;
		}
		return get_codepoint;
	}
} // namespace Mach::Core

#include <Core/Debug/Test.hpp>

#if MACH_ENABLE_TEST
MACH_TEST_SUITE("Containers") {
	using namespace Mach::Core;

	MACH_TEST_CASE("StringView") {
	#if 0
		const StringView foo = u8"aΠ1"sv;
		const Char chars[] = { 'a', 0x03A0, '1' };
		for (auto iter = foo.chars(); iter; ++iter) {
			const auto c = *iter;
			MACH_CHECK(c == chars[iter.index()]);
		}
	#endif
	}
}
#endif // MACH_ENABLE_TEST
