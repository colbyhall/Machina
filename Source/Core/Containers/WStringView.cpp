/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Containers/WStringView.hpp>
#include <Core/Debug/Test.hpp>

namespace Mach::Core {
	bool WStringView::operator==(const WStringView& rhs) const {
		if (len() != rhs.len()) {
			return false;
		}

		for (usize i = 0; i < len(); ++i) {
			if (m_chars[i] != rhs.m_chars[i]) {
				return false;
			}
		}

		return true;
	}
} // namespace Mach::Core

#if MACH_ENABLE_TEST
MACH_TEST_SUITE("Containers") {
	using namespace Mach::Core;

	MACH_TEST_CASE("WStringView") {
		WStringView view = L"Hello World";
		MACH_CHECK(view.len() == 11);
	}
}
#endif // MACH_ENABLE_TEST
