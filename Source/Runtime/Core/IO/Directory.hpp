/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Core.hpp>

#if GRIZZLY_PLATFORM == GRIZZLY_PLATFORM_WINDOWS

#include <Core/IO/Win32/Directory.hpp>

namespace Grizzly::Core {
	using Directory = Win32Directory;
} // namespace Grizzly::Core

#else

#include <Core/IO/Posix/Directory.hpp>

namespace Grizzly::Core {
	using Directory = PosixDirectory;
} // namespace Grizzly::Core

#endif
