/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Containers/Function.hpp>
#include <Core/Containers/String.hpp>

namespace Mach::Core {
	class PosixDirectory {
	public:
		static Option<PosixDirectory> open(const StringView& path);
		static PosixDirectory cwd();

		MACH_NO_COPY(PosixDirectory);
		PosixDirectory(PosixDirectory&& move) = default;
		PosixDirectory& operator=(PosixDirectory&& move) = default;

		void for_each(FunctionRef<bool()> f);

	private:
		explicit PosixDirectory(String&& path) : m_path(Mach::move(path)) {}
		String m_path;
	};

} // namespace Mach::Core
