/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Core.hpp>
#include <Core/Primitives.hpp>
#include <Core/TypeTraits.hpp>

namespace Grizzly::Core {
	enum class OpenFlags : u8 {
		Read = (1 << 0),
		Write = (1 << 1),
		Create = (1 << 2),
	};
	GRIZZLY_ENUM_CLASS_BITFIELD(OpenFlags);
} // namespace Grizzly::Core

#if GRIZZLY_PLATFORM == GRIZZLY_PLATFORM_WINDOWS

	#include <Core/IO/Win32/File.hpp>

namespace Grizzly::Core {
	using OpenFlags = OpenFlags;
	using File = Win32File;
} // namespace Grizzly::Core

#else

	#include <Core/IO/Posix/File.hpp>

namespace Grizzly::Core {
	using OpenFlags = OpenFlags;
	using File = PosixFile;
} // namespace Grizzly::Core

#endif
