/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Atomic.hpp>
#include <Core/Core.hpp>
#include <Core/Memory.hpp>
#include <Core/Primitives.hpp>

namespace Mach::Core {
	// Source: Dmitry Vyukov's MPMC
	// http://www.1024cores.net/home/lock-free-algorithms/queues/bounded-mpmc-queue
	template <Movable T>
	class MPMC {
		struct Cell {
			Atomic<usize> sequence;
			Option<T> data;
		};

	public:
		explicit MPMC() : m_buffer(nullptr), m_buffer_mask(0) {}

		static MPMC create(u32 capacity) {
			// Verify that size is a power of 2
			MACH_ASSERT(capacity >= 2 && (capacity & capacity - 1) == 0);

			auto ptr = Memory::alloc(Memory::Layout::array<Cell>(capacity));
			Cell* const buffer = static_cast<Cell*>(*ptr);
			for (u32 i = 0; i < capacity; ++i) {
				Cell* const cell = buffer + i;
				Memory::emplace<Cell>(cell, Cell{ .sequence{ i }, .data = nullopt });
			}

			return MPMC(buffer, capacity);
		}
		MPMC(const MPMC&) = delete;
		MPMC& operator=(const MPMC&) = delete;

		MACH_ALWAYS_INLINE MPMC(MPMC<T>&& move) noexcept
			: m_buffer(move.m_buffer)
			, m_buffer_mask(move.m_buffer_mask)
			, m_enqueue_pos(move.m_enqueue_pos.load(Order::Relaxed))
			, m_dequeue_pos(move.m_dequeue_pos.load(Order::Relaxed)) {
			move.m_buffer = nullptr;
			move.m_buffer_mask = 0;
		}
		MACH_ALWAYS_INLINE MPMC& operator=(MPMC<T>&& move) noexcept {
			this->~MPMC();

			m_buffer = move.m_buffer;
			m_buffer_mask = move.m_buffer_mask;

			const auto enqueue_pos = move.m_enqueue_pos.load(Order::Relaxed);
			m_enqueue_pos.store(enqueue_pos, Order::Relaxed);

			const auto dequeue_pos = move.m_dequeue_pos.load(Order::Relaxed);
			m_dequeue_pos.store(dequeue_pos, Order::Relaxed);

			move.m_buffer = nullptr;
			move.m_buffer_mask = 0;

			return *this;
		}

		~MPMC() {
			if (m_buffer) {
				for (u32 i = 0; i < m_buffer_mask + 1; i += 1) {
					m_buffer[i].~Cell();
				}
				Memory::free(m_buffer);
				m_buffer = nullptr;
			}
		}

		bool push(const T& t) const
			requires Copyable<T>
		{
			T copy = t;
			return push(Mach::move(copy));
		}

		bool push(T&& t) const {
			Cell* cell = nullptr;

			auto pos = m_enqueue_pos.load(Order::Relaxed);
			for (;;) {
				cell = &m_buffer[pos & m_buffer_mask];
				const auto seq = cell->sequence.load(Order::Acquire);
				const auto dif = static_cast<isize>(seq) - static_cast<isize>(pos);

				if (dif == 0) {
					if (m_enqueue_pos.compare_exchange_weak(pos, pos + 1, Order::Relaxed).is_set()) break;
				} else if (dif < 0) {
					return false;
				} else {
					pos = m_enqueue_pos.load(Order::Relaxed);
				}
			}

			cell->data = Mach::move(t);
			cell->sequence.store(pos + 1, Order::Release);

			return true;
		}

		MACH_NO_DISCARD Option<T> pop() const {
			Cell* cell = nullptr;

			auto pos = m_dequeue_pos.load(Order::Relaxed);
			for (;;) {
				cell = &m_buffer[pos & m_buffer_mask];
				const auto seq = cell->sequence.load(Order::Acquire);
				const auto dif = static_cast<isize>(seq) - static_cast<isize>(pos + 1);

				if (dif == 0) {
					if (m_dequeue_pos.compare_exchange_weak(pos, pos + 1, Order::Relaxed).is_set()) break;
				} else if (dif < 0) {
					return nullopt;
				} else {
					pos = m_dequeue_pos.load(Order::Relaxed);
				}
			}

			T t = cell->data.unwrap();
			cell->sequence.store(pos + m_buffer_mask + 1, Order::Release);

			return t;
		}

	private:
		struct CacheLinePad {
			u8 internal[MACH_CACHE_LINE_SIZE];
			CacheLinePad() : internal{} {}
		};
		MPMC(Cell* buffer, u32 size) : m_buffer(buffer), m_buffer_mask(size - 1) {}

		CacheLinePad m_pad0;
		Cell* m_buffer;
		usize m_buffer_mask;
		CacheLinePad m_pad1;
		Atomic<usize> m_enqueue_pos{ 0 };
		CacheLinePad m_pad2;
		Atomic<usize> m_dequeue_pos{ 0 };
		CacheLinePad m_pad3;
	};
} // namespace Mach::Core
