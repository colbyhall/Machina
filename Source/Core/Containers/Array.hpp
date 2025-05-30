/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Concepts.hpp>
#include <Core/Containers/Option.hpp>
#include <Core/Containers/Slice.hpp>
#include <Core/Debug/Assertions.hpp>
#include <Core/InitializerList.hpp>
#include <Core/Memory.hpp>
#include <Core/Primitives.hpp>

namespace Mach::Core {
	template <typename T>
	inline constexpr bool is_array_allocator = false;
	struct HeapAllocator;
	template <typename T>
	concept ArrayAllocator = is_array_allocator<T>;

	template <typename T, ArrayAllocator Allocator = HeapAllocator>
	class Array {
	private:
		template <typename D>
		using Storage = Allocator::template Storage<D>;
		inline constexpr static bool allocator_supports_reserve = Allocator::supports_reserve;

	public:
		using Element = T;

		Array() = default;
		Array(InitializerList<Element> initializer_list)
			requires CopyConstructible<Element>;

		Array(const Slice<RemoveConst<Element> const>& slice)
			requires CopyConstructible<Element>;

		Array(const Array& copy) noexcept
			requires CopyConstructible<Element>;
		Array& operator=(const Array& copy) noexcept
			requires CopyConstructible<Element>;
		Array(Array&& move) noexcept;
		Array& operator=(Array&& move) noexcept;
		~Array();

		MACH_NO_DISCARD MACH_ALWAYS_INLINE usize len() const { return m_len; }
		MACH_NO_DISCARD MACH_ALWAYS_INLINE usize cap() const { return m_storage.cap(); }

		MACH_NO_DISCARD MACH_ALWAYS_INLINE bool is_empty() const { return len() == 0; }
		MACH_NO_DISCARD MACH_ALWAYS_INLINE explicit operator bool() const { return !is_empty(); }
		MACH_NO_DISCARD MACH_ALWAYS_INLINE bool is_valid_index(usize index) const { return index < len(); }

		MACH_ALWAYS_INLINE Element* begin() { return m_storage.data(); }
		MACH_ALWAYS_INLINE Element* end() { return m_storage.data() + m_len; }
		MACH_ALWAYS_INLINE const Element* begin() const { return m_storage.data(); }
		MACH_ALWAYS_INLINE const Element* end() const { return m_storage.data() + m_len; }
		MACH_ALWAYS_INLINE const Element* cbegin() const { return m_storage.data(); }
		MACH_ALWAYS_INLINE const Element* cend() const { return m_storage.data() + m_len; }

		MACH_NO_DISCARD MACH_ALWAYS_INLINE Slice<T> as_slice() { return Slice{ m_storage.data(), m_len }; }
		MACH_NO_DISCARD MACH_ALWAYS_INLINE Slice<const T> as_const_slice() const
			requires(!is_const<T>)
		{
			return Slice<const T>{ m_storage.data(), m_len };
		}

		MACH_NO_DISCARD MACH_ALWAYS_INLINE Element& operator[](usize index);
		MACH_NO_DISCARD MACH_ALWAYS_INLINE const Element& operator[](usize index) const;

		MACH_NO_DISCARD MACH_ALWAYS_INLINE Option<Element&> get(usize index);
		MACH_NO_DISCARD MACH_ALWAYS_INLINE Option<Element const&> get(usize index) const;

		MACH_NO_DISCARD MACH_ALWAYS_INLINE Option<Element&> last();
		MACH_NO_DISCARD MACH_ALWAYS_INLINE Option<Element const&> last() const;

		MACH_ALWAYS_INLINE void reserve(usize amount)
			requires allocator_supports_reserve;

		void insert(usize index, Element&& item)
			requires MoveConstructible<Element>;
		void insert(usize index, const Element& item)
			requires CopyConstructible<Element>;

		MACH_ALWAYS_INLINE usize push(Element&& item)
			requires MoveConstructible<Element>;
		MACH_ALWAYS_INLINE usize push(const Element& item)
			requires CopyConstructible<Element>;

		Element remove(usize index)
			requires Movable<Element> || Copyable<Element>;

		MACH_ALWAYS_INLINE Option<Element> pop()
			requires Movable<Element> or Copyable<Element>;

		void set_len(usize len)
			requires DefaultInitializable<Element>;

		void set_len_uninitialized(usize len)
			requires is_trivial<Element>;

		void reset();

	private:
		Storage<Element> m_storage;
		usize m_len = 0;
	};

