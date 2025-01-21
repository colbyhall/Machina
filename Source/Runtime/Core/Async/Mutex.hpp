/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Async/Scheduler.hpp>
#include <Core/Atomic.hpp>

namespace Grizzly::Core {
	template <Movable T>
	class SpinlockMutex {
	public:
		explicit SpinlockMutex(T&& value) : m_value(Grizzly::forward<T>(value)) {}

		class Guard {
		public:
			GRIZZLY_ALWAYS_INLINE explicit operator T*() const { return &value(); }
			GRIZZLY_ALWAYS_INLINE explicit operator T&() const { return value(); }
			GRIZZLY_ALWAYS_INLINE T* operator->() const { return &value(); }
			GRIZZLY_ALWAYS_INLINE T& operator*() const { return value(); }
			~Guard() {
				const auto unused = m_owner.m_start.fetch_add(1);
				GRIZZLY_UNUSED(unused);
			}

		private:
			explicit Guard(SpinlockMutex const& owner) : m_owner(owner) {}
			GRIZZLY_NO_DISCARD GRIZZLY_ALWAYS_INLINE T& value() const { return const_cast<T&>(m_owner.m_value); }

			friend class SpinlockMutex<T>;

			SpinlockMutex const& m_owner;
		};

		GRIZZLY_NO_DISCARD Guard lock() const {
			const auto end = m_end.fetch_add(1);
			while (end != m_start.load()) {
			}
			return Guard(*this);
		}

	private:
		friend class Guard;

		T m_value;
		Atomic<u64> m_start{ 0 };
		Atomic<u64> m_end{ 0 };
	};
} // namespace Grizzly::Core
