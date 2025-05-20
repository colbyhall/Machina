/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Containers/StringView.hpp>
#include <Core/IO/Reader.hpp>
#include <Core/IO/Writer.hpp>

namespace Forge::Core {
	enum class OpenFlags : u8;

	class PosixFile final : public Reader, public Writer {
	public:
		static PosixFile stdin;
		static PosixFile stdout;
		static PosixFile stderr;

		FORGE_NO_DISCARD static Option<PosixFile> open(const StringView& path, OpenFlags flags);

		PosixFile(const PosixFile&) = delete;
		PosixFile& operator=(const PosixFile&) = delete;
		PosixFile(PosixFile&& move) : m_fd(move.m_fd) { move.m_fd = -1; }
		PosixFile& operator=(PosixFile&& move);
		~PosixFile();

		FORGE_NO_DISCARD usize read(Slice<u8> bytes) final;
		usize write(Slice<u8 const> bytes) final;

	private:
		explicit PosixFile(int fd) : m_fd(fd) {}
		int m_fd; // File descriptor
	};
} // namespace Forge::Core
