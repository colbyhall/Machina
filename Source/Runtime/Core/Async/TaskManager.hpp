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
	class Task : public AtomicSharedFromThis<Task> {
	public:
		enum class Status : u8 { NotStarted, InProgress, Complete };
		GRIZZLY_NO_DISCARD virtual Status status() const = 0;
		virtual ~Task() {}
	};

	class TaskList final : public Task {
	public:
		class Builder {
		public:
			explicit Builder() = default;

			Builder& add(Task const& task);

			AtomicShared<TaskList> finish();

		private:
			Array<AtomicShared<Task>> m_tasks;
		};

		Status status() const;
		GRIZZLY_NO_DISCARD GRIZZLY_ALWAYS_INLINE Slice<AtomicShared<Task> const> tasks() const {
			return m_tasks.as_const_slice();
		}

	private:
		explicit TaskList(Array<AtomicShared<Task>>&& tasks) : m_tasks(Grizzly::move(tasks)) {}
		friend class Builder;

		Array<AtomicShared<Task>> m_tasks;
	};

	template <typename T>
	class Future final : public Task {
	public:
		Status status() const final { return m_status.load(); }

	private:
		GRIZZLY_NO_DISCARD bool start() const {
			return m_status.compare_exchange_weak(Status::NotStarted, Status::InProgress).is_set();
		}
		void finish(T&& t) const {
			m_value = Grizzly::forward<T>(t);
			const bool updated = m_status.compare_exchange_weak(Status::InProgress, Status::Complete).is_set();
			GRIZZLY_ASSERT(updated);
		}

		friend class ThreadExecutor;
		Atomic<Status> m_status{ Status::NotStarted };
		Option<T> m_value;
	};

	template <>
	class Future<void> final : public Task {
	public:
		Status status() const final { return m_status.load(); }

	private:
		GRIZZLY_NO_DISCARD bool start() const {
			return m_status.compare_exchange_weak(Status::NotStarted, Status::InProgress).is_set();
		}
		void finish() const {
			const bool updated = m_status.compare_exchange_weak(Status::InProgress, Status::Complete).is_set();
			GRIZZLY_ASSERT(updated);
		}

		friend class TaskManager;
		Atomic<Status> m_status{ Status::NotStarted };
	};

	class TaskManager final : public AtomicSharedFromThis<TaskManager> {
	public:
		static AtomicShared<TaskManager> create();

		enum class Priority : u8 { Low, Normal, High };
		using Job = Function<void()>;

		template <typename T = void>
		GRIZZLY_NO_DISCARD AtomicShared<Future<T>> schedule(Priority priority, Function<T()>&& f) const {
			if constexpr (Core::is_same<T, void>) {
				auto future = AtomicShared<Future<T>>::create();

				Function<void()> job = [f = Grizzly::move(f), future = future]() {
					const bool successful = future->start();
					GRIZZLY_ASSERT(successful);
					f();
					future->finish();
				};

				MPMCQueue<Job> const* queue = nullptr;
				switch (priority) {
				case Priority::Low:
					queue = &m_low_priority;
					break;
				case Priority::Normal:
					queue = &m_normal_priority;
					break;
				case Priority::High:
					queue = &m_high_priority;
					break;
				default:
					GRIZZLY_UNIMPLEMENTED;
					break;
				}
				GRIZZLY_ASSERT(queue != nullptr);

				queue->push(Grizzly::move(job));

				return future;
			} else {
				const auto future = AtomicShared<Future<T>>::create();

				Function<void()> job = [f = Grizzly::move(f), future = future]() {
					const auto result = f();
					const bool successful = future->mark_complete(Grizzly::move(result));
					GRIZZLY_ASSERT(successful);
				};

				MPMCQueue<Job> const* queue = nullptr;
				switch (priority) {
				case Priority::Low:
					queue = &m_low_priority;
					break;
				case Priority::Normal:
					queue = &m_normal_priority;
					break;
				case Priority::High:
					queue = &m_high_priority;
					break;
				default:
					GRIZZLY_UNIMPLEMENTED;
					break;
				}
				GRIZZLY_ASSERT(queue != nullptr);

				queue->push(Grizzly::move(job));

				return future;
			}
		}

		GRIZZLY_NO_DISCARD bool wait_until(Duration const& duration, Task const& task) const;
		GRIZZLY_ALWAYS_INLINE void wait_for(Task const& task) const {
			// Not infinite but 584.9 billion years seems like enough time
			const bool complete = wait_until(Duration{ NumericLimits<u64>::max(), 0 }, task);
			GRIZZLY_UNUSED(complete);
		}

	private:
		TaskManager(MPMCQueue<Job>&& high_priority, MPMCQueue<Job>&& normal_priority, MPMCQueue<Job>&& low_priority)
			: m_high_priority(Grizzly::move(high_priority))
			, m_normal_priority(Grizzly::move(normal_priority))
			, m_low_priority(Grizzly::move(low_priority)) {}

		enum class State : u8 {
			Starting,
			Running,
			ShuttingDown,
		};
		Atomic<State> m_state{ State::Starting };

		Array<AtomicShared<Thread>> m_threads;
		Array<AtomicShared<Fiber>> m_fibers;

		MPMCQueue<Job> m_high_priority;
		MPMCQueue<Job> m_normal_priority;
		MPMCQueue<Job> m_low_priority;
	};
} // namespace Grizzly::Core
