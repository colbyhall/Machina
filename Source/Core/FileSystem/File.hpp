/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Core.hpp>
#include <Core/Primitives.hpp>
#include <Core/TypeTraits.hpp>

namespace Mach::Core {
	enum class OpenFlags : u8 {
		Read = (1 << 0),
		Write = (1 << 1),
		Create = (1 << 2),
	};
	MACH_ENUM_CLASS_BITFIELD(OpenFlags);
} // namespace Mach::Core

#if MACH_OS == MACH_OS_WINDOWS

	#include <Core/FileSystem/Win32/File.hpp>

namespace Mach::Core {
	using OpenFlags = OpenFlags;
	using File = Win32File;
} // namespace Mach::Core

#else

	#include <Core/FileSystem/Posix/File.hpp>

namespace Mach::Core {
	using OpenFlags = OpenFlags;
	using File = PosixFile;
} // namespace Mach::Core

#endif
