/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Core.hpp>
#include <initializer_list>

namespace Grizzly {
	template <typename T>
	using InitializerList = std::initializer_list<T>;
} // namespace Grizzly
