/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Async/Scheduler.hpp>

namespace Grizzly::Core {
	Task::Task(Scheduler const& owner) : m_owner(owner.to_shared()) {}

	TaskList::Builder& TaskList::Builder::add(Task const& task) {
		m_tasks.push(task.to_shared());
		return *this;
	}

	Arc<TaskList> TaskList::Builder::finish() {
		TaskList result{ m_owner, Grizzly::move(m_tasks) };
		return Arc<TaskList>::create(Grizzly::move(result));
	}

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
		const auto scheduler = Arc<Scheduler>::create(Scheduler{
			MPMC<Job>::create(create_info.high_priority_count),
			MPMC<Job>::create(create_info.normal_priority_count),
			MPMC<Job>::create(create_info.low_priority_count),
		});
		auto& mut_scheduler = scheduler.unsafe_get_mut();
		mut_scheduler.m_threads.reserve(create_info.thread_count);

		mut_scheduler.m_threads.push(Thread::current().to_shared());

		for (u32 i = 0; i < create_info.thread_count - 1; i += 1) {
			auto thread = Thread::spawn([scheduler]() {
				bool running = true;
				while (running) {
					const auto state = scheduler->m_state.load();

					switch (state) {
					case State::Starting: {
					} break;
					case State::Running: {
						auto job = scheduler->m_high_priority.pop();
						if (job.is_set()) {
							auto f = job.unwrap();
							f();
							continue;
						}

						// Look for finished task

						job = scheduler->m_normal_priority.pop();
						if (job.is_set()) {
							auto f = job.unwrap();
							f();
							continue;
						}

						job = scheduler->m_low_priority.pop();
						if (job.is_set()) {
							auto f = job.unwrap();
							f();
							continue;
						}
					} break;
					case State::ShuttingDown:
						running = false;
						break;
					}
				}
			});
			mut_scheduler.m_threads.push(Grizzly::move(thread));
		}

		scheduler->m_state.store(State::Running);

		return scheduler;
	}

	bool Scheduler::wait_until(Duration const& duration, Task const& task) const { return false; }
} // namespace Grizzly::Core
