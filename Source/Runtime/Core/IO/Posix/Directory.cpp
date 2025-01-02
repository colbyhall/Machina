/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Debug/Assertions.hpp>
#include <Core/IO/Posix/Directory.hpp>
#include <dirent.h>

namespace Grizzly::Core {
	Option<PosixDirectory> PosixDirectory::open(const StringView& path) { return PosixDirectory{ String::from(path) }; }

	void PosixDirectory::for_each(FunctionRef<bool()> f) {
		DIR* const dir = ::opendir((const char*)*m_path);
		GRIZZLY_ASSERT(dir != nullptr);

		dirent* entry = nullptr;
		while ((entry = ::readdir(dir)) != nullptr) {
			if (!f()) {
				break;
			}
		}

		::closedir(dir);
	}
} // namespace Grizzly::Core
