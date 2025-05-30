/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Containers/StringView.hpp>

namespace Mach::Core {
	using WChar = wchar_t;

	class WStringView {
	public:
		MACH_ALWAYS_INLINE constexpr WStringView() : m_chars{} {}
		MACH_ALWAYS_INLINE WStringView(const Slice<WChar const>& bytes) : m_chars{ bytes } {}
		MACH_ALWAYS_INLINE constexpr WStringView(const WChar* ptr) : m_chars{ ptr, Core::strlen(ptr) } {}

		MACH_NO_DISCARD MACH_ALWAYS_INLINE usize len() const { return m_chars.len(); }
		MACH_NO_DISCARD MACH_ALWAYS_INLINE const WChar* operator*() const { return &m_chars[0]; }

		MACH_NO_DISCARD MACH_ALWAYS_INLINE const WChar* begin() const { return m_chars.cbegin(); }
		MACH_NO_DISCARD MACH_ALWAYS_INLINE const WChar* end() const { return m_chars.cend(); }
		MACH_NO_DISCARD MACH_ALWAYS_INLINE const WChar* cbegin() const { return m_chars.cbegin(); }
		MACH_NO_DISCARD MACH_ALWAYS_INLINE const WChar* cend() const { return m_chars.cend(); }

		MACH_ALWAYS_INLINE WChar operator[](usize index) const { return m_chars[index]; }

		bool operator==(const WStringView& rhs) const;
		MACH_ALWAYS_INLINE bool operator!=(const WStringView& rhs) const { return !(*this == rhs); }

	private:
		Slice<WChar const> m_chars;
	};
} // namespace Mach::Core
