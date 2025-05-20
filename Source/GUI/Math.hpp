/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Math/Vector2.hpp>

namespace Forge::GUI {
	using Real = f32;
	using Point = Vector2<Real>;
	using Size = Vector2<Real>;

	class Color {
	public:
		explicit constexpr Color(u32 rgba) : m_rgba(rgba) {}

		static const Color white;
		static const Color black;
		static const Color transparent;

		static const Color red;
		static const Color green;
		static const Color blue;

		static const Color cyan;
		static const Color magenta;
		static const Color yellow;

		FORGE_ALWAYS_INLINE explicit operator u32() const { return m_rgba; }

	private:
		u32 m_rgba;
	};

	struct Bounds {
		Point min;
		Point max;

		/**
		 * Creates a new bounds from the given center point and size.
		 *
		 * @param c - The center point of the bounds.
		 * @param size - The size of the bounds.
		 * @return Bounds - The new bounds.
		 */
		static Bounds from_center(const Point& c, const Size& size);

		/**
		 * Checks if the given point is within the bounds.
		 *
		 * @param point - The point to check.
		 * @return bool - True if the point is within the bounds, false otherwise.
		 */
		bool contains(const Point& point) const;

		/**
		 * Returns the smallest bounding box that contains both this bounds and the other bounds.
		 *
		 * @param other - The other bounds to check for intersection with.
		 * @return Option<Bounds> - The intersection of the two bounds, or nullopt if they do not intersect.
		 */
		Option<Bounds> intersects(const Bounds& other) const;

		FORGE_ALWAYS_INLINE Real width() const { return max.x - min.x; }
		FORGE_ALWAYS_INLINE Real height() const { return max.y - min.y; }
		FORGE_ALWAYS_INLINE Size size() const { return Size(width(), height()); }
	};
} // namespace Forge::GUI
