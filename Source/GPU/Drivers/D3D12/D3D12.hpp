/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Debug/Assertions.hpp>
#include <Core/Windows.hpp>

FORGE_MSVC_DISABLE_WARNINGS_PUSH;
FORGE_MSVC_DISABLE_WARNING(4365); // 'conversion': conversion from 'int' to 'unsigned int', signed/unsigned mismatch

// Windows Runtime Library. Needed for Microsoft::WRL::ComPtr<> template class.
#include <wrl.h>

#include <d3d12.h>
#include <d3d12sdklayers.h>
#include <d3dcompiler.h>
#include <dxgi1_6.h>

FORGE_MSVC_DISABLE_WARNINGS_POP;

namespace Forge::GPU {
	using Microsoft::WRL::ComPtr;

	// From DXSampleHelper.h
	// Source: https://github.com/Microsoft/DirectX-Graphics-Samples
	FORGE_ALWAYS_INLINE void throw_if_failed(HRESULT hr) {
		if (FAILED(hr)) {
			FORGE_PANIC("D3D12 Crash");
		}
	}
} // namespace Forge::GPU
