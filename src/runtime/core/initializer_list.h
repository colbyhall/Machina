/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include "core/core.h"
#include <initializer_list>

namespace op {
	template <typename T>
	using InitializerList = std::initializer_list<T>;
} // namespace op