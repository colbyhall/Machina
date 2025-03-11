/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Containers/Function.hpp>
#include <Core/Containers/Shared.hpp>
#include <Core/Containers/StringView.hpp>

namespace Grizzly::Core {
	class Thread : public ArcFromThis<Thread> {
	public:
		using Function = Function<void()>;
		struct SpawnInfo {
			Option<StringView> name = nullopt;
			Option<usize> stack_size = nullopt;
			bool start_suspended = false;
		};
		static Arc<Thread> spawn(Function&& f, const SpawnInfo& info);
		static Arc<Thread> spawn(Function&& f);
		static Thread const& current();

		using Id = u64;

		virtual void join() = 0;
		virtual void detach() = 0;
		virtual Id id() const = 0;

		virtual ~Thread() {}
	};
} // namespace Grizzly::Core
