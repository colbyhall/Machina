/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Core.hpp>
#include <Core/Primitives.hpp>

namespace Mach::Core {
	constexpr u64 nanos_per_sec = 1000000000;
	constexpr u64 nanos_per_milli = 1000000;
	constexpr u64 nanos_per_micro = 1000;
	constexpr u64 millis_per_sec = 1000;
	constexpr u64 micros_per_sec = 1000000;

	class Duration {
	public:
		constexpr explicit Duration(u64 secs, u32 nanos) : m_secs(secs), m_nanos(nanos) {}
		MACH_ALWAYS_INLINE static Duration from_millis(u64 millis) {
			return Duration(millis / millis_per_sec, static_cast<u32>(millis % millis_per_sec));
		}

		MACH_ALWAYS_INLINE f32 as_secs_f32() const { return (f32)m_secs + ((f32)m_nanos / (f32)nanos_per_sec); }
		MACH_ALWAYS_INLINE f64 as_secs_f64() const { return (f64)m_secs + ((f64)m_nanos / (f64)nanos_per_sec); }

	private:
		u64 m_secs;
		u32 m_nanos;
	};

	class Instant {
	public:
		static Instant now();

		Duration since(Instant earlier) const;
		MACH_ALWAYS_INLINE Duration elapsed() const { return Instant::now().since(*this); }

	private:
#if MACH_OS == MACH_OS_WINDOWS
		explicit Instant(u64 ticks) : m_ticks(ticks) {}
		u64 m_ticks;
#else
		explicit Instant(u64 secs, u32 nanos) : m_secs(secs), m_nanos(nanos) {}
		u64 m_secs;
		u32 m_nanos;
#endif
	};
} // namespace Mach::Core