	template <typename T, ArrayAllocator Allocator>
	Array<T, Allocator>::Array(InitializerList<Element> initializer_list)
		requires CopyConstructible<Element>
	{
		const auto count = initializer_list.size();
		if constexpr (allocator_supports_reserve) {
			reserve(count);
		}

		// Fast path for trivially copyable types
		if constexpr (is_trivially_copyable<T>) {
			Memory::copy(m_storage.data(), initializer_list.begin(), count * sizeof(Element));
			m_len = count;
		}
		// Otherwise call the copy constructor
		else {
			for (const auto& item : initializer_list) {
				// Copy the item into the array
				push(item);
			}
		}
	}

	template <typename T, ArrayAllocator Allocator>
	Array<T, Allocator>::Array(const Slice<RemoveConst<Element> const>& slice)
		requires CopyConstructible<Element>
	{
		const auto count = slice.len();
		if constexpr (allocator_supports_reserve) {
			reserve(count);
		}

		// Fast path for trivially copyable types
		if constexpr (is_trivially_copyable<T>) {
			Memory::copy(m_storage.data(), slice.begin(), count * sizeof(Element));
			m_len = count;
		}
		// Otherwise call the copy constructor
		else {
			for (const auto& item : slice) {
				// Copy the item into the array
				push(item);
			}
		}
	}

	template <typename T, ArrayAllocator Allocator>
	Array<T, Allocator>::Array(const Array& copy) noexcept
		requires CopyConstructible<Element>
		: m_len(copy.m_len) {
		if constexpr (allocator_supports_reserve) {
			reserve(m_len);
		}
		for (usize i = 0; i < m_len; ++i) {
			Memory::emplace<Element>(m_storage.data() + i, copy[i]);
		}
	}

	template <typename T, ArrayAllocator Allocator>
	Array<T, Allocator>& Array<T, Allocator>::operator=(const Array& copy) noexcept
		requires CopyConstructible<Element>
	{
		auto to_destroy = Mach::move(*this);
		MACH_UNUSED(to_destroy);

		m_len = copy.m_len;
		if constexpr (allocator_supports_reserve) {
			reserve(m_len);
		}
		for (usize i = 0; i < m_len; ++i) {
			Memory::emplace<Element>(m_storage.data() + i, copy[i]);
		}

		return *this;
	}

	template <typename T, ArrayAllocator Allocator>
	Array<T, Allocator>::Array(Array&& move) noexcept : m_storage(Mach::move(move.m_storage))
													  , m_len(move.m_len) {
		move.m_len = 0;
	}

	template <typename T, ArrayAllocator Allocator>
	Array<T, Allocator>& Array<T, Allocator>::operator=(Array&& move) noexcept {
		this->~Array();

		m_len = move.m_len;
		move.m_len = 0;

		m_storage = Mach::move(move.m_storage);

		return *this;
	}

	template <typename T, ArrayAllocator Allocator>
	Array<T, Allocator>::~Array() {
		for (usize i = 0; i < m_len; ++i) {
			Element& item = m_storage.data()[i];
			item.~Element();
		}
		m_len = 0;
	}

	template <typename T, ArrayAllocator Allocator>
	MACH_ALWAYS_INLINE Array<T, Allocator>::Element& Array<T, Allocator>::operator[](usize index) {
		MACH_ASSERT(is_valid_index(index), "Index out of bounds");
		return m_storage.data()[index];
	}

	template <typename T, ArrayAllocator Allocator>
	MACH_ALWAYS_INLINE const Array<T, Allocator>::Element& Array<T, Allocator>::operator[](usize index) const {
		MACH_ASSERT(is_valid_index(index), "Index out of bounds");
		return m_storage.data()[index];
	}

	template <typename T, ArrayAllocator Allocator>
	MACH_NO_DISCARD MACH_ALWAYS_INLINE Option<typename Array<T, Allocator>::Element&>
	Array<T, Allocator>::get(usize index) {
		if (is_valid_index(index)) {
			return m_storage.data()[index];
		}
		return nullopt;
	}

	template <typename T, ArrayAllocator Allocator>
	MACH_NO_DISCARD MACH_ALWAYS_INLINE Option<typename Array<T, Allocator>::Element const&>
	Array<T, Allocator>::get(usize index) const {
		if (is_valid_index(index)) {
			return m_storage.data()[index];
		}
		return nullopt;
	}

	template <typename T, ArrayAllocator Allocator>
	MACH_NO_DISCARD MACH_ALWAYS_INLINE Option<typename Array<T, Allocator>::Element&> Array<T, Allocator>::last() {
		if (!is_empty()) {
			return m_storage.data()[m_len - 1];
		}
		return nullopt;
	}

