/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Containers/String.hpp>
#include <Core/Debug/Assertions.hpp>
#include <Core/IO/Writer.hpp>
#include <cstdio>
#undef stderr
#undef stdin
#undef stdout

namespace Grizzly {
	template <typename T>
	struct Formatter;
} // namespace Grizzly

namespace Grizzly::Core {
	GRIZZLY_ALWAYS_INLINE u32 format_one(Writer& writer, const StringView& fmt) { return writer.write(fmt); }

	template <typename T>
	u32 format_one(Writer& writer, const StringView& fmt, const T& arg) {
		u32 bytes_written = 0;
		bool found_arg = false;
		for (auto iter = fmt.chars(); iter; ++iter) {
			const auto c = *iter;
			if (c == '{') {
				++iter;
				if (found_arg) {
					GRIZZLY_PANIC("Too many arguments provided");
				}
				found_arg = true;
				if (auto next = iter) {
					const auto b = *next;

					if (b == '{') {
						const u8 byte = '{';
						bytes_written += writer.write(Slice<u8 const>{ &byte, 1 });
					} else if (b == '}') {
						Formatter<T> formatter;
						bytes_written += formatter.format(writer, arg);
					} else {
						GRIZZLY_PANIC("Invalid format string");
					}
				}
			} else {
				// TODO: Proper utf8 handling
				const u8 byte = static_cast<u8>(c);
				bytes_written += writer.write(Slice<u8 const>{ &byte, 1 });
			}
		}
		if (!found_arg) {
			GRIZZLY_PANIC("No arguments provided");
		}
		return bytes_written;
	}

	template <typename T, typename... Args>
	u32 format_one(Writer& writer, const StringView& fmt, const T& arg, const Args&... args) {
		u32 bytes_written = 0;
		for (auto iter = fmt.chars(); iter; ++iter) {
			const auto c = *iter;
			if (c == '{') {
				++iter;
				if (auto next = iter) {
					const auto b = *next;

					if (b == '{') {
						const u8 byte = '{';
						bytes_written += writer.write(Slice<u8 const>{ &byte, 1 });
					} else if (b == '}') {
						Formatter<T> formatter;
						bytes_written += formatter.format(writer, arg);
						return bytes_written +
							   format_one(writer, fmt.substring(iter.byte_offset(), fmt.len()), args...);
					} else {
						GRIZZLY_PANIC("Invalid format string");
					}
				}
			} else {
				// TODO: Proper utf8 handling
				const u8 byte = static_cast<u8>(c);
				bytes_written += writer.write(Slice<u8 const>{ &byte, 1 });
			}
		}
		GRIZZLY_PANIC("Unmatched braces in format string");
		return 0;
	}

	template <typename... Args>
	u32 format(Writer& writer, const StringView& fmt, const Args&... args) {
		return format_one(writer, fmt, args...);
	}
} // namespace Grizzly::Core

namespace Grizzly {
	template <>
	struct Formatter<u8> {
		void parse(const StringView& fmt) {}

		usize format(Core::Writer& writer, u8 value) {
			char buffer[32];
			const auto written = std::snprintf(buffer, 32, "%d", value);
			return writer.write(Core::Slice<u8 const>((const u8*)buffer, written));
		}
	};

	template <>
	struct Formatter<u16> {
		void parse(const StringView& fmt) {}

		usize format(Core::Writer& writer, u16 value) {
			char buffer[32];
			const auto written = std::snprintf(buffer, 32, "%d", value);
			return writer.write(Core::Slice<u8 const>((const u8*)buffer, written));
		}
	};

	template <>
	struct Formatter<u32> {
		void parse(const StringView& fmt) {}

		usize format(Core::Writer& writer, u32 value) {
			char buffer[32];
			const auto written = std::snprintf(buffer, 32, "%d", value);
			return writer.write(Core::Slice<u8 const>((const u8*)buffer, written));
		}
	};

	template <>
	struct Formatter<u64> {
		void parse(const StringView& fmt) {}

		usize format(Core::Writer& writer, u64 value) {
			char buffer[32];
			const auto written = std::snprintf(buffer, 32, "%lld", value);
			return writer.write(Core::Slice<u8 const>((const u8*)buffer, written));
		}
	};

	template <>
	struct Formatter<i8> {
		void parse(const StringView& fmt) {}

		usize format(Core::Writer& writer, i8 value) {
			char buffer[32];
			const auto written = std::snprintf(buffer, 32, "%d", value);
			return writer.write(Core::Slice<u8 const>((const u8*)buffer, written));
		}
	};

	template <>
	struct Formatter<i16> {
		void parse(const StringView& fmt) {}

		usize format(Core::Writer& writer, i16 value) {
			char buffer[32];
			const auto written = std::snprintf(buffer, 32, "%d", value);
			return writer.write(Core::Slice<u8 const>((const u8*)buffer, written));
		}
	};

	template <>
	struct Formatter<i32> {
		void parse(const StringView& fmt) {}

		usize format(Core::Writer& writer, i32 value) {
			char buffer[32];
			const auto written = std::snprintf(buffer, 32, "%d", value);
			return writer.write(Core::Slice<u8 const>((const u8*)buffer, written));
		}
	};

	template <>
	struct Formatter<i64> {
		void parse(const StringView& fmt) {}

		usize format(Core::Writer& writer, i64 value) {
			char buffer[32];
			const auto written = std::snprintf(buffer, 32, "%lld", value);
			return writer.write(Core::Slice<u8 const>((const u8*)buffer, written));
		}
	};

	template <>
	struct Formatter<f32> {
		void parse(const StringView& fmt) {}

		usize format(Core::Writer& writer, f32 value) {
			char buffer[32];
			const auto written = std::snprintf(buffer, 32, "%f", value);
			return writer.write(Core::Slice<u8 const>((const u8*)buffer, written));
		}
	};

	template <>
	struct Formatter<f64> {
		void parse(const StringView& fmt) {}

		usize format(Core::Writer& writer, f64 value) {
			char buffer[32];
			const auto written = std::snprintf(buffer, 32, "%f", value);
			return writer.write(Core::Slice<u8 const>((const u8*)buffer, written));
		}
	};

	template <>
	struct Formatter<StringView> {
		void parse(const StringView& fmt) {}

		usize format(Core::Writer& writer, const StringView& value) {
			return writer.write(Slice<u8 const>{ (const u8*)*value, value.len() });
		}
	};
} // namespace Grizzly
