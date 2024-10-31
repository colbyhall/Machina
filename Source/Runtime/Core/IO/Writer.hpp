/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Containers/StringView.hpp>

namespace Grizzly::Core {
	class Writer {
	public:
		virtual ~Writer() = default;
		virtual usize write(Slice<u8 const> bytes) = 0;
		usize write(StringView string) { return write(Slice<u8 const>((const u8*)*string, string.len())); }
	};

	class NullWriter final : public Writer {
	public:
		usize write(Slice<u8 const> bytes) final { return bytes.len(); }
	};
} // namespace Grizzly::Core