	template <typename T, ArrayAllocator Allocator>
	MACH_NO_DISCARD MACH_ALWAYS_INLINE Option<typename Array<T, Allocator>::Element const&>
	Array<T, Allocator>::last() const {
		if (!is_empty()) {
			return m_storage.data()[m_len - 1];
		}
		return nullopt;
	}

	template <typename T, ArrayAllocator Allocator>
	MACH_ALWAYS_INLINE void Array<T, Allocator>::reserve(usize amount)
		requires allocator_supports_reserve
	{
		m_storage.reserve(amount);
	}

	template <typename T, ArrayAllocator Allocator>
	void Array<T, Allocator>::insert(usize index, Element&& item)
		requires MoveConstructible<Element>
	{
		MACH_ASSERT(index <= m_len);
		if (len() == cap()) {
			if constexpr (allocator_supports_reserve) {
				reserve(1);
			} else {
				MACH_PANIC("Ran out of space in array to insert item into.");
			}
		}

		auto* src = m_storage.data() + index;
		if (index != len()) {
			Memory::move(src + 1, src, (len() - index) * sizeof(Element));
#if MACH_BUILD == MACH_BUILD_DEBUG
			Memory::set(src, 0, sizeof(Element));
#endif
		}

		new (src) Element{ Mach::forward<Element>(item) };
		m_len += 1;
	}

	template <typename T, ArrayAllocator Allocator>
	void Array<T, Allocator>::insert(usize index, const Element& item)
		requires CopyConstructible<Element>
	{
		MACH_ASSERT(index <= m_len);
		if (len() == cap()) {
			if constexpr (allocator_supports_reserve) {
				reserve(1);
			} else {
				MACH_PANIC("Ran out of space in array to insert item into.");
			}
		}

		auto* src = m_storage.data() + index;
		if (index != len()) {
			Memory::move(src + 1, src, (len() - index) * sizeof(Element));
#if MACH_BUILD == MACH_BUILD_DEBUG
			Memory::set(src, 0, sizeof(Element));
#endif
		}

		new (src) Element{ item };
		m_len += 1;
	}

	template <typename T, ArrayAllocator Allocator>
	MACH_ALWAYS_INLINE usize Array<T, Allocator>::push(Element&& item)
		requires MoveConstructible<Element>
	{
		const auto index = len();
		insert(index, Mach::forward<T>(item));
		return index;
	}

	template <typename T, ArrayAllocator Allocator>
	MACH_ALWAYS_INLINE usize Array<T, Allocator>::push(const Element& item)
		requires CopyConstructible<Element>
	{
		const auto index = len();
		insert(index, item);
		return index;
	}

	template <typename T, ArrayAllocator Allocator>
	T Array<T, Allocator>::remove(usize index)
		requires Movable<Element> || Copyable<Element>
	{
		MACH_ASSERT(is_valid_index(index), "Index out of bounds");

		Option<Element> result = nullopt;
		if constexpr (Movable<Element>) {
			result = Mach::move(m_storage.data()[index]);
		} else {
			result = m_storage.data()[index];
		}

#if MACH_BUILD == MACH_BUILD_DEBUG
		// Set memory that element used to occupy to 0
		void* clear = &m_storage.data()[index];
		Memory::set(clear, 0, sizeof(Element));
#endif

		// If not removed from the end of the vector copy entire array over
		if (index < m_len - 1) {
			auto* src = m_storage.data() + index;
			Memory::move(src, src + 1, (len() - index) * sizeof(Element));
		}

		// Decrement length
		m_len -= 1;

		return result.unwrap();
	}

	template <typename T, ArrayAllocator Allocator>
	MACH_ALWAYS_INLINE Option<typename Array<T, Allocator>::Element> Array<T, Allocator>::pop()
		requires Movable<Element> or Copyable<Element>
	{
		if (m_len > 0) {
			m_len -= 1;

			if constexpr (Movable<Element>) {
				return Mach::move(m_storage.data()[m_len]);
			} else {
				return m_storage.data()[m_len];
			}
		}

		return nullopt;
	}

	template <typename T, ArrayAllocator Allocator>
	void Array<T, Allocator>::reset() {
		// TODO: Just reset the length if the type is trivially destructible
		const i32 count = static_cast<i32>(len());
		for (i32 i = count - 1; i >= 0; i -= 1) {
			remove(static_cast<usize>(i));
		}
	}

