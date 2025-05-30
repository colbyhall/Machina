/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Containers/Function.hpp>
#include <Core/Containers/String.hpp>

namespace Mach::Core {
	class Win32Directory {
	public:
		static Option<Win32Directory> open(const StringView& path);
		static Win32Directory cwd();

		MACH_NO_COPY(Win32Directory);
		Win32Directory(Win32Directory&& move) = default;
		Win32Directory& operator=(Win32Directory&& move) = default;

		void for_each(FunctionRef<bool()> f);

	private:
		explicit Win32Directory(String&& path) : m_path(Mach::move(path)) {}
		String m_path;
	};
} // namespace Mach::Core
