/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include "core/concepts.h"
#include "core/containers/option.h"
#include "core/containers/slice.h"
#include "core/debug/assertions.h"
#include "core/initializer_list.h"
#include "core/memory.h"
#include "core/primitives.h"

namespace op::core {
	template <typename T>
	inline constexpr bool is_array_allocator = false;
	struct HeapAllocator;
	template <typename T>
	concept ArrayAllocator = is_array_allocator<T>;

	template <typename T, ArrayAllocator Allocator = HeapAllocator>
	class Array {
	private:
		template <typename T>
		using Storage = Allocator::template Storage<T>;
		inline constexpr static bool allocator_supports_reserve = Allocator::template supports_reserve;

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

		OP_NO_DISCARD OP_ALWAYS_INLINE usize len() const { return m_len; }
		OP_NO_DISCARD OP_ALWAYS_INLINE usize cap() const { return m_storage.cap(); }

		OP_NO_DISCARD OP_ALWAYS_INLINE bool is_empty() const { return len() == 0; }
		OP_NO_DISCARD OP_ALWAYS_INLINE explicit operator bool() const { return !is_empty(); }
		OP_NO_DISCARD OP_ALWAYS_INLINE bool is_valid_index(usize index) const { return index < len(); }

		OP_ALWAYS_INLINE Element* begin() { return m_storage.data(); }
		OP_ALWAYS_INLINE Element* end() { return m_storage.data() + m_len; }
		OP_ALWAYS_INLINE const Element* begin() const { return m_storage.data(); }
		OP_ALWAYS_INLINE const Element* end() const { return m_storage.data() + m_len; }
		OP_ALWAYS_INLINE const Element* cbegin() const { return m_storage.data(); }
		OP_ALWAYS_INLINE const Element* cend() const { return m_storage.data() + m_len; }

		OP_NO_DISCARD OP_ALWAYS_INLINE Slice<T> as_slice() { return Slice{ m_storage.data(), m_len }; }
		OP_NO_DISCARD OP_ALWAYS_INLINE Slice<const T> as_const_slice() const
			requires(!is_const<T>)
		{
			return Slice<const T>{ m_storage.data(), m_len };
		}

		OP_NO_DISCARD OP_ALWAYS_INLINE Element& operator[](usize index);
		OP_NO_DISCARD OP_ALWAYS_INLINE const Element& operator[](usize index) const;

		OP_NO_DISCARD OP_ALWAYS_INLINE Option<Element&> get(usize index);
		OP_NO_DISCARD OP_ALWAYS_INLINE Option<Element const&> get(usize index) const;

		OP_NO_DISCARD OP_ALWAYS_INLINE Option<Element&> last();
		OP_NO_DISCARD OP_ALWAYS_INLINE Option<Element const&> last() const;

		OP_ALWAYS_INLINE void reserve(usize amount)
			requires allocator_supports_reserve;

		void insert(usize index, Element&& item)
			requires MoveConstructible<Element>;
		void insert(usize index, const Element& item)
			requires CopyConstructible<Element>;

		OP_ALWAYS_INLINE usize push(Element&& item)
			requires MoveConstructible<Element>;
		OP_ALWAYS_INLINE usize push(const Element& item)
			requires CopyConstructible<Element>;

		Element remove(usize index)
			requires Movable<Element> || Copyable<Element>;

