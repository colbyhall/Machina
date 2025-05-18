/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/IO/File.hpp>

#include <Core/Containers/WString.hpp>
#include <Core/Windows.hpp>

namespace Forge::Core {
	Win32File Win32File::stdin(GetStdHandle(STD_INPUT_HANDLE), OpenFlags::Read);
	Win32File Win32File::stdout(GetStdHandle(STD_OUTPUT_HANDLE), OpenFlags::Write);
	Win32File Win32File::stderr(GetStdHandle(STD_ERROR_HANDLE), OpenFlags::Write);

	Option<Win32File> Win32File::open(const StringView& path, OpenFlags flags) {
		const bool read = (flags & OpenFlags::Read) == OpenFlags::Read;
		const bool write = (flags & OpenFlags::Write) == OpenFlags::Write;
		const bool create = (flags & OpenFlags::Create) == OpenFlags::Create;

		FORGE_ASSERT(read || write);

		DWORD access = 0;
		if (read) access |= GENERIC_READ;
		if (write) access |= GENERIC_WRITE;

		const auto wpath = WString::from(path);

		void* handle = CreateFileW(
			*wpath,
			access,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			nullptr,
			create ? CREATE_ALWAYS : OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			nullptr);

		if (handle == nullptr) {
			// TODO: Error handling
			return nullopt;
		}

		return Win32File(handle, flags);
	}

	Win32File& Win32File::operator=(Win32File&& move) {
		auto to_destroy = Forge::move(*this);
		FORGE_UNUSED(to_destroy);

		m_handle = move.m_handle;
		move.m_handle = nullptr;

		return *this;
	}

	Win32File::~Win32File() {
		if (m_handle) {
			::CloseHandle(m_handle);
			m_handle = nullptr;
		}
	}

	usize Win32File::read(Slice<u8> bytes) {
		FORGE_ASSERT(m_handle != nullptr);
		FORGE_ASSERT((m_flags & OpenFlags::Read) == OpenFlags::Read);

		DWORD amount_read = 0;
		const bool ok = ::ReadFile(m_handle, bytes.begin(), static_cast<DWORD>(bytes.len()), &amount_read, nullptr);
		FORGE_ASSERT(ok);

		m_cursor += amount_read;

		return amount_read;
	}

	usize Win32File::write(Slice<u8 const> bytes) {
		FORGE_ASSERT(m_handle != nullptr);
		FORGE_ASSERT((m_flags & OpenFlags::Write) == OpenFlags::Write);

		DWORD amount_written = 0;
		const bool ok = ::WriteFile(m_handle, bytes.begin(), static_cast<DWORD>(bytes.len()), &amount_written, nullptr);
		FORGE_ASSERT(ok);

		m_cursor += amount_written;

		return amount_written;
	}
} // namespace Forge::Core
