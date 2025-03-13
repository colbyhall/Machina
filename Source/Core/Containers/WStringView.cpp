/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Containers/WStringView.hpp>
#include <Core/Debug/Test.hpp>

namespace Forge::Core {
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
} // namespace Forge::Core

#if FORGE_ENABLE_TEST
FORGE_TEST_SUITE("Containers") {
	using namespace Forge::Core;

	FORGE_TEST_CASE("WStringView") {
		WStringView view = L"Hello World";
		FORGE_CHECK(view.len() == 11);
	}
}
#endif // FORGE_ENABLE_TEST
