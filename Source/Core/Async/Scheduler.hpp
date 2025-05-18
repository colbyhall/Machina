/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Async/Fiber.hpp>
#include <Core/Async/MPMC.hpp>
#include <Core/Async/Thread.hpp>
#include <Core/Containers/Array.hpp>
#include <Core/Containers/Function.hpp>
#include <Core/Containers/Shared.hpp>
#include <Core/Containers/UniquePtr.hpp>
#include <Core/Time.hpp>

namespace Forge::Core {
	class Scheduler;

	class Task {
	public:
		enum class Status : u8 { NotStarted, InProgress, Complete };
		FORGE_NO_DISCARD virtual Status status() const = 0;
	};

	class Scheduler {
	public:
		explicit Scheduler() = default;

		FORGE_NO_COPY(Scheduler);
		FORGE_NO_MOVE(Scheduler);

		struct InitInfo {
			u32 thread_count;

			u32 fiber_count;
			u32 waiting_count;

			u32 high_priority_count = 256;
			u32 normal_priority_count = 512;
			u32 low_priority_count = 1024;
		};
		void init(InitInfo const& create_info);

		enum class Priority : u8 { Low, Normal, High };
		using Job = Function<void()>;

		void enqueue(Job&& job) const { return enqueue(Priority::Normal, Forge::forward<Function<void()>>(job)); }

		void enqueue(Priority priority, Job&& job) const {
			auto& queue = m_work_queue.get(priority);
			queue.push(Forge::move(job));
		}

		FORGE_NO_DISCARD bool wait_until(Duration const& duration, Task const& task) const;
		FORGE_ALWAYS_INLINE void wait_for(Task const& task) const {
			// Not infinite but 584.9 billion years seems like enough time
			const bool complete = wait_until(Duration{ NumericLimits<u64>::max(), 0 }, task);
			FORGE_UNUSED(complete);
		}

		bool is_running() const;

	private:
		struct ThreadController {
			Array<Arc<Thread>> threads;
			Atomic<u32> ready_count{ 0 };
		};
		struct WaitingTask {
			struct Inner {
				const Task& task;
				u32 fiber;
				Option<Thread::Id> thread;
			};
			enum class State : u8 {
				Vacant,
				Updating,
				Filled,
			};
			Atomic<State> state{ State::Vacant };
			Option<Inner> inner;
		};

		struct TaskTracker {
			UniquePtr<WaitingTask[]> waiting_task;
			MPMC<u32> vacant_waiting_task;
		};

		struct FiberController {
			Array<Arc<Fiber>> fibers;
			MPMC<u32> dormant_fibers;
		};

		struct WorkQueue {
			MPMC<Job> high_priority;
			MPMC<Job> normal_priority;
			MPMC<Job> low_priority;

			FORGE_ALWAYS_INLINE MPMC<Job> const& get(Priority priority) const {
				switch (priority) {
				case Priority::Low:
					return low_priority;
					break;
				case Priority::Normal:
					return normal_priority;
					break;
				case Priority::High:
					return high_priority;
					break;
				default:
					FORGE_UNIMPLEMENTED;
					break;
				}
			}
		};

		void worker_main(u32 fiber_index) const;

		ThreadController m_thread_controller;
		FiberController m_fiber_controller;
		TaskTracker m_task_tracker;
		WorkQueue m_work_queue;
	};
} // namespace Forge::Core
