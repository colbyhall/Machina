/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/FileSystem/Win32/Library.hpp>

#include <Core/Containers/WString.hpp>
#include <Core/Windows.hpp>

namespace Mach::Core {
	Mach::SharedPtr<Library> Library::load(const StringView& path) {
		WString wide_path;
		wide_path.append(path);

		auto handle = ::LoadLibraryW(wide_path.begin());
		if (handle == nullptr) {
			return Mach::SharedPtr<Library>();
		}

		return Mach::SharedPtr<Win32Library>::create(handle);
	}

	void* Win32Library::find_symbol_impl(const StringView& name) const {
		return (void*)::GetProcAddress(static_cast<HMODULE>(m_handle), reinterpret_cast<LPCSTR>(*name));
	}

	Win32Library::Win32Library(Win32Library&& m) : m_handle(m.m_handle) { m.m_handle = nullptr; }

	Win32Library& Win32Library::operator=(Win32Library&& m) {
		this->~Win32Library();
		m_handle = m.m_handle;
		m.m_handle = nullptr;
		return *this;
	}

	Win32Library::~Win32Library() {
		if (m_handle != nullptr) {
			::FreeLibrary(static_cast<HMODULE>(m_handle));
			m_handle = nullptr;
		}
	}
} // namespace Mach::Core
