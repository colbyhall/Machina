/**
 * copyright (c) 2024 colby hall <me@cobeh.com>
 *
 * this software is released under the mit license.
 */

#pragma once

#include <Core/Core.hpp>
#include <Core/TypeTraits.hpp>

namespace Grizzly::Core {
	template <typename T>
	class Interface {
	public:
		explicit Interface(T* internal) : m_internal(internal) {
			@autoreleasepool {
				[m_internal retain];
			}
		}
		Interface(const Interface& c) : m_internal(c.m_internal) {
			@autoreleasepool {
				[m_internal retain];
			}
		}
		Interface& operator=(const Interface& c) {
			@autoreleasepool {
				m_internal = c.m_internal;
				[m_internal retain];
			}
			return *this;
		}
		Interface(Interface&& move) : m_internal(move.m_internal) { move.m_internal = nil; }
		Interface& operator=(Interface&& move) {
			@autoreleasepool {
				m_internal = move.m_internal;
				move.m_internal = nil;
			}
		}
		~Interface() {
			@autoreleasepool {
				if (m_internal) {
					[m_internal autorelease];
					m_internal = nil;
				}
			}
		}
		GRIZZLY_ALWAYS_INLINE T* operator*() const { return m_internal; }
		GRIZZLY_ALWAYS_INLINE operator T*() const { return m_internal; }

	private:
		T* m_internal;
	};
} // namespace Grizzly::Core
