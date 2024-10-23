/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Containers/StringView.h>

namespace Grizzly::Core {
	using WChar = wchar_t;

	class WStringView {
	public:
		GRIZZLY_ALWAYS_INLINE constexpr WStringView() : m_chars{} {}
		GRIZZLY_ALWAYS_INLINE WStringView(const Slice<WChar const>& bytes) : m_chars{ bytes } {}
		GRIZZLY_ALWAYS_INLINE constexpr WStringView(const WChar* ptr) : m_chars{ ptr, Core::constexpr_strlen(ptr) } {}

		GRIZZLY_NO_DISCARD GRIZZLY_ALWAYS_INLINE usize len() const { return m_chars.len(); }
		GRIZZLY_NO_DISCARD GRIZZLY_ALWAYS_INLINE const WChar* operator*() const { return &m_chars[0]; }

		GRIZZLY_NO_DISCARD GRIZZLY_ALWAYS_INLINE const WChar* begin() const { return m_chars.cbegin(); }
		GRIZZLY_NO_DISCARD GRIZZLY_ALWAYS_INLINE const WChar* end() const { return m_chars.cend(); }
		GRIZZLY_NO_DISCARD GRIZZLY_ALWAYS_INLINE const WChar* cbegin() const { return m_chars.cbegin(); }
		GRIZZLY_NO_DISCARD GRIZZLY_ALWAYS_INLINE const WChar* cend() const { return m_chars.cend(); }

		GRIZZLY_ALWAYS_INLINE WChar operator[](usize index) const { return m_chars[index]; }

		bool operator==(const WStringView& rhs) const;
		GRIZZLY_ALWAYS_INLINE bool operator!=(const WStringView& rhs) const { return !(*this == rhs); }

	private:
		Slice<WChar const> m_chars;
	};
} // namespace Grizzly::Core
