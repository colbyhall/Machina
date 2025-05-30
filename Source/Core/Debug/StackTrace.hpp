/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Containers/Array.hpp>
#include <Core/Containers/String.hpp>
#include <Core/Primitives.hpp>

namespace Mach::Core::StackTrace {
	using Frame = String;
	Array<Frame> capture();
} // namespace Mach::Core::StackTrace
