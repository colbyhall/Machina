/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Containers/WString.hpp>

namespace Grizzly::Core {
WChar utf32_to_utf16(Char c) {
  u32 h;
  u32 l;

  if (c < 0x10000) {
    h = 0;
    l = c;
    return static_cast<WChar>(c);
  }
  Char t = c - 0x10000;
  h = (((t << 12) >> 22) + 0xD800);
  l = (((t << 22) >> 22) + 0xDC00);
  Char ret = ((h << 16) | (l & 0x0000FFFF));
  return static_cast<WChar>(ret);
}

Char utf16_to_utf32(WChar c) {
  if (c < 0xD800 || c > 0xDFFF) {
    return static_cast<Char>(c);
  }
  Char h = (c & 0xFFFF0000) >> 16;
  Char l = (c & 0x0000FFFF);
  return static_cast<Char>(((h - 0xD800) << 10) + (l - 0xDC00) + 0x10000);
}

WString WString::from(const WStringView &view) {
  WString string;
  string.append(view);
  return string;
}

WString WString::from(const StringView &string) {
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

WString &WString::push(WChar w) {
  const auto start_len = m_chars.len();

  if (start_len == 0) {
    m_chars.push(0);
  }

  // Append the null terminator and then replace the old one
  m_chars.push(0);
  m_chars[m_chars.len() - 2] = w;

  return *this;
}

WString &WString::append(const WStringView &string) {
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

WString &WString::append(const StringView &string) {
  const usize slag = m_chars.cap() - m_chars.len();
  if (slag < string.len()) {
    m_chars.reserve(string.len());
  }

  for (auto iter = string.chars(); iter; ++iter) {
    push(utf32_to_utf16(*iter));
  }

  return *this;
}
} // namespace Grizzly::Core

#include <Core/Debug/Test.hpp>

#if GRIZZLY_ENABLE_TEST
GRIZZLY_TEST_SUITE("Containers") {
  using namespace Grizzly::Core;

  GRIZZLY_TEST_CASE("WString") {
    GRIZZLY_SUBCASE("default constructor") {
      const WString string;
      GRIZZLY_CHECK(string.len() == 0);
    }

    GRIZZLY_SUBCASE("from WStringView") {
      const WStringView view = L"Hello, World!";
      const WString string = WString::from(view);
      GRIZZLY_CHECK(string.len() == view.len());
      GRIZZLY_CHECK(string == view);
    }

    GRIZZLY_SUBCASE("from StringView") {
      const StringView view = u8"Hello, World!"sv;
      const WString string = WString::from(view);
      GRIZZLY_CHECK(string.len() == view.len());
      GRIZZLY_CHECK(string == L"Hello, World!");
    }

    GRIZZLY_SUBCASE("push") {
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
      GRIZZLY_CHECK(string.len() == 13);
      GRIZZLY_CHECK(string == L"Hello, World!");
    }

    GRIZZLY_SUBCASE("append WStringView") {
      WString string;
      string.append(L"Hello, ");
      string.append(L"World!");
      GRIZZLY_CHECK(string.len() == 13);
      GRIZZLY_CHECK(string == L"Hello, World!");
    }

    GRIZZLY_SUBCASE("append StringView") {
      WString string;
      string.append(u8"Hello, "sv);
      string.append(u8"World!"sv);
      GRIZZLY_CHECK(string.len() == 13);
      GRIZZLY_CHECK(string == L"Hello, World!");
    }
  }
}
#endif
