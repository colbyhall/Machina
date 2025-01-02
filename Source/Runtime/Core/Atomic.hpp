/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Containers/Option.hpp>
#include <atomic>

namespace Grizzly::Core {
	enum class Order : u8 { Relaxed, Release, Acquire, AcqRel, SeqCst };

	template <typename T>
		requires is_trivially_copyable<T>
	class Atomic {
	public:
		Atomic() noexcept = default;
		constexpr explicit Atomic(T desired) noexcept : m_atomic(desired) {}
		GRIZZLY_ALWAYS_INLINE Atomic(const Atomic& rhs) noexcept
			: m_atomic(rhs.m_atomic.load(std::memory_order_relaxed)) {}
		GRIZZLY_ALWAYS_INLINE Atomic& operator=(const Atomic& rhs) noexcept {
			const auto order = std::memory_order_relaxed;
			m_atomic.store(rhs.m_atomic.load(order), order);
			return *this;
		}
		GRIZZLY_ALWAYS_INLINE Atomic(Atomic&& rhs) noexcept : m_atomic(rhs.m_atomic.load(std::memory_order_relaxed)) {}
		GRIZZLY_ALWAYS_INLINE Atomic& operator=(Atomic&& rhs) noexcept {
			const auto order = std::memory_order_relaxed;
			m_atomic.store(rhs.m_atomic.load(order), order);
			return *this;
		}

		GRIZZLY_ALWAYS_INLINE void store(T desired, Order order = Order::SeqCst) const noexcept {
			m_atomic.store(desired, to_std(order));
		}
		GRIZZLY_NO_DISCARD GRIZZLY_ALWAYS_INLINE T load(Order order = Order::SeqCst) const noexcept {
			return m_atomic.load(to_std(order));
		}
		GRIZZLY_NO_DISCARD GRIZZLY_ALWAYS_INLINE T exchange(T desired, Order order = Order::SeqCst) const noexcept {
			return m_atomic.exchange(desired, to_std(order));
		}

		GRIZZLY_NO_DISCARD GRIZZLY_ALWAYS_INLINE Option<T>
		compare_exchange_weak(T expected, T desired, Order order = Order::SeqCst) const noexcept {
			T expected_copy = expected;
			if (m_atomic.compare_exchange_weak(expected_copy, desired, to_std(order))) {
				return expected_copy;
			}
			return nullopt;
		}

		GRIZZLY_NO_DISCARD GRIZZLY_ALWAYS_INLINE Option<T>
		compare_exchange_strong(T expected, T desired, Order order = Order::SeqCst) const noexcept {
			T expected_copy = expected;
			if (m_atomic.compare_exchange_strong(expected_copy, desired, to_std(order))) {
				return expected_copy;
			}
			return nullopt;
		}

		GRIZZLY_NO_DISCARD GRIZZLY_ALWAYS_INLINE T fetch_add(T arg, Order order = Order::SeqCst) const noexcept {
			return m_atomic.fetch_add(arg, to_std(order));
		}
		GRIZZLY_NO_DISCARD GRIZZLY_ALWAYS_INLINE T fetch_sub(T arg, Order order = Order::SeqCst) const noexcept {
			return m_atomic.fetch_sub(arg, to_std(order));
		}
		GRIZZLY_NO_DISCARD GRIZZLY_ALWAYS_INLINE T fetch_and(T arg, Order order = Order::SeqCst) const noexcept {
			return m_atomic.fetch_and(arg, to_std(order));
		}
		GRIZZLY_NO_DISCARD GRIZZLY_ALWAYS_INLINE T fetch_or(T arg, Order order = Order::SeqCst) const noexcept {
			return m_atomic.fetch_or(arg, to_std(order));
		}
		GRIZZLY_NO_DISCARD GRIZZLY_ALWAYS_INLINE T fetch_xor(T arg, Order order = Order::SeqCst) const noexcept {
			return m_atomic.fetch_xor(arg, to_std(order));
		}

	private:
		GRIZZLY_ALWAYS_INLINE std::memory_order to_std(Order order) const {
			static const std::memory_order convert[] = { std::memory_order_relaxed,
														 std::memory_order_release,
														 std::memory_order_acquire,
														 std::memory_order_acq_rel,
														 std::memory_order_seq_cst };
			return convert[(u8)order];
		}

		mutable std::atomic<T> m_atomic;
	};

} // namespace Grizzly::Core
