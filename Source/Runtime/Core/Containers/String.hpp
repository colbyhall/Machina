/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Containers/Array.hpp>
#include <Core/Containers/StringView.hpp>

namespace Grizzly::Core {
	class String {
	public:
		String() = default;
		GRIZZLY_NO_DISCARD static String from(const StringView& s);

		GRIZZLY_ALWAYS_INLINE explicit operator StringView() const;

		GRIZZLY_NO_DISCARD GRIZZLY_ALWAYS_INLINE CharsIterator chars() const {
			return CharsIterator(m_bytes.as_const_slice());
		}
		GRIZZLY_NO_DISCARD GRIZZLY_ALWAYS_INLINE usize len() const { return m_bytes.len() > 0 ? m_bytes.len() - 1 : 0; }
		GRIZZLY_NO_DISCARD GRIZZLY_ALWAYS_INLINE usize cap() const { return m_bytes.cap(); }

		GRIZZLY_ALWAYS_INLINE bool operator==(const StringView& rhs) const {
			const StringView view = static_cast<StringView>(*this);
			return view == rhs;
		}
		GRIZZLY_ALWAYS_INLINE bool operator!=(const StringView& rhs) const {
			const StringView view = static_cast<StringView>(*this);
			return view != rhs;
		}

		GRIZZLY_ALWAYS_INLINE void reserve(usize amount) { m_bytes.reserve(amount + 1); }
		String& push(Char c);
		String& append(const StringView& string);
		GRIZZLY_ALWAYS_INLINE const UTF8Char* operator*() const { return &m_bytes[0]; }

	private:
		Array<UTF8Char> m_bytes;
	};
} // namespace Grizzly::Core

namespace Grizzly {
	using Core::String;
} // namespace Grizzly
