/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * this software is released under the mit license.
 */

#pragma once

#include <Core/Core.hpp>
#include <Core/TypeTraits.hpp>

namespace Mach::Core {
	MACH_CLANG_DISABLE_WARNINGS_PUSH;
	MACH_CLANG_DISABLE_WARNING("-Wobjc-method-access");
	/**
	 * Type erased Unique ptr for Objective-C protocols.
	 *
	 * As of writing this I couldn't get the protocols to work with the C++ type system.
	 */
	class Protocol {
	public:
		explicit Protocol(id internal) : m_internal(internal) {
			@autoreleasepool {
				[m_internal retain];
			}
		}
		Protocol(const Protocol& c) : m_internal(c.m_internal) {
			@autoreleasepool {
				[m_internal retain];
			}
		}
		Protocol& operator=(const Protocol& c) {
			@autoreleasepool {
				m_internal = c.m_internal;
				[m_internal retain];
			}
			return *this;
		}
		Protocol(Protocol&& move) : m_internal(move.m_internal) { move.m_internal = nullptr; }
		Protocol& operator=(Protocol&& move) {
			@autoreleasepool {
				m_internal = move.m_internal;
				move.m_internal = nullptr;
			}
			return *this;
		}
		~Protocol() {
			@autoreleasepool {
				if (m_internal) {
					[m_internal release];
					m_internal = nullptr;
				}
			}
		}
		MACH_ALWAYS_INLINE id operator*() const { return m_internal; }
		MACH_ALWAYS_INLINE operator id() const { return m_internal; }

	private:
		id m_internal;
	};

	MACH_CLANG_DISBALE_WARNINGS_POP;
} // namespace Mach::Core