		OP_ALWAYS_INLINE Option<Element> pop()
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
			core::copy(m_storage.data(), initializer_list.begin(), count * sizeof(Element));
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
			core::copy(m_storage.data(), slice.begin(), count * sizeof(Element));
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
			new (m_storage.data() + i) Element(copy[i]);
		}
	}

	template <typename T, ArrayAllocator Allocator>
	Array<T, Allocator>& Array<T, Allocator>::operator=(const Array& copy) noexcept
		requires CopyConstructible<Element>
	{
		auto to_destroy = op::move(*this);
		OP_UNUSED(to_destroy);

		m_len = copy.m_len;
		if constexpr (allocator_supports_reserve) {
			reserve(m_len);
		}
		for (usize i = 0; i < m_len; ++i) {
			new (m_storage.data() + i) Element(copy[i]);
		}

		return *this;
	}

	template <typename T, ArrayAllocator Allocator>
	Array<T, Allocator>::Array(Array&& move) noexcept : m_storage(op::move(move.m_storage))
													  , m_len(move.m_len) {
		move.m_len = 0;
	}

	template <typename T, ArrayAllocator Allocator>
	Array<T, Allocator>& Array<T, Allocator>::operator=(Array&& move) noexcept {
		auto to_destroy = op::move(*this);
		OP_UNUSED(to_destroy);

		m_len = move.m_len;
		move.m_len = 0;
		m_storage = op::move(move.m_storage);
		return *this;
	}

	template <typename T, ArrayAllocator Allocator>
	Array<T, Allocator>::~Array() {
		for (usize i = 0; i < m_len; ++i) {
			Element& item = m_storage.data()[i];
			item.~Element();
		}
	}

	template <typename T, ArrayAllocator Allocator>
	OP_ALWAYS_INLINE Array<T, Allocator>::Element& Array<T, Allocator>::operator[](usize index) {
		OP_ASSERT(is_valid_index(index), "Index out of bounds");
		return m_storage.data()[index];
	}

	template <typename T, ArrayAllocator Allocator>
	OP_ALWAYS_INLINE const Array<T, Allocator>::Element& Array<T, Allocator>::operator[](usize index) const {
		OP_ASSERT(is_valid_index(index), "Index out of bounds");
		return m_storage.data()[index];
	}

	template <typename T, ArrayAllocator Allocator>
	OP_NO_DISCARD OP_ALWAYS_INLINE Option<typename Array<T, Allocator>::Element&>
	Array<T, Allocator>::get(usize index) {
		if (is_valid_index(index)) {
			return m_storage.data()[index];
		}
		return nullopt;
	}

	template <typename T, ArrayAllocator Allocator>
	OP_NO_DISCARD OP_ALWAYS_INLINE Option<typename Array<T, Allocator>::Element const&>
	Array<T, Allocator>::get(usize index) const {
		if (is_valid_index(index)) {
			return m_storage.data()[index];
		}
		return nullopt;
	}

	template <typename T, ArrayAllocator Allocator>
	OP_NO_DISCARD OP_ALWAYS_INLINE Option<typename Array<T, Allocator>::Element&> Array<T, Allocator>::last() {
		if (!is_empty()) {
			return m_storage.data()[m_len - 1];
		}
		return nullopt;
	}

	template <typename T, ArrayAllocator Allocator>
	OP_NO_DISCARD OP_ALWAYS_INLINE Option<typename Array<T, Allocator>::Element const&>
	Array<T, Allocator>::last() const {
		if (!is_empty()) {
			return m_storage.data()[m_len - 1];
		}
		return nullopt;
	}

	template <typename T, ArrayAllocator Allocator>
	OP_ALWAYS_INLINE void Array<T, Allocator>::reserve(usize amount)
		requires allocator_supports_reserve
	{
		m_storage.reserve(amount);
	}

	template <typename T, ArrayAllocator Allocator>
	void Array<T, Allocator>::insert(usize index, Element&& item)
		requires MoveConstructible<Element>
	{
		OP_ASSERT(index <= m_len);
		if (len() == cap()) {
			if constexpr (allocator_supports_reserve) {
				reserve(1);
			} else {
				OP_PANIC("Ran out of space in array to insert item into.");
			}
		}

		auto* src = m_storage.data() + index;
		if (index != len()) {
			core::move(src + 1, src, (len() - index) * sizeof(Element));
			core::set(src, 0, sizeof(Element));
		}

		new (src) Element{ op::forward<Element>(item) };
		m_len += 1;
	}

	template <typename T, ArrayAllocator Allocator>
	void Array<T, Allocator>::insert(usize index, const Element& item)
		requires CopyConstructible<Element>
	{
		OP_ASSERT(index <= m_len);
		if (len() == cap()) {
			if constexpr (allocator_supports_reserve) {
				reserve(1);
			} else {
				OP_PANIC("Ran out of space in array to insert item into.");
			}
		}

		auto* src = m_storage.data() + index;
		if (index != len()) {
			core::move(src + 1, src, (len() - index) * sizeof(Element));
			core::set(src, 0, sizeof(Element));
		}

		new (src) Element{ item };
		m_len += 1;
	}

	template <typename T, ArrayAllocator Allocator>
	OP_ALWAYS_INLINE usize Array<T, Allocator>::push(Element&& item)
		requires MoveConstructible<Element>
	{
		const auto index = len();
		insert(index, op::forward<T>(item));
		return index;
	}

	template <typename T, ArrayAllocator Allocator>
	OP_ALWAYS_INLINE usize Array<T, Allocator>::push(const Element& item)
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
		OP_ASSERT(is_valid_index(index), "Index out of bounds");

		Option<Element> result = nullopt;
		if constexpr (Movable<Element>) {
			result = op::move(m_storage.data()[index]);
		} else {
			result = m_storage.data()[index];
		}

		// Set memory that element used to occupy to 0
		void* clear = &m_storage.data()[index];
		core::set(clear, 0, sizeof(Element));

		// If not removed from the end of the vector copy entire array over
		if (index < m_len - 1) {
			auto* src = m_storage.data() + index;
			op::core::move(src, src + 1, (len() - index) * sizeof(Element));
		}

		// Decrement length
		m_len -= 1;

		return result;
	}

	template <typename T, ArrayAllocator Allocator>
	OP_ALWAYS_INLINE Option<typename Array<T, Allocator>::Element> Array<T, Allocator>::pop()
		requires Movable<Element> or Copyable<Element>
	{
		if (m_len > 0) {
			m_len -= 1;

			if constexpr (Movable<Element>) {
				return op::move(m_storage.data()[m_len]);
			} else {
				return m_storage.data()[m_len];
			}
		}

		return nullopt;
	}

	template <typename T, ArrayAllocator Allocator>
	void Array<T, Allocator>::reset() {
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
					OP_PANIC("Ran out of space in array to insert item into.");
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
					OP_PANIC("Ran out of space in array to insert item into.");
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
				auto to_destroy = op::move(*this);
				OP_UNUSED(to_destroy);

				m_ptr = move.m_ptr;
				move.m_ptr = nullptr;
				m_cap = move.m_cap;
				move.m_cap = 0;

				return *this;
			}
			~Storage() {
				if (m_ptr) {
					core::free(m_ptr);
					m_ptr = nullptr;
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
					m_ptr = core::alloc(Layout::array<T>(m_cap)).template as<T>();
				} else {
					const auto new_ptr = core::alloc(Layout::array<T>(m_cap));
					core::copy(new_ptr, m_ptr, old_cap * sizeof(T));
					core::free(m_ptr);
					m_ptr = new_ptr.template as<T>();
				}
			}

			OP_ALWAYS_INLINE T* data() const { return m_ptr; }
			OP_ALWAYS_INLINE usize cap() const { return m_cap; }

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
				core::copy(m_bytes, move.m_bytes, sizeof(m_bytes));
				core::set(move.m_bytes, 0, sizeof(move.m_bytes));
			}
			Storage& operator=(Storage&& move) noexcept {
				core::copy(m_bytes, move.m_bytes, sizeof(m_bytes));
				core::set(move.m_bytes, 0, sizeof(move.m_bytes));

				return *this;
			}

			OP_ALWAYS_INLINE T* data() { return reinterpret_cast<T*>(&m_bytes[0]); }
			OP_ALWAYS_INLINE const T* data() const { return reinterpret_cast<const T*>(&m_bytes[0]); }
			OP_ALWAYS_INLINE usize cap() const { return Count; }

		private:
			alignas(T) u8 m_bytes[sizeof(T) * Count];
		};
	};
	template <usize Count>
	inline constexpr bool is_array_allocator<InlineAllocator<Count>> = true;
} // namespace op::core

namespace op {
	using core::Array;
	using core::HeapAllocator;
	using core::InlineAllocator;
} // namespace op