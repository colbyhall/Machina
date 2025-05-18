/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Async/Scheduler.hpp>

#include <Core/Debug/Log.hpp>

namespace Forge::Core {
	thread_local Option<u32> g_fiber_index = nullopt;

	void Scheduler::init(InitInfo const& create_info) {
		m_fiber_controller.dormant_fibers = MPMC<u32>::create(create_info.fiber_count);
		m_task_tracker.waiting_task = UniquePtr<WaitingTask[]>::create(create_info.waiting_count);
		m_task_tracker.vacant_waiting_task = MPMC<u32>::create(create_info.waiting_count);
		m_work_queue.high_priority = MPMC<Job>::create(create_info.high_priority_count);
		m_work_queue.normal_priority = MPMC<Job>::create(create_info.normal_priority_count);
		m_work_queue.low_priority = MPMC<Job>::create(create_info.low_priority_count);

		for (u32 index = 0; index < create_info.waiting_count; index += 1) {
			m_task_tracker.vacant_waiting_task.push(index);
		}

		m_fiber_controller.fibers.reserve(create_info.fiber_count);
		for (u32 index = 0; index < create_info.thread_count; index += 1) {
			m_fiber_controller.fibers.push(Fiber::current().to_shared());
		}
		for (u32 index = create_info.thread_count; index < create_info.fiber_count; index += 1) {
			auto fiber = Fiber::spawn([index, this]() { worker_main(index); });
			m_fiber_controller.fibers.push(Forge::move(fiber));
			m_fiber_controller.dormant_fibers.push(index);
		}

		m_thread_controller.threads.reserve(create_info.thread_count);
		m_thread_controller.threads.push(Thread::current().to_shared());
		for (u32 index = 1; index < create_info.thread_count; index += 1) {
			auto thread = Thread::spawn([index, this]() {
				{
					m_fiber_controller.fibers[index] = Fiber::current().to_shared();
					g_fiber_index = index;
					m_thread_controller.ready_count.fetch_add(1, Order::AcqRel);
				}
				worker_main(index);
			});
			m_thread_controller.threads.push(Forge::move(thread));
		}

		g_fiber_index = 0;
		m_thread_controller.ready_count.fetch_add(1, Order::AcqRel);
	}

	bool Scheduler::wait_until(Duration const& duration, Task const& task) const {
		FORGE_UNUSED(duration);

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
						.task = task,
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
				if (thread_viable && inner.task.status() == Task::Status::Complete) {
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
