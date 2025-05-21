/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/FileSystem/Library.hpp>

namespace Forge::Core {
	class Win32Library final : public Library {
	public:
		explicit Win32Library(void* handle) : m_handle(handle) {}

		FORGE_NO_COPY(Win32Library);
		Win32Library(Win32Library&&);
		Win32Library& operator=(Win32Library&&);

		// Library Interaface
		void* find_symbol_impl(const StringView& name) const final;
		~Win32Library() final;

	private:
		void* m_handle = nullptr;
	};

} // namespace Forge::Core
