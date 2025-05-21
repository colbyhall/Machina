/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Core.hpp>

#if FORGE_OS == FORGE_OS_WINDOWS

	#include <Core/FileSystem/Win32/Directory.hpp>

namespace Forge::Core {
	using Directory = Win32Directory;
} // namespace Forge::Core

#else

	#include <Core/FileSystem/Posix/Directory.hpp>

namespace Forge::Core {
	using Directory = PosixDirectory;
} // namespace Forge::Core

#endif
