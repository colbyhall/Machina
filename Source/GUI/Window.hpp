/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Containers/StringView.hpp>
#include <Core/Containers/UniquePtr.hpp>
#include <GPU/Swapchain.hpp>
#include <GUI/Math.hpp>

namespace Mach::GUI {
	class Application;

	class Window {
	public:
		struct CreateInfo {
			StringView title;
			Size size;
			const GPU::Device& device;
		};
		static UniquePtr<Window> create(CreateInfo const& create_info);
		virtual ~Window() {}

		virtual bool close() = 0;
		virtual bool show() = 0;
		virtual bool hide() = 0;
		virtual bool maximize() = 0;
		virtual bool minimize() = 0;
		virtual GPU::Swapchain& swapchain() const = 0;
		virtual Bounds viewport() const = 0;
		virtual Point cursor_position() const = 0;
	};
} // namespace Mach::GUI
