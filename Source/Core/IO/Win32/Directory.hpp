/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Containers/Function.hpp>
#include <Core/Containers/String.hpp>

namespace Forge::Core {
	class Win32Directory {
	public:
		static Option<Win32Directory> open(const StringView& path);
		static Win32Directory cwd();

		Win32Directory(const Win32Directory&) = delete;
		Win32Directory& operator=(const Win32Directory&) = delete;
		Win32Directory(Win32Directory&& move) = default;
		Win32Directory& operator=(Win32Directory&& move) = default;

		void for_each(FunctionRef<bool()> f);

	private:
		explicit Win32Directory(String&& path) : m_path(Forge::move(path)) {}
		String m_path;
	};
} // namespace Forge::Core
