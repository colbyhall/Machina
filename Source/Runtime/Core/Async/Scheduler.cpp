/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Async/Scheduler.hpp>

#include <Core/Debug/Log.hpp>

namespace Forge::Core {
	Task::Task(Scheduler const& owner) : m_owner(owner.to_shared()) {}

	TaskList::Builder& TaskList::Builder::add(Task const& task) {
		m_tasks.push(task.to_shared());
		return *this;
	}

	Arc<TaskList> TaskList::Builder::finish() {
		TaskList result{ m_owner, Forge::move(m_tasks) };
		return Arc<TaskList>::create(Forge::move(result));
	}

	thread_local Option<u32> g_fiber_index = nullopt;

	Task::Status TaskList::status() const {
		u32 complete_count = 0;
		for (auto& task : m_tasks) {
			switch (task->status()) {
			case Status::NotStarted:
				// Early out if we've found a complete and then a not started. If any task has not been started while
				// others have been completed then we know this task is InProgress.
				if (complete_count > 0) {
					return Status::InProgress;
				}
				break;
			case Status::InProgress:
				// Early out if we find a subtask that is InProgress as that means this task is InProgress.
				return Status::InProgress;
			case Status::Complete:
				// This task is only completed if all subtask are completed so keep track of the number completed.
				complete_count += 1;
				break;
			}
		}

		if (complete_count == 0) {
			return Status::NotStarted;
		} else if (complete_count == m_tasks.len()) {
			return Status::Complete;
		} else {
			return Status::InProgress;
		}
	}

	Arc<Scheduler> Scheduler::create(CreateInfo const& create_info) {
		auto thread_controller = ThreadController{};
		auto fiber_controller = FiberController{
			.dormant_fibers = MPMC<u32>::create(create_info.fiber_count),
		};
		auto task_tracker = TaskTracker{
			.waiting_task = Unique<WaitingTask[]>::create(create_info.waiting_count),
			.vacant_waiting_task = MPMC<u32>::create(create_info.waiting_count),
		};
		auto work_queue = WorkQueue{
			.high_priority = MPMC<Job>::create(create_info.high_priority_count),
			.normal_priority = MPMC<Job>::create(create_info.normal_priority_count),
			.low_priority = MPMC<Job>::create(create_info.low_priority_count),
		};

		const auto result = Arc<Scheduler>::create(Scheduler(
			Forge::move(thread_controller),
			Forge::move(fiber_controller),
			Forge::move(task_tracker),
			Forge::move(work_queue)));
		auto& scheduler = result.unsafe_get_mut();

		for (u32 index = 0; index < create_info.waiting_count; index += 1) {
			scheduler.m_task_tracker.vacant_waiting_task.push(index);
		}

		scheduler.m_fiber_controller.fibers.reserve(create_info.fiber_count);
		for (u32 index = 0; index < create_info.thread_count; index += 1) {
			scheduler.m_fiber_controller.fibers.push(Fiber::current().to_shared());
		}
		for (u32 index = create_info.thread_count; index < create_info.fiber_count; index += 1) {
			auto fiber = Fiber::spawn([index, scheduler = result]() { scheduler->worker_main(index); });
			scheduler.m_fiber_controller.fibers.push(Forge::move(fiber));
			scheduler.m_fiber_controller.dormant_fibers.push(index);
		}

		scheduler.m_thread_controller.threads.reserve(create_info.thread_count);
		scheduler.m_thread_controller.threads.push(Thread::current().to_shared());
		for (u32 index = 1; index < create_info.thread_count; index += 1) {
			auto thread = Thread::spawn([index, scheduler = result]() {
				{
					auto& mut_scheduler = scheduler.unsafe_get_mut();
					mut_scheduler.m_fiber_controller.fibers[index] = Fiber::current().to_shared();
					g_fiber_index = index;
					mut_scheduler.m_thread_controller.ready_count.fetch_add(1, Order::AcqRel);
				}
				scheduler->worker_main(index);
			});
			scheduler.m_thread_controller.threads.push(Forge::move(thread));
		}

		g_fiber_index = 0;
		scheduler.m_thread_controller.ready_count.fetch_add(1, Order::AcqRel);

		return result;
	}

	bool Scheduler::wait_until(Duration const& duration, Task const& task) const {
		while (true) {
			for (auto& waiting : m_task_tracker.waiting_task) {
				if (waiting.state.compare_exchange_weak(WaitingTask::State::Vacant, WaitingTask::State::Updating)
						.is_set()) {
					Option<u32> available_fiber = nullopt;
					while (!available_fiber.is_set()) {
						available_fiber = m_fiber_controller.dormant_fibers.pop();
					}

					auto& mut_waiting = const_cast<WaitingTask&>(waiting);
					mut_waiting.inner = WaitingTask::Inner{
						.task = task.to_shared(),
						.fiber = g_fiber_index.unwrap(),
						.thread = nullopt,
					};
					waiting.state.store(WaitingTask::State::Filled);

					g_fiber_index = available_fiber.unwrap();
					m_fiber_controller.fibers[available_fiber.unwrap()]->switch_to();
					return true;
				}
			}
		}
		return true;
	}

	bool Scheduler::is_running() const {
		return m_thread_controller.threads.len() == m_thread_controller.ready_count.load(Order::Acquire);
	}

	void Scheduler::worker_main(u32 fiber_index) const {
		while (!is_running()) {
			// Do nothing as we're initializing the Scheduler
		}

		while (is_running()) {
			auto job = m_work_queue.high_priority.pop();
			if (job.is_set()) {
				auto f = job.unwrap();
				f();
				continue;
			}

			// Look for finished task
			bool resumed_work = false;
			for (u32 index = 0; index < m_task_tracker.waiting_task.len(); index += 1) {
				auto& waiting = const_cast<WaitingTask&>(m_task_tracker.waiting_task[index]);

				const auto ready =
					waiting.state.compare_exchange_weak(WaitingTask::State::Filled, WaitingTask::State::Updating)
						.is_set();
				if (!ready) {
					continue;
				}

				auto& inner = waiting.inner.as_const_ref().unwrap();
				const bool thread_viable = (inner.thread.is_set() && inner.thread.unwrap() == Thread::current().id()) ||
										   !inner.thread.is_set();
				if (thread_viable && inner.task->status() == Task::Status::Complete) {
					const auto inner_fiber_index = inner.fiber;
					waiting.inner = nullopt;
					waiting.state.store(WaitingTask::State::Vacant, Order::Release);
					m_task_tracker.vacant_waiting_task.push(index);
					m_fiber_controller.dormant_fibers.push(fiber_index);
					g_fiber_index = inner_fiber_index;
					m_fiber_controller.fibers[inner_fiber_index]->switch_to();
					resumed_work = true;
					break;
				} else {
					waiting.state.store(WaitingTask::State::Filled, Order::Release);
				}
			}
			if (resumed_work) {
				continue;
			}

			job = m_work_queue.normal_priority.pop();
			if (job.is_set()) {
				auto f = job.unwrap();
				f();
				continue;
			}

			job = m_work_queue.low_priority.pop();
			if (job.is_set()) {
				auto f = job.unwrap();
				f();
				continue;
			}
		}
	}
} // namespace Forge::Core
