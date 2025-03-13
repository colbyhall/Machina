/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Containers/Array.hpp>
#include <Core/Containers/String.hpp>
#include <Core/Primitives.hpp>

namespace Forge::Core::StackTrace {
	using Frame = String;
	Array<Frame> capture();
} // namespace Forge::Core::StackTrace
