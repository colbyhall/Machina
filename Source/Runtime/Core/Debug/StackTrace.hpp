/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Containers/Array.hpp>
#include <Core/Containers/String.hpp>
#include <Core/Primitives.hpp>

namespace Grizzly::Core {
	using StackFrame = String;
	Array<StackFrame> capture_stack_trace();
} // namespace Grizzly::Core
