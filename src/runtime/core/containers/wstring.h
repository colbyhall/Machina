/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include "core/containers/array.h"
#include "core/containers/wstring_view.h"

namespace op::core {
	WChar utf32_to_utf16(Char c);

	class WString {
	public:
		WString() = default;

		static WString from(const WStringView& view);
		static WString from(const StringView& string);

		explicit operator WStringView() const;
		OP_NO_DISCARD OP_ALWAYS_INLINE const WChar* operator*() const { return m_chars.begin(); }

		OP_NO_DISCARD OP_ALWAYS_INLINE usize len() const { return m_chars.len() > 0 ? m_chars.len() - 1 : 0; }
		OP_NO_DISCARD OP_ALWAYS_INLINE usize cap() const { return m_chars.cap(); }

		OP_NO_DISCARD OP_ALWAYS_INLINE WChar* begin() { return m_chars.begin(); }
		OP_NO_DISCARD OP_ALWAYS_INLINE WChar* end() { return m_chars.end(); }
		OP_NO_DISCARD OP_ALWAYS_INLINE const WChar* begin() const { return m_chars.begin(); }
		OP_NO_DISCARD OP_ALWAYS_INLINE const WChar* end() const { return m_chars.end(); }

		OP_ALWAYS_INLINE WChar& operator[](usize index) { return m_chars[index]; }
		OP_ALWAYS_INLINE WChar operator[](usize index) const { return m_chars[index]; }

		OP_ALWAYS_INLINE bool operator==(const WStringView& rhs) const {
			const WStringView view = static_cast<WStringView>(*this);
			return view == rhs;
		}
		OP_ALWAYS_INLINE bool operator!=(const WStringView& rhs) const {
			const WStringView view = static_cast<WStringView>(*this);
			return view != rhs;
		}

		OP_ALWAYS_INLINE void reserve(usize amount) { return m_chars.reserve(amount + 1); }
		WString& push(WChar w);
		WString& append(const WStringView& string);
		WString& append(const StringView& string);

	private:
		Array<WChar> m_chars;
	};

} // namespace op::core