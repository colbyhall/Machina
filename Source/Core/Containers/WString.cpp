/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Containers/WString.hpp>

namespace Forge::Core {
	WChar utf32_to_utf16(Char c) {
		u32 h;
		u32 l;

		if (c < 0x10000) {
			h = 0;
			l = *c;
			return static_cast<WChar>(*c);
		}
		Char t = *c - 0x10000;
		h = (((*t << 12) >> 22) + 0xD800);
		l = (((*t << 22) >> 22) + 0xDC00);
		Char ret = ((h << 16) | (l & 0x0000FFFF));
		return static_cast<WChar>(*ret);
	}

	Char utf16_to_utf32(WChar c) {
		if (c < 0xD800 || c > 0xDFFF) {
			return Char(static_cast<u32>(c));
		}
		Char h = Char(static_cast<u32>((c & 0xFFFF0000) >> 16));
		Char l = Char(static_cast<u32>((c & 0x0000FFFF)));
		return Char(static_cast<u32>(((*h - 0xD800) << 10) + (*l - 0xDC00) + 0x10000));
	}

	WString WString::from(const WStringView& view) {
		WString string;
		string.append(view);
		return string;
	}

	WString WString::from(const StringView& string) {
		WString result;
		result.reserve(string.len());

		for (auto iter = string.chars(); iter; ++iter) {
			const Char c = *iter;
			result.push(utf32_to_utf16(c));
		}

		return result;
	}

	WString::operator WStringView() const {
		const Slice<WChar const> bytes = m_chars.as_const_slice();

		// Exclude the null terminator
		return bytes.shrink(m_chars.len() - len());
	}

	WString& WString::push(WChar w) {
		const auto start_len = m_chars.len();

		if (start_len == 0) {
			m_chars.push(0);
		}

		// Append the null terminator and then replace the old one
		m_chars.push(0);
		m_chars[m_chars.len() - 2] = w;

		return *this;
	}

	WString& WString::append(const WStringView& string) {
		const usize slag = m_chars.cap() - m_chars.len();
		if (slag < string.len()) {
			m_chars.reserve(string.len());
		}

		if (m_chars.len() == 0) {
			m_chars.push(0);
		}
		for (WChar w : string)
			m_chars.insert(m_chars.len() - 1, w);

		return *this;
	}

	WString& WString::append(const StringView& string) {
		const usize slag = m_chars.cap() - m_chars.len();
		if (slag < string.len()) {
			m_chars.reserve(string.len());
		}

		for (auto iter = string.chars(); iter; ++iter) {
			push(utf32_to_utf16(*iter));
		}

		return *this;
	}
} // namespace Forge::Core

#include <Core/Debug/Test.hpp>

#if FORGE_ENABLE_TEST
FORGE_TEST_SUITE("Containers") {
	using namespace Forge::Core;

	FORGE_TEST_CASE("WString") {
		FORGE_SUBCASE("default constructor") {
			const WString string;
			FORGE_CHECK(string.len() == 0);
		}

		FORGE_SUBCASE("from WStringView") {
			const WStringView view = L"Hello, World!";
			const WString string = WString::from(view);
			FORGE_CHECK(string.len() == view.len());
			FORGE_CHECK(string == view);
		}

		FORGE_SUBCASE("from StringView") {
			const StringView view = u8"Hello, World!"_sv;
			const WString string = WString::from(view);
			FORGE_CHECK(string.len() == view.len());
			FORGE_CHECK(string == L"Hello, World!");
		}

		FORGE_SUBCASE("push") {
			WString string;
			string.reserve(13);
			string.push(L'H');
			string.push(L'e');
			string.push(L'l');
			string.push(L'l');
			string.push(L'o');
			string.push(L',');
			string.push(L' ');
			string.push(L'W');
			string.push(L'o');
			string.push(L'r');
			string.push(L'l');
			string.push(L'd');
			string.push(L'!');
			FORGE_CHECK(string.len() == 13);
			FORGE_CHECK(string == L"Hello, World!");
		}

		FORGE_SUBCASE("append WStringView") {
			WString string;
			string.append(L"Hello, ");
			string.append(L"World!");
			FORGE_CHECK(string.len() == 13);
			FORGE_CHECK(string == L"Hello, World!");
		}

		FORGE_SUBCASE("append StringView") {
			WString string;
			string.append(u8"Hello, "_sv);
			string.append(u8"World!"_sv);
			FORGE_CHECK(string.len() == 13);
			FORGE_CHECK(string == L"Hello, World!");
		}
	}
}
#endif
