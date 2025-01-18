/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Async/TaskManager.hpp>

namespace Grizzly::Core {
	TaskList::Builder& TaskList::Builder::add(Task const& task) {
		m_tasks.push(task.to_shared());
		return *this;
	}

	AtomicShared<TaskList> TaskList::Builder::finish() {
		TaskList result{ Grizzly::move(m_tasks) };
		return AtomicShared<TaskList>::create(Grizzly::move(result));
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

	AtomicShared<TaskManager> TaskManager::create() {
		constexpr u32 num_threads = 10;

		const AtomicShared<TaskManager> task_manager = AtomicShared<TaskManager>::create(
			TaskManager{ MPMCQueue<Job>::create(1024), MPMCQueue<Job>::create(1024), MPMCQueue<Job>::create(1024) });
		task_manager->m_threads.reserve(num_threads);

		task_manager->m_threads.push(Thread::current().to_shared());
		for (u32 i = 0; i < num_threads - 1; i += 1) {
			auto thread = Thread::spawn([task_manager]() {
				bool running = true;
				while (running) {
					const auto state = task_manager->m_state.load();
					switch (state) {
					case State::Starting: {
					} break;
					case State::Running: {
						auto job = task_manager->m_high_priority.pop();
						if (job.is_set()) {
							auto f = job.unwrap();
							f();
							return;
						}

						// Look for finished task

						job = task_manager->m_normal_priority.pop();
						if (job.is_set()) {
							auto f = job.unwrap();
							f();
							return;
						}

						job = task_manager->m_low_priority.pop();
						if (job.is_set()) {
							auto f = job.unwrap();
							f();
							return;
						}
					} break;
					case State::ShuttingDown:
						running = false;
						break;
					}
				}
			});
			task_manager->m_threads.push(Grizzly::move(thread));
		}

		task_manager->m_state.store(State::Running);

		return task_manager;
	}

	bool TaskManager::wait_until(Duration const& duration, Task const& task) const { return false; }
} // namespace Grizzly::Core
