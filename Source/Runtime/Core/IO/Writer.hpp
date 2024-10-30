/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Containers/Slice.hpp>

namespace Grizzly::Core {
	class Writer {
	public:
		virtual ~Writer() = default;
		virtual usize write(Slice<u8 const> bytes) = 0;
	};
} // namespace Grizzly::Core
