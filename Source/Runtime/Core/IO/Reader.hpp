/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Containers/Slice.hpp>

namespace Grizzly::Core {
	class Reader {
	public:
		virtual ~Reader() {}
		virtual usize read(Slice<u8> bytes) = 0;
	};
} // namespace Grizzly::Core
