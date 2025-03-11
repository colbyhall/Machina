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
#include <Core/Time.hpp>

namespace Grizzly::Core {
	class Scheduler;

	class Task : public ArcFromThis<Task> {
	public:
		explicit Task(Scheduler const& owner);

		enum class Status : u8 { NotStarted, InProgress, Complete };
		GRIZZLY_NO_DISCARD virtual Status status() const = 0;

		virtual ~Task() {}

	private:
		Arc<Scheduler> m_owner;
	};

	class TaskList final : public Task {
	public:
		class Builder {
		public:
			explicit Builder(Scheduler const& owner) : m_owner(owner) {}

			Builder& add(Task const& task);

			Arc<TaskList> finish();

		private:
			Scheduler const& m_owner;
			Array<Arc<Task>> m_tasks;
		};

		Status status() const;
		GRIZZLY_NO_DISCARD GRIZZLY_ALWAYS_INLINE Slice<Arc<Task> const> tasks() const {
			return m_tasks.as_const_slice();
		}

	private:
		explicit TaskList(Scheduler const& owner, Array<Arc<Task>>&& tasks)
			: Task(owner)
			, m_tasks(Grizzly::move(tasks)) {}
		friend class Builder;

		Array<Arc<Task>> m_tasks;
	};

	template <typename T>
	class Future final : public Task {
	public:
		explicit Future(Scheduler const& owner) : Task(owner) {}

		Status status() const final { return m_status.load(); }

	private:
		GRIZZLY_NO_DISCARD bool start() const {
			return m_status.compare_exchange_weak(Status::NotStarted, Status::InProgress).is_set();
		}
		void finish(T&& t) const {
			auto* mut_self = const_cast<Future<T>*>(this);
			mut_self->m_value = Grizzly::forward<T>(t);
			const bool updated = m_status.compare_exchange_weak(Status::InProgress, Status::Complete).is_set();
			GRIZZLY_ASSERT(updated);
		}

		friend class Scheduler;
		Atomic<Status> m_status{ Status::NotStarted };
		Option<T> m_value;
	};

	template <>
	class Future<void> final : public Task {
	public:
		explicit Future(Scheduler const& owner) : Task(owner) {}
		Status status() const final { return m_status.load(); }

	private:
		GRIZZLY_NO_DISCARD bool start() const {
			return m_status.compare_exchange_weak(Status::NotStarted, Status::InProgress).is_set();
		}
		void finish() const {
			const bool updated = m_status.compare_exchange_weak(Status::InProgress, Status::Complete).is_set();
			GRIZZLY_ASSERT(updated);
		}

		friend class Scheduler;
		Atomic<Status> m_status{ Status::NotStarted };
	};

	class Scheduler final : public ArcFromThis<Scheduler> {
	public:
		struct CreateInfo {
			u32 thread_count;

			u32 fiber_count;
			u32 waiting_count;

			u32 high_priority_count = 256;
			u32 normal_priority_count = 512;
			u32 low_priority_count = 1024;
		};
		static Arc<Scheduler> create(CreateInfo const& create_info);

		enum class Priority : u8 { Low, Normal, High };
		using Job = Function<void()>;

		Arc<Future<void>> schedule(Function<void()>&& f) const {
			return schedule(Priority::Normal, Grizzly::forward<Function<void()>>(f));
		}

		template <typename T>
		GRIZZLY_NO_DISCARD Arc<Future<T>> schedule(Priority priority, Function<T()>&& f) const {
			auto future = Arc<Future<T>>::create(*this);
			auto& queue = m_work_queue.get(priority);
			if constexpr (Core::is_same<T, void>) {
				Function<void()> job = [f = Grizzly::move(f), future = future]() {
					const bool successful = future->start();
					GRIZZLY_ASSERT(successful);
					f();
					future->finish();
				};
				queue.push(Grizzly::move(job));
			} else {
				const auto started = future->start();
				GRIZZLY_ASSERT(started);

				Function<void()> job = [f = Grizzly::move(f), future = future]() {
					auto result = f();
					future->finish(Grizzly::move(result));
				};
				queue.push(Grizzly::move(job));
			}
			return future;
		}

		GRIZZLY_NO_DISCARD bool wait_until(Duration const& duration, Task const& task) const;
		GRIZZLY_ALWAYS_INLINE void wait_for(Task const& task) const {
			// Not infinite but 584.9 billion years seems like enough time
			const bool complete = wait_until(Duration{ NumericLimits<u64>::max(), 0 }, task);
			GRIZZLY_UNUSED(complete);
		}

		bool is_running() const;

	private:
		struct ThreadController {
			Array<Arc<Thread>> threads;
			Atomic<u32> ready_count{ 0 };
		};
		struct WaitingTask {
			struct Inner {
				Arc<Task> task;
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
			Unique<WaitingTask[]> waiting_task;
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

			GRIZZLY_ALWAYS_INLINE MPMC<Job> const& get(Priority priority) const {
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
					GRIZZLY_UNIMPLEMENTED;
					break;
				}
			}
		};

		void worker_main(u32 fiber_index) const;

		ThreadController m_thread_controller;
		FiberController m_fiber_controller;
		TaskTracker m_task_tracker;
		WorkQueue m_work_queue;

		Scheduler(
			ThreadController&& thread_controller,
			FiberController&& fiber_controller,
			TaskTracker&& task_tracker,
			WorkQueue&& work_queue)
			: m_thread_controller(Grizzly::move(thread_controller))
			, m_fiber_controller(Grizzly::move(fiber_controller))
			, m_task_tracker(Grizzly::move(task_tracker))
			, m_work_queue(Grizzly::move(work_queue)) {}
	};
} // namespace Grizzly::Core
