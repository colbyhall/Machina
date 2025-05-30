/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <GUI/Math.hpp>

namespace Mach::GUI {
	const Color Color::white = Color(0xFFFFFFFF);
	const Color Color::black = Color(0x000000FF);
	const Color Color::transparent = Color(0x00000000);

	const Color Color::red = Color(0xFF0000FF);
	const Color Color::green = Color(0x00FF00FF);
	const Color Color::blue = Color(0x0000FFFF);

	const Color Color::cyan = Color(0x00FFFFFF);
	const Color Color::magenta = Color(0xFF00FFFF);
	const Color Color::yellow = Color(0xFFFF00FF);

	Bounds Bounds::from_center(const Point& c, const Size& size) {
		const auto half_size = size * 0.5f;
		return Bounds{
			.min = c - half_size,
			.max = c + half_size,
		};
	}

	bool Bounds::contains(const Point& point) const {
		return point.x >= min.x && point.x <= max.x && point.y >= min.y && point.y <= max.y;
	}

	Option<Bounds> Bounds::intersects(const Bounds& other) const {
		if (max.x < other.min.x || min.x > other.max.x || max.y < other.min.y || min.y > other.max.y) {
			return nullopt;
		}

		const auto x_min = Math::max(min.x, other.min.x);
		const auto y_min = Math::max(min.y, other.min.y);
		const auto x_max = Math::min(max.x, other.max.x);
		const auto y_max = Math::min(max.y, other.max.y);

		return Bounds{
			.min = Point(x_min, y_min),
			.max = Point(x_max, y_max),
		};
	}
} // namespace Mach::GUI
