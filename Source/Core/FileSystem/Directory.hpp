/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Core.hpp>

#if MACH_OS == MACH_OS_WINDOWS

	#include <Core/FileSystem/Win32/Directory.hpp>

namespace Mach::Core {
	using Directory = Win32Directory;
} // namespace Mach::Core

#else

	#include <Core/FileSystem/Posix/Directory.hpp>

namespace Mach::Core {
	using Directory = PosixDirectory;
} // namespace Mach::Core

#endif
