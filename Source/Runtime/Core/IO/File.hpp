/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Core.hpp>
#include <Core/Primitives.hpp>
#include <Core/TypeTraits.hpp>

namespace Forge::Core {
	enum class OpenFlags : u8 {
		Read = (1 << 0),
		Write = (1 << 1),
		Create = (1 << 2),
	};
	FORGE_ENUM_CLASS_BITFIELD(OpenFlags);
} // namespace Forge::Core

#if FORGE_PLATFORM == FORGE_PLATFORM_WINDOWS

	#include <Core/IO/Win32/File.hpp>

namespace Forge::Core {
	using OpenFlags = OpenFlags;
	using File = Win32File;
} // namespace Forge::Core

#else

	#include <Core/IO/Posix/File.hpp>

namespace Forge::Core {
	using OpenFlags = OpenFlags;
	using File = PosixFile;
} // namespace Forge::Core

#endif
