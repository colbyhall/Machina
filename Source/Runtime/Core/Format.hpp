/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Containers/Function.hpp>
#include <Core/Containers/StringView.hpp>
#include <Core/Debug/Assertions.hpp>
#include <Core/IO/Writer.hpp>

namespace Grizzly {
	template <typename T>
	struct TypeFormatter;
} // namespace Grizzly

namespace Grizzly::Core {
	class Formatter {
	public:
		explicit Formatter(Writer& writer, bool accepts_ansi = true)
			: m_accepts_ansi{ accepts_ansi }
			, m_writer{ writer } {}

		template <typename... Args>
		Formatter& format(const StringView& fmt, const Args&... args) {
			format_one(fmt, args...);
			return *this;
		}

		GRIZZLY_NO_DISCARD GRIZZLY_ALWAYS_INLINE usize bytes_written() const { return m_bytes_written; }

	private:
		void format_lambda(const StringView& fmt, Option<FunctionRef<void(StringView)>> f);

		void format_one(const StringView& fmt);

		template <typename T>
		void format_one(const StringView& fmt, const T& arg) {
			format_lambda(fmt, FunctionRef<void(StringView)>{ [&](StringView rest) {
							  TypeFormatter<T> formatter;
							  const auto bytes_written = formatter.format(m_writer, arg);
							  m_bytes_written += bytes_written;
							  format_lambda(rest, nullopt);
						  } });
		}

		template <typename T, typename... Args>
		void format_one(const StringView& fmt, const T& arg, const Args&... args) {
			format_lambda(fmt, FunctionRef<void(StringView)>{ [&](StringView rest) {
							  TypeFormatter<T> formatter;
							  const auto bytes_written = formatter.format(m_writer, arg);
							  m_bytes_written += bytes_written;
							  format_one(rest, args...);
						  } });
		}

		bool m_accepts_ansi;
		Writer& m_writer;
		usize m_bytes_written = 0;
	};

	usize print_unsigned_integer(Writer& writer, u64 value, u8 base = 10);
	usize print_signed_integer(Writer& writer, i64 value, u8 base = 10);
	usize print_float(Writer& writer, f32 value);
	usize print_double(Writer& writer, f64 value);

} // namespace Grizzly::Core

namespace Grizzly {
	template <>
	struct TypeFormatter<u8> {
		void parse(const StringView& fmt) {}

		usize format(Core::Writer& writer, u8 value) { return Core::print_unsigned_integer(writer, value); }
	};

	template <>
	struct TypeFormatter<u16> {
		void parse(const StringView& fmt) {}

		usize format(Core::Writer& writer, u16 value) { return Core::print_unsigned_integer(writer, value); }
	};

	template <>
	struct TypeFormatter<u32> {
		void parse(const StringView& fmt) {}

		usize format(Core::Writer& writer, u32 value) { return Core::print_unsigned_integer(writer, value); }
	};

	template <>
	struct TypeFormatter<u64> {
		void parse(const StringView& fmt) {}

		usize format(Core::Writer& writer, u64 value) { return Core::print_unsigned_integer(writer, value); }
	};

	template <>
	struct TypeFormatter<i8> {
		void parse(const StringView& fmt) {}

		usize format(Core::Writer& writer, i8 value) { return Core::print_signed_integer(writer, value); }
	};

	template <>
	struct TypeFormatter<i16> {
		void parse(const StringView& fmt) {}

		usize format(Core::Writer& writer, i16 value) { return Core::print_signed_integer(writer, value); }
	};

	template <>
	struct TypeFormatter<i32> {
		void parse(const StringView& fmt) {}

		usize format(Core::Writer& writer, i32 value) { return Core::print_signed_integer(writer, value); }
	};

	template <>
	struct TypeFormatter<i64> {
		void parse(const StringView& fmt) {}

		usize format(Core::Writer& writer, i64 value) { return Core::print_signed_integer(writer, value); }
	};

	template <>
	struct TypeFormatter<f32> {
		void parse(const StringView& fmt) {}

		usize format(Core::Writer& writer, f32 value) { return Core::print_float(writer, value); }
	};

	template <>
	struct TypeFormatter<f64> {
		void parse(const StringView& fmt) {}

		usize format(Core::Writer& writer, f64 value) { return Core::print_double(writer, value); }
	};

	template <>
	struct TypeFormatter<StringView> {
		void parse(const StringView& fmt) {}

		usize format(Core::Writer& writer, const StringView& value) {
			return writer.write(Slice<u8 const>{ (const u8*)*value, value.len() });
		}
	};
} // namespace Grizzly
