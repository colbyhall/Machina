/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/IO/File.hpp>
#include <fcntl.h>
#include <unistd.h>

namespace Forge::Core {
	PosixFile PosixFile::stdin{ 0 };
	PosixFile PosixFile::stdout{ 1 };
	PosixFile PosixFile::stderr{ 2 };

	Option<PosixFile> PosixFile::open(const StringView& path, OpenFlags flags) {
		int open_flags = 0;
		if ((flags & OpenFlags::Read) == OpenFlags::Read) {
			open_flags |= O_RDONLY;
		}
		if ((flags & OpenFlags::Write) == OpenFlags::Write) {
			open_flags |= O_WRONLY;
		}
		int fd = ::open((const char*)*path, open_flags, 0644);
		if (fd == -1) {
			return nullopt;
		}
		return PosixFile(fd);
	}

	PosixFile& PosixFile::operator=(PosixFile&& move) {
		auto to_destroy = Forge::move(*this);
		FORGE_UNUSED(to_destroy);

		m_fd = move.m_fd;
		move.m_fd = -1;

		return *this;
	}

	usize PosixFile::read(Slice<u8> bytes) {
		FORGE_ASSERT(m_fd != -1);
		return static_cast<usize>(::read(m_fd, bytes.begin(), bytes.len()));
	}

	usize PosixFile::write(Slice<u8 const> bytes) {
		FORGE_ASSERT(m_fd != -1);
		return static_cast<usize>(::write(m_fd, bytes.begin(), bytes.len()));
	}

	PosixFile::~PosixFile() {
		// Only close the file descriptor if it's greater than 2 (stdin, stdout, stderr). Invalid is also -1.
		if (m_fd > 2) {
			::close(m_fd);
			m_fd = -1;
		}
	}
} // namespace Forge::Core
