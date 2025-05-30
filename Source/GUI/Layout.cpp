/**
 * Copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <GUI/Layout.hpp>

namespace Mach::GUI {
	Layout::Layout(Direction direction, const Bounds& bounds) : m_direction(direction), m_bounds(bounds) {
		switch (m_direction) {
		case Direction::UpToDown:
			m_cursor = m_bounds.max.y;
			break;
		case Direction::DownToUp:
			m_cursor = m_bounds.min.y;
			break;
		case Direction::LeftToRight:
			m_cursor = m_bounds.min.x;
			break;
		case Direction::RightToLeft:
			m_cursor = m_bounds.max.x;
			break;
		}
	}

	/**
	 * Helpers to align a size within a bounds.
	 *
	 * @see Layout::allocate_space
	 *
	 * @param size - The size to align.
	 * @param bounds - The bounds to align the size within.
	 * @param horizontal - The horizontal alignment.
	 * @param vertical - The vertical alignment.
	 * @return Bounds - The aligned bounds.
	 */
	static Bounds
	align_in_bounds(const Size& size, const Bounds& bounds, Layout::Alignment horizontal, Layout::Alignment vertical) {
		// Determine the center of the result bounds based on the alignment
		Point center;
		switch (horizontal) {
		case Layout::Alignment::Min:
			center.x = bounds.min.x + size.x;
			break;
		case Layout::Alignment::Center:
			center.x = bounds.min.x + bounds.width() * 0.5f;
			break;
		case Layout::Alignment::Max:
			center.x = bounds.max.x - size.x;
			break;
		}
		switch (vertical) {
		case Layout::Alignment::Min:
			center.y = bounds.min.y + size.y;
			break;
		case Layout::Alignment::Center:
			center.y = bounds.min.y + bounds.height() * 0.5f;
			break;
		case Layout::Alignment::Max:
			center.y = bounds.max.y - size.x;
			break;
		}

		// Size could be larger than the 'bounds', so we intersect the 'bounds'
		// with 'at_center" to ensure the result is within 'bounds;.
		const auto at_center = Bounds::from_center(center, size);
		return at_center.intersects(bounds).unwrap();
	}

	Bounds Layout::allocate_space(const Size& size) {
		auto space = m_bounds;

		// Move the cursor to the next position based on the direction. Ensure
		// the cursor does not go out of bounds.
		const auto original = m_cursor;
		switch (m_direction) {
		case Direction::DownToUp: {
			m_cursor = Math::min(m_cursor + size.y, m_bounds.max.y);
			space.max.y = m_cursor;
			space.min.y = original;
			return align_in_bounds(size, space, Alignment::Min, Alignment::Center);
		} break;
		case Direction::UpToDown: {
			m_cursor = Math::max(m_cursor - size.y, m_bounds.min.y);
			space.max.y = original;
			space.min.y = m_cursor;
			return align_in_bounds(size, space, Alignment::Min, Alignment::Center);
		} break;
		case Direction::LeftToRight: {
			m_cursor = Math::min(m_cursor + size.x, m_bounds.max.x);
			space.max.x = m_cursor;
			space.min.x = original;
			return align_in_bounds(size, space, Alignment::Center, Alignment::Center);
		} break;
		case Direction::RightToLeft: {
			m_cursor = Math::max(m_cursor - size.x, m_bounds.min.x);
			space.max.y = original;
			space.min.y = m_cursor;
			return align_in_bounds(size, space, Alignment::Center, Alignment::Center);
		} break;
		}

		MACH_UNREACHABLE;
		// C++ requires a return statement, but we should never reach this point.
		return Bounds{};
	}

	Real Layout::available_space() const {
		switch (m_direction) {
		case Direction::UpToDown:
			return m_cursor - m_bounds.min.y;
		case Direction::DownToUp:
			return m_bounds.max.y - m_cursor;
		case Direction::LeftToRight:
			return m_bounds.max.x - m_cursor;
		case Direction::RightToLeft:
			return m_cursor - m_bounds.min.x;
		}
		MACH_UNREACHABLE;
		return 0;
	}
} // namespace Mach::GUI
