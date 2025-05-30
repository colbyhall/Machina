/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Async/Scheduler.hpp>
#include <Core/Debug/Log.hpp>
#include <Core/Math/Matrix4.hpp>
#include <Core/Math/Vector3.hpp>
#include <Core/Memory.hpp>
#include <GUI/Application.hpp>

#if FORGE_OS == FORGE_OS_MACOS
	#include <GPU/Drivers/Metal/Device.hpp>
#elif FORGE_OS == FORGE_OS_WINDOWS
	#include <GPU/Drivers/D3D12/Device.hpp>
#endif

namespace Forge {
	int main() {
		Core::Scheduler scheduler;
		scheduler.init({
			.thread_count = 10,
			.fiber_count = 512,
			.waiting_count = 1024,
		});

#if FORGE_OS == FORGE_OS_MACOS
		const auto device = GPU::create_metal_device();
#elif FORGE_OS == FORGE_OS_WINDOWS
		const auto device = GPU::D3D12Device::create();
#endif

		auto app = GUI::Application(scheduler, *device);
		return app.run([&](auto& frame) {
			frame.window(u8"Hello World"_sv, [&](auto& ui) {
				if (ui.button(u8"Hello World"_sv)) {
					dbgln(u8"Hello World"_sv);
				}
			});
		});
	}
} // namespace Forge
