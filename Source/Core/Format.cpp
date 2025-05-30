/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Format.hpp>
#include <cstdio>

namespace Mach::Core {
	struct ANSIIdentifier {
		enum Code : u8 {
			Default = 0,
			Bold = 1,
			Dim = 2,
			Italic = 3,
			Underline = 4,
			Blink = 5,
			Inverse = 7,
			Hidden = 8,
			Strikethrough = 9,
			Black = 30,
			Red = 31,
			Green = 32,
			Yellow = 33,
			Blue = 34,
			Magenta = 35,
			Cyan = 36,
			White = 37,
		};

		StringView identifier;
		Code code;
	};
	static const Array<ANSIIdentifier, InlineAllocator<32>> g_ansi_identifiers = {
		{
			.identifier = u8"default"_sv,
			.code = ANSIIdentifier::Default,
		},
		{
			.identifier = u8"bold"_sv,
			.code = ANSIIdentifier::Bold,
		},
		{
			.identifier = u8"dim"_sv,
			.code = ANSIIdentifier::Dim,
		},
		{
			.identifier = u8"italic"_sv,
			.code = ANSIIdentifier::Italic,
		},
		{
			.identifier = u8"underline"_sv,
			.code = ANSIIdentifier::Underline,
		},
		{
			.identifier = u8"blink"_sv,
			.code = ANSIIdentifier::Blink,
		},
		{
			.identifier = u8"inverse"_sv,
			.code = ANSIIdentifier::Inverse,
		},
		{
			.identifier = u8"hidden"_sv,
			.code = ANSIIdentifier::Hidden,
		},
		{
			.identifier = u8"strikethrough"_sv,
			.code = ANSIIdentifier::Strikethrough,
		},
		{
			.identifier = u8"black"_sv,
			.code = ANSIIdentifier::Black,
		},
		{
			.identifier = u8"red"_sv,
			.code = ANSIIdentifier::Red,
		},
		{
			.identifier = u8"green"_sv,
			.code = ANSIIdentifier::Green,
		},
		{
			.identifier = u8"yellow"_sv,
			.code = ANSIIdentifier::Yellow,
		},
		{
			.identifier = u8"blue"_sv,
			.code = ANSIIdentifier::Blue,
		},
		{
			.identifier = u8"magenta"_sv,
			.code = ANSIIdentifier::Magenta,
		},
		{
			.identifier = u8"cyan"_sv,
			.code = ANSIIdentifier::Cyan,
		},
		{
			.identifier = u8"white"_sv,
			.code = ANSIIdentifier::White,
		},
	};

	void Formatter::format_lambda(const StringView& fmt, Option<FunctionRef<void(StringView)>> f) {
		usize base = 0;
		for (auto iter = fmt.chars(); iter; ++iter) {
			auto c = *iter;
			if (c == '{') {
				if (base != iter.byte_offset()) {
					m_bytes_written += m_writer.write(fmt.substring(base, iter.byte_offset()));
				}

				++iter;

				const auto start = iter.index();
				for (; iter; ++iter) {
					c = *iter;

					if (c == '{') {
						const u8 byte = '{';
						m_bytes_written += m_writer.write(Slice<u8 const>{ &byte, 1 });
					} else if (c == '}') {
						base = iter.byte_offset() + 1;

						const auto substring = fmt.substring(start, iter.byte_offset());
						if (substring.len() > 0) {
							if (!m_accepts_ansi) break;

							bool found = false;
							for (auto const& identifier : g_ansi_identifiers) {
								if (substring == identifier.identifier) {
									const StringView prefix = u8"\033["_sv;
									m_bytes_written += m_writer.write(prefix);
									TypeFormatter<u8> formatter;
									m_bytes_written += formatter.format(m_writer, static_cast<u8>(identifier.code));
									const StringView postfix = u8"m"_sv;
									m_bytes_written += m_writer.write(postfix);
									found = true;
									break;
								}
							}
							if (!found) {
								MACH_PANIC("Unknown ANSI identifier");
							}
							break;
						} else if (f) {
							const auto rest = fmt.substring(base, fmt.len());
							(f.as_ref().unwrap())(rest);
							return;
						} else {
							MACH_PANIC("Not enough arguments were provided");
						}
					}
				}
			}
		}

		MACH_ASSERT(!f.is_set(), "Too many arguments were provided");

		if (base != fmt.len()) {
			m_bytes_written += m_writer.write(fmt.substring(base, fmt.len()));
		}
	}

	void Formatter::format_one(const StringView& fmt) { format_lambda(fmt, nullopt); }

	usize print_unsigned_integer(Writer& writer, u64 value, u8 base) {
		MACH_UNUSED(base);

		char buffer[32];
		const auto written = std::snprintf(buffer, 32, "%llu", value);
		return writer.write(Slice<u8 const>{ (const u8*)buffer, static_cast<usize>(written) });
	}
	usize print_signed_integer(Writer& writer, i64 value, u8 base) {
		MACH_UNUSED(base);

		char buffer[32];
		const auto written = std::snprintf(buffer, 32, "%lld", value);
		return writer.write(Slice<u8 const>{ (const u8*)buffer, static_cast<usize>(written) });
	}
	usize print_float(Writer& writer, f32 value) {
		char buffer[32];
		const auto written = std::snprintf(buffer, 32, "%f", value);
		return writer.write(Slice<u8 const>{ (const u8*)buffer, static_cast<usize>(written) });
	}
	usize print_double(Writer& writer, f64 value) {
		char buffer[32];
		const auto written = std::snprintf(buffer, 32, "%f", value);
		return writer.write(Slice<u8 const>{ (const u8*)buffer, static_cast<usize>(written) });
	}
} // namespace Mach::Core
