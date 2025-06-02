/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <DXC/inc/d3d12shader.h>
#include <DXC/inc/dxcapi.h>
#include <GPU/Drivers/D3D12/D3D12.hpp>

namespace Mach::DXC {
	class Compiler {
	public:
		explicit Compiler();

		MACH_NO_COPY(Compiler);

	private:
	};
} // namespace Mach::DXC
