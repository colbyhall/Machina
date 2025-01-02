/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Debug/Assertions.hpp>
#include <Core/Primitives.hpp>
#include <Core/TypeTraits.hpp>

namespace Grizzly::Core {
	template <typename T>
	class NonNull;

	// void ptr specialization
	template <>
	class NonNull<void> {
	public:
		// Only way to initialize NonNull is by a valid ptr
		GRIZZLY_ALWAYS_INLINE constexpr NonNull(void* ptr) : m_ptr(ptr) {
			GRIZZLY_ASSERT(m_ptr != nullptr, "NonNull only accepts pointers that are not nullptr");
		}

		// Prevent default and nullptr initialization
		NonNull() = delete;
		NonNull(NullPtr) = delete;

		// Accessor
		GRIZZLY_ALWAYS_INLINE operator void*() const { return m_ptr; }
		GRIZZLY_ALWAYS_INLINE void* operator*() const { return m_ptr; }

		// Compare ops
		GRIZZLY_ALWAYS_INLINE bool operator==(NonNull<void> ptr) const { return ptr.m_ptr == m_ptr; }
		GRIZZLY_ALWAYS_INLINE bool operator==(void* ptr) const { return ptr == m_ptr; }
		GRIZZLY_ALWAYS_INLINE bool operator!=(NonNull<void> ptr) const { return ptr.m_ptr != m_ptr; }
		GRIZZLY_ALWAYS_INLINE bool operator!=(void* ptr) const { return ptr != m_ptr; }

		// Casting
		template <typename T>
		GRIZZLY_ALWAYS_INLINE NonNull<T> as() const {
			return static_cast<T*>(m_ptr);
		}

	private:
		void* m_ptr;
	};

	// const void ptr specialization
	template <>
	class NonNull<void const> {
	public:
		// Only way to initialize NonNull is by a valid ptr
		GRIZZLY_ALWAYS_INLINE constexpr NonNull(void const* ptr) : m_ptr(ptr) {
			GRIZZLY_ASSERT(m_ptr != nullptr, "NonNull only accepts pointers that are not nullptr");
		}

		// Prevent default and nullptr initialization
		NonNull() = delete;
		NonNull(NullPtr) = delete;

		// Accessor
		GRIZZLY_ALWAYS_INLINE operator void const*() const { return m_ptr; }
		GRIZZLY_ALWAYS_INLINE void const* operator*() const { return m_ptr; }

		// Compare ops
		GRIZZLY_ALWAYS_INLINE bool operator==(NonNull<void const> ptr) const { return ptr.m_ptr == m_ptr; }
		GRIZZLY_ALWAYS_INLINE bool operator==(void const* ptr) const { return ptr == m_ptr; }
		GRIZZLY_ALWAYS_INLINE bool operator!=(NonNull<void const> ptr) const { return ptr.m_ptr != m_ptr; }
		GRIZZLY_ALWAYS_INLINE bool operator!=(void const* ptr) const { return ptr != m_ptr; }

		// Casting
		template <typename T>
			requires is_const<T>
		GRIZZLY_ALWAYS_INLINE NonNull<T> as() const {
			return static_cast<T*>(m_ptr);
		}

	private:
		void const* m_ptr;
	};

	template <typename T>
	class NonNull {
	public:
		// Only way to initialize NonNull is by a valid ptr
		GRIZZLY_ALWAYS_INLINE constexpr NonNull(T* ptr) : m_ptr(ptr) {
			GRIZZLY_ASSERT(m_ptr != nullptr, "NonNull only accepts pointers that are not nullptr");
		}

		// Prevent default and nullptr initialization
		NonNull() = delete;
		NonNull(NullPtr) = delete;

		// Accessors
		GRIZZLY_ALWAYS_INLINE operator T*() const { return m_ptr; }
		GRIZZLY_ALWAYS_INLINE operator NonNull<void>() const { return m_ptr; }
		GRIZZLY_ALWAYS_INLINE operator NonNull<void const>() const { return m_ptr; }
		GRIZZLY_ALWAYS_INLINE T* operator->() const { return m_ptr; }
		GRIZZLY_ALWAYS_INLINE T& operator*() const { return *m_ptr; }
		GRIZZLY_ALWAYS_INLINE T& operator[](usize index) const { return m_ptr[index]; }

		// Compare ops
		GRIZZLY_ALWAYS_INLINE bool operator==(NonNull<T> ptr) const { return ptr.m_ptr == m_ptr; }
		GRIZZLY_ALWAYS_INLINE bool operator==(T* ptr) const { return ptr == m_ptr; }
		GRIZZLY_ALWAYS_INLINE bool operator!=(NonNull<T> ptr) const { return ptr.m_ptr != m_ptr; }
		GRIZZLY_ALWAYS_INLINE bool operator!=(T* ptr) const { return ptr != m_ptr; }

	private:
		T* m_ptr;
	};

} // namespace Grizzly::Core

namespace Grizzly {
	using Core::NonNull;
} // namespace Grizzly
