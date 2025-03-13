/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Hash.hpp>

namespace Forge::Core {
	const u64 FNV1Hasher::offset_basic = 0xcbf29ce484222325ULL;
	const u64 FNV1Hasher::prime = 0x100000001b3ULL;

	u64 FNV1Hasher::finish() { return m_result; }

	void FNV1Hasher::write(Slice<u8 const> bytes) {
		u64 hash = FNV1Hasher::offset_basic;
		for (usize i = 0; i < bytes.len(); ++i) {
			hash *= FNV1Hasher::prime;
			hash = hash ^ bytes[i];
		}
		m_result |= hash;
	}
} // namespace Forge::Core
