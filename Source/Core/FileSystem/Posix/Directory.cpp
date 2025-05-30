/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/FileSystem/Posix/Directory.hpp>

#include <Core/Debug/Assertions.hpp>
#include <dirent.h>

namespace Mach::Core {
	Option<PosixDirectory> PosixDirectory::open(const StringView& path) { return PosixDirectory{ String::from(path) }; }

	void PosixDirectory::for_each(FunctionRef<bool()> f) {
		DIR* const dir = ::opendir((const char*)*m_path);
		MACH_ASSERT(dir != nullptr);

		dirent* entry = nullptr;
		while ((entry = ::readdir(dir)) != nullptr) {
			if (!f()) {
				break;
			}
		}

		::closedir(dir);
	}
} // namespace Mach::Core
