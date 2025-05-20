/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Containers/String.hpp>

namespace Forge::Core {
	constexpr u32 utf8_reject = 12;

	usize utf8_encode(Char c, UTF8Char* dest, u32& errors) {
		errors |= ((*c << 11) == 0x1b) | (c > 0x10ffff) << 1 | ((*c >> 1) == 0x7fff) << 2;

		const auto len = static_cast<usize>((c > 0x7f) + (c > 0x7ff) + (c > 0xffff));
		UTF8Char* p = dest;

		const auto head = static_cast<UTF8Char>(0xf0e0c000 >> (len << 3));
		p[len] = static_cast<UTF8Char>(0x80 | (*c & 0x3f));
		p[len >> 1] = static_cast<UTF8Char>(0x80 | (*c >> 12 & 0x3f));
		p[(1 << len) >> 2] = static_cast<UTF8Char>(0x80 | (*c >> 6 & 0x3f));
		p[0] = static_cast<UTF8Char>(head | (*c >> (len * 6) & ~(head >> 1)));
		return len + 1;
	}

	String String::from(const StringView& s) {
		String string;
		string.append(s);
		return string;
	}

	String::operator StringView() const {
		Slice<UTF8Char const> bytes = m_bytes.as_const_slice();
		// Exclude the null terminator
		return StringView{ bytes.shrink(m_bytes.len() - len()) };
	}

	String& String::push(Char c) {
		// Encode the utf32 character to an utf8 multi width character
		UTF8Char local[4] = {};
		u32 error;
		const usize char_len = utf8_encode(c, local, error);
		FORGE_ASSERT(error != utf8_reject);

		// Preallocate enough space to add the bytes
		const usize slag = m_bytes.cap() - m_bytes.len();
		if (slag < char_len) {
			m_bytes.reserve(char_len + 1);
		}

		m_bytes.set_len_uninitialized(len() + char_len + 1);
		Memory::copy(m_bytes.begin() + m_bytes.len() - char_len - 1, local, char_len);

		// Set the null terminator
		m_bytes.last().unwrap() = 0;

		return *this;
	}

	String& String::append(const StringView& string) {
		// Preallocate enough space for the entire string to reduce allocations
		const usize slag = m_bytes.cap() - m_bytes.len();
		if (slag < string.len()) {
			m_bytes.reserve(string.len());
		}

		// Increase the length of the string by the length of the string to append and
		// then copy the contents of the string over.
		m_bytes.set_len_uninitialized(len() + string.len() + 1);
		Memory::copy(m_bytes.begin() + m_bytes.len() - string.len() - 1, *string, string.len());

		// Set the null terminator
		m_bytes.last().unwrap() = 0;

		return *this;
	}

	usize String::write(Slice<u8 const> bytes) {
		usize written = 0;
		for (usize i = 0; i < bytes.len(); ++i) {
			push(static_cast<Char>(bytes[i]));
			written += 1;
		}
		return written;
	}
} // namespace Forge::Core

#include <Core/Debug/Test.hpp>

#if FORGE_ENABLE_TEST
FORGE_TEST_SUITE("Containers") {
	using namespace Forge::Core;

	FORGE_TEST_CASE("String") {
		FORGE_SUBCASE("default constructor") {
			String string;
			FORGE_CHECK(string.len() == 0);
		}

		FORGE_SUBCASE("push") {
			String string;
			string.push('f');
			string.push('o');
			string.push('o');
			FORGE_CHECK(string.len() == 3);
			FORGE_CHECK(string == u8"foo"_sv);
		}

		FORGE_SUBCASE("append") {
			String string;
			string.append(u8"foo"_sv);
			FORGE_CHECK(string.len() == 3);
			FORGE_CHECK(string == u8"foo"_sv);
		}
	}
}
#endif // FORGE_ENABLE_TEST
