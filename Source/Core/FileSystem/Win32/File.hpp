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

	class Win32File final : public Reader, public Writer {
	public:
		static Win32File stdin;
		static Win32File stdout;
		static Win32File stderr;

		FORGE_NO_DISCARD static Option<Win32File> open(const StringView& path, OpenFlags flags);

		FORGE_NO_COPY(Win32File);
		Win32File(Win32File&& move) : m_handle(move.m_handle) { move.m_handle = nullptr; }
		Win32File& operator=(Win32File&& move);
		~Win32File();

		// Reader interface
		FORGE_NO_DISCARD usize read(Slice<u8> bytes) final;
		// ~Reader interface

		// Writer interface
		usize write(Slice<u8 const> bytes) final;
		// ~Writer interface

	private:
		explicit Win32File(void* handle, OpenFlags flags) : m_handle(handle), m_flags(flags) {}

		void* m_handle;
		OpenFlags m_flags;
		usize m_cursor = 0;
	};
} // namespace Forge::Core
