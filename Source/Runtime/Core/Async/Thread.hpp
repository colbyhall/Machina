/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Containers/Function.hpp>
#include <Core/Containers/Shared.hpp>
#include <Core/Containers/StringView.hpp>

namespace Grizzly::Core {
	class Thread {
	public:
		static AtomicShared<Thread> spawn(Function<void()> f);

		using Id = u64;

		virtual void join() = 0;
		virtual void detach() = 0;

		virtual ~Thread() {}
	};
} // namespace Grizzly::Core