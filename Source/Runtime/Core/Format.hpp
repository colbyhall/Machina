/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Containers/Function.hpp>
#include <Core/Containers/String.hpp>
#include <Core/Debug/Assertions.hpp>
#include <Core/IO/Writer.hpp>

namespace Grizzly {
	template <typename T>
	struct Formatter;
} // namespace Grizzly

namespace Grizzly::Core {
	u32 format_lambda(Writer& writer, const StringView& fmt, Option<FunctionRef<u32(StringView)>> f);

	u32 format_one(Writer& writer, const StringView& fmt);

	template <typename T>
	u32 format_one(Writer& writer, const StringView& fmt, const T& arg) {
		return format_lambda(writer, fmt, FunctionRef<u32(StringView)>{ [&](StringView rest) -> u32 {
								 Formatter<T> formatter;
								 const auto bytes_written = formatter.format(writer, arg);
								 return bytes_written + format_lambda(writer, rest, nullopt);
							 } });
	}

	template <typename T, typename... Args>
	u32 format_one(Writer& writer, const StringView& fmt, const T& arg, const Args&... args) {
		return format_lambda(writer, fmt, FunctionRef<u32(StringView)>{ [&](StringView rest) -> u32 {
								 Formatter<T> formatter;
								 const auto bytes_written = formatter.format(writer, arg);
								 return bytes_written + format_one(writer, rest, args...);
							 } });
	}

	template <typename... Args>
	u32 format(Writer& writer, const StringView& fmt, const Args&... args) {
		return format_one(writer, fmt, args...);
	}

	usize print_unsigned_integer(Writer& writer, u64 value, u8 base = 10);
	usize print_signed_integer(Writer& writer, i64 value, u8 base = 10);
	usize print_float(Writer& writer, f32 value);
	usize print_double(Writer& writer, f64 value);

} // namespace Grizzly::Core

namespace Grizzly {
	template <>
	struct Formatter<u8> {
		void parse(const StringView& fmt) {}

		usize format(Core::Writer& writer, u8 value) { return Core::print_unsigned_integer(writer, value); }
	};

	template <>
	struct Formatter<u16> {
		void parse(const StringView& fmt) {}

		usize format(Core::Writer& writer, u16 value) { return Core::print_unsigned_integer(writer, value); }
	};

	template <>
	struct Formatter<u32> {
		void parse(const StringView& fmt) {}

		usize format(Core::Writer& writer, u32 value) { return Core::print_unsigned_integer(writer, value); }
	};

	template <>
	struct Formatter<u64> {
		void parse(const StringView& fmt) {}

		usize format(Core::Writer& writer, u64 value) { return Core::print_unsigned_integer(writer, value); }
	};

	template <>
	struct Formatter<i8> {
		void parse(const StringView& fmt) {}

		usize format(Core::Writer& writer, i8 value) { return Core::print_signed_integer(writer, value); }
	};

	template <>
	struct Formatter<i16> {
		void parse(const StringView& fmt) {}

		usize format(Core::Writer& writer, i16 value) { return Core::print_signed_integer(writer, value); }
	};

	template <>
	struct Formatter<i32> {
		void parse(const StringView& fmt) {}

		usize format(Core::Writer& writer, i32 value) { return Core::print_signed_integer(writer, value); }
	};

	template <>
	struct Formatter<i64> {
		void parse(const StringView& fmt) {}

		usize format(Core::Writer& writer, i64 value) { return Core::print_signed_integer(writer, value); }
	};

	template <>
	struct Formatter<f32> {
		void parse(const StringView& fmt) {}

		usize format(Core::Writer& writer, f32 value) { return Core::print_float(writer, value); }
	};

	template <>
	struct Formatter<f64> {
		void parse(const StringView& fmt) {}

		usize format(Core::Writer& writer, f64 value) { return Core::print_double(writer, value); }
	};

	template <>
	struct Formatter<StringView> {
		void parse(const StringView& fmt) {}

		usize format(Core::Writer& writer, const StringView& value) {
			return writer.write(Slice<u8 const>{ (const u8*)*value, value.len() });
		}
	};

	template <>
	struct Formatter<String> {
		void parse(const String& fmt) {}

		usize format(Core::Writer& writer, const String& value) {
			return writer.write(Slice<u8 const>{ (const u8*)*value, value.len() });
		}
	};
} // namespace Grizzly
