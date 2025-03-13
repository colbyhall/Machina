/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Core.hpp>

#if FORGE_PLATFORM == FORGE_PLATFORM_WINDOWS

	#include <Core/IO/Win32/Directory.hpp>

namespace Forge::Core {
	using Directory = Win32Directory;
} // namespace Forge::Core

#else

	#include <Core/IO/Posix/Directory.hpp>

namespace Forge::Core {
	using Directory = PosixDirectory;
} // namespace Forge::Core

#endif
