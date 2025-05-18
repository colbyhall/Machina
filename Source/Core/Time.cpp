/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Time.hpp>

#include <Core/Containers/Option.hpp>

#if FORGE_OS == FORGE_OS_WINDOWS
	#include <Core/Windows.hpp>
#else
	#include <time.h>
#endif

namespace Forge::Core {
#if FORGE_OS == FORGE_OS_WINDOWS
	static u64 acquire_frequency() {
		LARGE_INTEGER freq;
		const auto result = ::QueryPerformanceFrequency(&freq);
		FORGE_ASSERT(result);
		return static_cast<u64>(freq.QuadPart);
	}
	static const u64 g_timer_frequency = acquire_frequency();

	Instant Instant::now() {
		LARGE_INTEGER ticks;
		const auto result = QueryPerformanceCounter(&ticks);
		FORGE_ASSERT(result);
		return Instant(static_cast<u64>(ticks.QuadPart));
	}

	Duration Instant::since(Instant earlier) const {
		auto duration = m_ticks - earlier.m_ticks;
		const auto secs = duration / g_timer_frequency;
		duration -= secs * g_timer_frequency;
		const auto nanos = (u32)(duration * (nanos_per_sec / g_timer_frequency));
		return Duration(secs, nanos);
	}
#else
	Instant Instant::now() {
		timespec time;
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time);
		return Instant(time.tv_sec, time.tv_nsec);
	}

	Duration Instant::since(Instant earlier) const {
		return Duration(m_secs - earlier.m_secs, m_nanos - earlier.m_nanos);
	}
#endif
} // namespace Forge::Core
