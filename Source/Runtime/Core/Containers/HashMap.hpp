/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Containers/Array.hpp>
#include <Core/Hash.hpp>

namespace Grizzly::Core {
	template <typename Key, typename Value>
		requires Copyable<Key> && EqualityComparable<Key>
	class HashMap {
	public:
		constexpr HashMap() = default;

		GRIZZLY_NO_DISCARD GRIZZLY_ALWAYS_INLINE usize len() const { return m_buckets.len(); }
		GRIZZLY_NO_DISCARD GRIZZLY_ALWAYS_INLINE usize cap() const { return m_buckets.cap(); }

		void insert(const Key& key, Value&& value)
			requires Movable<Value>
		{
			m_buckets.push(Bucket{ .key = key, .value = Grizzly::forward<Value>(value) });
			refresh_layout();
		}
		void insert(const Key& key, const Value& value)
			requires CopyConstructible<Value>
		{
			m_buckets.push(Bucket{ .key = key, .value = value });
			refresh_layout();
		}

		Option<Value> remove(const Key& key) {
			if (m_buckets.is_empty()) {
				return nullopt;
			}

			const auto index = key_to_layout_index(key);
			const auto mapped = m_layout[index];
			if (!mapped.is_set()) {
				return nullopt;
			}

			Option<usize> bucket_index = mapped;
			Bucket* bucket = &m_buckets[mapped.unwrap()];
			while (true) {
				if (bucket->key == key) break;
				if (!bucket->next.is_set()) {
					bucket = nullptr;
					break;
				}
				bucket_index = bucket->next;
				bucket = &m_buckets[bucket->next.unwrap()];
			}

			if (bucket == nullptr) {
				return nullopt;
			}

			Bucket result = m_buckets.remove(bucket_index.unwrap());
			refresh_layout();

			return result.value;
		}

		Option<Value&> find(const Key& key) {
			if (m_buckets.is_empty()) {
				return nullopt;
			}

			const auto index = key_to_layout_index(key);
			const auto mapped = m_layout[index];
			if (!mapped.is_set()) {
				return nullopt;
			}

			auto* bucket = &m_buckets[mapped.unwrap()];
			while (true) {
				if (bucket->key == key) return bucket->value;
				if (!bucket->next.is_set()) break;
				bucket = &m_buckets[bucket->next.unwrap()];
			}

			return nullopt;
		}

		Option<Value const&> find(const Key& key) const {
			if (m_buckets.is_empty()) {
				return nullopt;
			}

			const auto index = key_to_layout_index(key);
			const auto mapped = m_layout[index];
			if (!mapped.is_set()) {
				return nullopt;
			}

			const auto* bucket = &m_buckets[mapped];
			while (true) {
				if (bucket->key == key) return bucket->value;
				if (!bucket->next.is_set()) break;
				bucket = &m_buckets[bucket->next.unwrap()];
			}

			return nullopt;
		}

	private:
		usize key_to_layout_index(const Key& key) const {
			FNV1Hasher hasher{};
			hash(hasher, key);
			return hasher.finish() % m_layout.len();
		}

		void refresh_layout() {
			m_layout.reset();
			m_layout.set_len(m_buckets.len());

			// Layout buckets by hash(key) & buckets.len() and build tree if collision
			// detected
			for (usize i = 0; i < m_buckets.len(); ++i) {
				auto& bucket = m_buckets[i];

				const auto layout_index = key_to_layout_index(bucket.key);
				bucket.next = nullopt;

				// Check what index lies in the layout array
				auto& found = m_layout[layout_index];

				// If its invalid then simply set the bucket index
				if (!found.is_set()) {
					found = i;
				} else {
					// If its valid then descend the bucket tree until an empty spot is
					// found
					auto* other = &m_buckets[found.unwrap()];
					while (other->next.is_set()) {
						other = &m_buckets[other->next.unwrap()];
					}
					other->next = i;
				}
			}
		}

		struct Bucket {
			Key key;
			Value value;
			Option<usize> next;
		};
		Array<Bucket> m_buckets;
		Array<Option<usize>> m_layout;
	};
} // namespace Grizzly::Core
