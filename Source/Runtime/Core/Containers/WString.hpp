/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Containers/Array.hpp>
#include <Core/Containers/WStringView.hpp>

namespace Grizzly::Core {
	WChar utf32_to_utf16(Char c);
	Char utf16_to_utf32(WChar c);

	class WString {
	public:
		WString() = default;

		static WString from(const WStringView& view);
		static WString from(const StringView& string);

		operator WStringView() const;
		GRIZZLY_NO_DISCARD GRIZZLY_ALWAYS_INLINE const WChar* operator*() const { return m_chars.begin(); }

		GRIZZLY_NO_DISCARD GRIZZLY_ALWAYS_INLINE usize len() const { return m_chars.len() > 0 ? m_chars.len() - 1 : 0; }
		GRIZZLY_NO_DISCARD GRIZZLY_ALWAYS_INLINE usize cap() const { return m_chars.cap(); }

		GRIZZLY_NO_DISCARD GRIZZLY_ALWAYS_INLINE WChar* begin() { return m_chars.begin(); }
		GRIZZLY_NO_DISCARD GRIZZLY_ALWAYS_INLINE WChar* end() { return m_chars.end(); }
		GRIZZLY_NO_DISCARD GRIZZLY_ALWAYS_INLINE const WChar* begin() const { return m_chars.begin(); }
		GRIZZLY_NO_DISCARD GRIZZLY_ALWAYS_INLINE const WChar* end() const { return m_chars.end(); }

		GRIZZLY_ALWAYS_INLINE WChar& operator[](usize index) { return m_chars[index]; }
		GRIZZLY_ALWAYS_INLINE WChar operator[](usize index) const { return m_chars[index]; }

		GRIZZLY_ALWAYS_INLINE bool operator==(const WStringView& rhs) const {
			const WStringView view = static_cast<WStringView>(*this);
			return view == rhs;
		}
		GRIZZLY_ALWAYS_INLINE bool operator!=(const WStringView& rhs) const {
			const WStringView view = static_cast<WStringView>(*this);
			return view != rhs;
		}

		GRIZZLY_ALWAYS_INLINE void reserve(usize amount) { return m_chars.reserve(amount + 1); }
		WString& push(WChar w);
		WString& append(const WStringView& string);
		WString& append(const StringView& string);
		GRIZZLY_ALWAYS_INLINE void set_len(usize len) { m_chars.set_len(len + 1); }

	private:
		Array<WChar> m_chars;
	};
} // namespace Grizzly::Core

namespace Grizzly {
	using Core::WString;
} // namespace Grizzly
