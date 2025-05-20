/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <GUI/Math.hpp>

namespace Forge::GUI {
	class Layout {
	public:
		/** Defines the direction elements will be layed out. */
		enum class Direction : u8 {
			DownToUp,
			UpToDown,
			LeftToRight,
			RightToLeft,
		};
		enum class Alignment : u8 { Min, Center, Max };

		explicit Layout(Direction direction, const Bounds& bounds);

		/**
		 * Given an element, allocate space for it in the layout within bounds at the cursor.
		 *
		 * @param size - The size of the element to allocate space for.
		 * @return Bounds - The bounds of the allocated space. If the size is larger than the available space, the
		 * bounds will be clamped to the available space.
		 */
		Bounds allocate_space(const Size& size);

		/** Returns the available space from the cursor to the bounds along the current direction. */
		Real available_space() const;

	private:
		Direction m_direction;
		Bounds m_bounds;
		Real m_cursor;
	};
} // namespace Forge::GUI
