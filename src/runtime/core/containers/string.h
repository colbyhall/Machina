/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include "core/containers/array.h"
#include "core/containers/string_view.h"

namespace op::core {
	class String {
	public:
		String() = default;
		OP_NO_DISCARD static String from(const StringView& s);

		OP_ALWAYS_INLINE explicit operator StringView() const;

		OP_NO_DISCARD OP_ALWAYS_INLINE CharsIterator chars() const { return CharsIterator(m_bytes.as_const_slice()); }
		OP_NO_DISCARD OP_ALWAYS_INLINE usize len() const { return m_bytes.len() > 0 ? m_bytes.len() - 1 : 0; }
		OP_NO_DISCARD OP_ALWAYS_INLINE usize cap() const { return m_bytes.cap(); }

		OP_ALWAYS_INLINE bool operator==(const StringView& rhs) const {
			const StringView view = static_cast<StringView>(*this);
			return view == rhs;
		}
		OP_ALWAYS_INLINE bool operator!=(const StringView& rhs) const {
			const StringView view = static_cast<StringView>(*this);
			return view != rhs;
		}

		OP_ALWAYS_INLINE void reserve(usize amount) { m_bytes.reserve(amount + 1); }
		String& push(Char c);
		String& append(const StringView& string);

	private:
		Array<UTF8Char> m_bytes;
	};
} // namespace op::core

namespace op {
	using core::String;
} // namespace op