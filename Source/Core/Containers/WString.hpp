/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Containers/Array.hpp>
#include <Core/Containers/WStringView.hpp>

namespace Forge::Core {
	WChar utf32_to_utf16(Char c);
	Char utf16_to_utf32(WChar c);

	class WString {
	public:
		WString() = default;

		static WString from(const WStringView& view);
		static WString from(const StringView& string);

		operator WStringView() const;
		FORGE_NO_DISCARD FORGE_ALWAYS_INLINE const WChar* operator*() const { return m_chars.begin(); }

		FORGE_NO_DISCARD FORGE_ALWAYS_INLINE usize len() const { return m_chars.len() > 0 ? m_chars.len() - 1 : 0; }
		FORGE_NO_DISCARD FORGE_ALWAYS_INLINE usize cap() const { return m_chars.cap(); }

		FORGE_NO_DISCARD FORGE_ALWAYS_INLINE WChar* begin() { return m_chars.begin(); }
		FORGE_NO_DISCARD FORGE_ALWAYS_INLINE WChar* end() { return m_chars.end(); }
		FORGE_NO_DISCARD FORGE_ALWAYS_INLINE const WChar* begin() const { return m_chars.begin(); }
		FORGE_NO_DISCARD FORGE_ALWAYS_INLINE const WChar* end() const { return m_chars.end(); }

		FORGE_ALWAYS_INLINE WChar& operator[](usize index) { return m_chars[index]; }
		FORGE_ALWAYS_INLINE WChar operator[](usize index) const { return m_chars[index]; }

		FORGE_ALWAYS_INLINE bool operator==(const WStringView& rhs) const {
			const WStringView view = static_cast<WStringView>(*this);
			return view == rhs;
		}
		FORGE_ALWAYS_INLINE bool operator!=(const WStringView& rhs) const {
			const WStringView view = static_cast<WStringView>(*this);
			return view != rhs;
		}

		FORGE_ALWAYS_INLINE void reserve(usize amount) { return m_chars.reserve(amount + 1); }
		WString& push(WChar w);
		WString& append(const WStringView& string);
		WString& append(const StringView& string);
		FORGE_ALWAYS_INLINE void set_len(usize len) { m_chars.set_len(len + 1); }

	private:
		Array<WChar> m_chars;
	};
} // namespace Forge::Core

namespace Forge {
	using Core::WString;
} // namespace Forge
