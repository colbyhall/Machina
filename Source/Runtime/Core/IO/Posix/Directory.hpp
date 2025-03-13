/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Containers/Function.hpp>
#include <Core/Containers/String.hpp>

namespace Forge::Core {
	class PosixDirectory {
	public:
		static Option<PosixDirectory> open(const StringView& path);
		static PosixDirectory cwd();

		PosixDirectory(const PosixDirectory&) = delete;
		PosixDirectory& operator=(const PosixDirectory&) = delete;
		PosixDirectory(PosixDirectory&& move) = default;
		PosixDirectory& operator=(PosixDirectory&& move) = default;

		void for_each(FunctionRef<bool()> f);

	private:
		explicit PosixDirectory(String&& path) : m_path(Forge::move(path)) {}
		String m_path;
	};

} // namespace Forge::Core
