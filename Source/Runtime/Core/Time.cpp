/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Time.hpp>

#if GRIZZLY_PLATFORM != GRIZZLY_PLATFORM_WINDOWS
	#include <time.h>
#endif

namespace Grizzly::Core {
	Instant Instant::now() {
		timespec time;
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time);
		return Instant(time.tv_sec, time.tv_nsec);
	}

	Duration Instant::since(Instant earlier) const {
		return Duration(m_secs - earlier.m_secs, m_nanos - earlier.m_nanos);
	}
} // namespace Grizzly::Core
