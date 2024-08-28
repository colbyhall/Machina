/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include "core/containers/wstring_view.h"
#include "core/debug/test.h"

namespace op::core {
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
} // namespace op::core

#if OP_ENABLE_TEST
OP_TEST_SUITE("containers") {
	using namespace op::core;

	OP_TEST_CASE("WStringView") {
		WStringView view = L"Hello World";
		OP_CHECK(view.len() == 11);
	}
}
#endif // OP_ENABLE_TEST
