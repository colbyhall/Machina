/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Containers/Slice.hpp>

namespace Forge::Core {
	class Reader {
	public:
		virtual ~Reader() {}
		virtual usize read(Slice<u8> bytes) = 0;
	};
} // namespace Forge::Core
