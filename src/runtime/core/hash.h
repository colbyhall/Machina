/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include "core/containers/slice.h"

namespace op::core {
	class Hasher {
	public:
		virtual u64 finish() = 0;
		virtual void write(Slice<u8 const> bytes) = 0;
		virtual ~Hasher() = default;
	};

	class FNV1Hasher final : public Hasher {
	public:
		constexpr FNV1Hasher() = default;

		static const u64 offset_basic;
		static const u64 prime;

		// Hasher
		u64 finish() final;
		void write(Slice<u8 const> bytes) final;
		// ~Hasher

	private:
		u64 m_result = 0;
	};
} // namespace op::core

namespace op {
	template <typename Hasher>
	void hash(Hasher& hasher, const u8& value) {
		hasher.write({ reinterpret_cast<const u8*>(&value), sizeof(u8) });
	}
	template <typename Hasher>
	void hash(Hasher& hasher, const u16& value) {
		hasher.write({ reinterpret_cast<const u8*>(&value), sizeof(u16) });
	}
	template <typename Hasher>
	void hash(Hasher& hasher, const u32& value) {
		hasher.write(Slice<u8 const>{ reinterpret_cast<const u8*>(&value), sizeof(u32) });
	}
	template <typename Hasher>
	void hash(Hasher& hasher, const u64& value) {
		hasher.write({ reinterpret_cast<const u8*>(&value), sizeof(u64) });
	}
	template <typename Hasher>
	void hash(Hasher& hasher, const i8& value) {
		hasher.write({ reinterpret_cast<const u8*>(&value), sizeof(i8) });
	}
	template <typename Hasher>
	void hash(Hasher& hasher, const i16& value) {
		hasher.write({ reinterpret_cast<const u8*>(&value), sizeof(i16) });
	}
	template <typename Hasher>
	void hash(Hasher& hasher, const i32& value) {
		hasher.write({ reinterpret_cast<const u8*>(&value), sizeof(i32) });
	}
	template <typename Hasher>
	void hash(Hasher& hasher, const i64& value) {
		hasher.write({ reinterpret_cast<const u8*>(&value), sizeof(i64) });
	}
	template <typename Hasher>
	void hash(Hasher& hasher, const f32& value) {
		hasher.write({ reinterpret_cast<const u8*>(&value), sizeof(f32) });
	}
	template <typename Hasher>
	void hash(Hasher& hasher, const f64& value) {
		hasher.write({ reinterpret_cast<const u8*>(&value), sizeof(f64) });
	}
} // namespace op