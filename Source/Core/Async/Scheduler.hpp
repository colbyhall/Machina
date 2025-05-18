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
#include <Core/Containers/Unique.hpp>
#include <Core/Time.hpp>

namespace Forge::Core {
	class Scheduler;

	class Task : public ArcFromThis<Task> {
	public:
		explicit Task(Scheduler const& owner);

		enum class Status : u8 { NotStarted, InProgress, Complete };
		FORGE_NO_DISCARD virtual Status status() const = 0;

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
		FORGE_NO_DISCARD FORGE_ALWAYS_INLINE Slice<Arc<Task> const> tasks() const { return m_tasks.as_const_slice(); }

	private:
		explicit TaskList(Scheduler const& owner, Array<Arc<Task>>&& tasks)
			: Task(owner)
			, m_tasks(Forge::move(tasks)) {}
		friend class Builder;

		Array<Arc<Task>> m_tasks;
	};

	template <typename T>
	class Future final : public Task {
	public:
		explicit Future(Scheduler const& owner) : Task(owner) {}

		Status status() const final { return m_status.load(); }

	private:
		FORGE_NO_DISCARD bool start() const {
			return m_status.compare_exchange_weak(Status::NotStarted, Status::InProgress).is_set();
		}
		void finish(T&& t) const {
			auto* mut_self = const_cast<Future<T>*>(this);
			mut_self->m_value = Forge::forward<T>(t);
			const bool updated = m_status.compare_exchange_weak(Status::InProgress, Status::Complete).is_set();
			FORGE_ASSERT(updated);
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
		FORGE_NO_DISCARD bool start() const {
			return m_status.compare_exchange_weak(Status::NotStarted, Status::InProgress).is_set();
		}
		void finish() const {
			const bool updated = m_status.compare_exchange_weak(Status::InProgress, Status::Complete).is_set();
			FORGE_ASSERT(updated);
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

		Arc<Future<void>> enqueue(Function<void()>&& f) const {
			return enqueue(Priority::Normal, Forge::forward<Function<void()>>(f));
		}

		template <typename T>
		FORGE_NO_DISCARD Arc<Future<T>> enqueue(Priority priority, Function<T()>&& f) const {
			auto future = Arc<Future<T>>::create(*this);
			auto& queue = m_work_queue.get(priority);
			if constexpr (Core::is_same<T, void>) {
				Function<void()> job = [f = Forge::move(f), future = future]() {
					const bool successful = future->start();
					FORGE_ASSERT(successful);
					f();
					future->finish();
				};
				queue.push(Forge::move(job));
			} else {
				const auto started = future->start();
				FORGE_ASSERT(started);

				Function<void()> job = [f = Forge::move(f), future = future]() {
					auto result = f();
					future->finish(Forge::move(result));
				};
				queue.push(Forge::move(job));
			}
			return future;
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

		Scheduler(
			ThreadController&& thread_controller,
			FiberController&& fiber_controller,
			TaskTracker&& task_tracker,
			WorkQueue&& work_queue)
			: m_thread_controller(Forge::move(thread_controller))
			, m_fiber_controller(Forge::move(fiber_controller))
			, m_task_tracker(Forge::move(task_tracker))
			, m_work_queue(Forge::move(work_queue)) {}
	};
} // namespace Forge::Core