	template <typename T, ArrayAllocator Allocator>
	void Array<T, Allocator>::set_len(usize len)
		requires DefaultInitializable<Element>
	{
		if (len == m_len) {
			return;
		}

		if (len > m_len) {
			if (len > cap()) {
				if constexpr (allocator_supports_reserve) {
					reserve(len - m_len);
				} else {
					MACH_PANIC("Ran out of space in array to insert item into.");
				}
			}

			for (usize i = m_len; i < len; ++i) {
				new (m_storage.data() + i) Element();
			}
		} else {
			for (usize i = len; i < m_len; ++i) {
				m_storage.data()[i].~Element();
			}
		}
		m_len = len;
	}

	template <typename T, ArrayAllocator Allocator>
	void Array<T, Allocator>::set_len_uninitialized(usize len)
		requires is_trivial<Element>
	{
		if (len == m_len) {
			return;
		}

		if (len > m_len) {
			if (len > cap()) {
				if constexpr (allocator_supports_reserve) {
					reserve(len - m_len);
				} else {
					MACH_PANIC("Ran out of space in array to insert item into.");
				}
			}
		}
		m_len = len;
	}

	struct HeapAllocator {
		static inline constexpr bool supports_reserve = true;

		template <typename T>
		class Storage {
		public:
			Storage() = default;
			Storage(const Storage& copy) noexcept = delete;
			Storage& operator=(const Storage& copy) noexcept = delete;
			Storage(Storage&& move) noexcept : m_ptr(move.m_ptr), m_cap(move.m_cap) {
				move.m_ptr = nullptr;
				move.m_cap = 0;
			}
			Storage& operator=(Storage&& move) noexcept {
				this->~Storage();

				m_ptr = move.m_ptr;
				m_cap = move.m_cap;

				move.m_ptr = nullptr;
				move.m_cap = 0;

				return *this;
			}
			~Storage() {
				if (m_ptr) {
					Memory::free(m_ptr);
					m_ptr = nullptr;
					m_cap = 0;
				}
			}

			void reserve(usize amount) {
				const auto desired = m_cap + amount;
				const auto old_cap = m_cap;
				while (m_cap < desired) {
					m_cap += desired >> 1;
					m_cap += 1;
				}

				if (m_ptr == nullptr) {
					m_ptr = Memory::alloc(Memory::Layout::array<T>(m_cap)).template as<T>();
				} else {
					const auto new_ptr = Memory::alloc(Memory::Layout::array<T>(m_cap));
					Memory::copy(new_ptr, m_ptr, old_cap * sizeof(T));
					Memory::free(m_ptr);
					m_ptr = new_ptr.template as<T>();
				}
			}

			MACH_ALWAYS_INLINE T* data() const { return m_ptr; }
			MACH_ALWAYS_INLINE usize cap() const { return m_cap; }

		private:
			T* m_ptr = nullptr;
			usize m_cap = 0;
		};
	};
	template <>
	inline constexpr bool is_array_allocator<HeapAllocator> = true;

	template <usize Count>
	struct InlineAllocator {
		static inline constexpr bool supports_reserve = false;

		template <typename T>
		struct Storage {
		public:
			Storage() = default;
			Storage(const Storage& copy) noexcept = delete;
			Storage& operator=(const Storage& copy) noexcept = delete;
			Storage(Storage&& move) noexcept {
				Memory::copy(m_bytes, move.m_bytes, sizeof(m_bytes));
#if MACH_BUILD == MACH_BUILD_DEBUG
				Memory::set(move.m_bytes, 0, sizeof(move.m_bytes));
#endif
			}
			Storage& operator=(Storage&& move) noexcept {
				Memory::copy(m_bytes, move.m_bytes, sizeof(m_bytes));
#if MACH_BUILD == MACH_BUILD_DEBUG
				Memory::set(move.m_bytes, 0, sizeof(move.m_bytes));
#endif

				return *this;
			}

			MACH_ALWAYS_INLINE T* data() { return reinterpret_cast<T*>(&m_bytes[0]); }
			MACH_ALWAYS_INLINE const T* data() const { return reinterpret_cast<const T*>(&m_bytes[0]); }
			MACH_ALWAYS_INLINE usize cap() const { return Count; }

		private:
			alignas(T) u8 m_bytes[sizeof(T) * Count];
		};
	};
	template <usize Count>
	inline constexpr bool is_array_allocator<InlineAllocator<Count>> = true;
} // namespace Mach::Core

namespace Mach {
	using Core::Array;
	using Core::HeapAllocator;
	using Core::InlineAllocator;
} // namespace Mach
