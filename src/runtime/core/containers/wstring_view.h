/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include "core/containers/string_view.h"

namespace op::core {
	using WChar = wchar_t;

	class WStringView {
	public:
		OP_ALWAYS_INLINE constexpr WStringView() : m_chars{} {}
		OP_ALWAYS_INLINE WStringView(const Slice<WChar const>& bytes) : m_chars{ bytes } {}
		OP_ALWAYS_INLINE constexpr WStringView(const WChar* ptr) : m_chars{ ptr, core::constexpr_strlen(ptr) } {}

		OP_NO_DISCARD OP_ALWAYS_INLINE usize len() const { return m_chars.len(); }
		OP_NO_DISCARD OP_ALWAYS_INLINE const WChar* operator*() const { return &m_chars[0]; }

		OP_NO_DISCARD OP_ALWAYS_INLINE const WChar* begin() const { return m_chars.cbegin(); }
		OP_NO_DISCARD OP_ALWAYS_INLINE const WChar* end() const { return m_chars.cend(); }
		OP_NO_DISCARD OP_ALWAYS_INLINE const WChar* cbegin() const { return m_chars.cbegin(); }
		OP_NO_DISCARD OP_ALWAYS_INLINE const WChar* cend() const { return m_chars.cend(); }

		OP_ALWAYS_INLINE WChar operator[](usize index) const { return m_chars[index]; }

		bool operator==(const WStringView& rhs) const;
		OP_ALWAYS_INLINE bool operator!=(const WStringView& rhs) const { return !(*this == rhs); }

	private:
		Slice<WChar const> m_chars;
	};
} // namespace op::core