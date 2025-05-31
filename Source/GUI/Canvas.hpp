/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Containers/Array.hpp>
#include <Core/Containers/StringView.hpp>
#include <Core/Containers/Variant.hpp>
#include <GPU/Resource.hpp>
#include <GUI/Math.hpp>

namespace Mach::GUI {
	struct Mesh {
		struct Vertex {
			Point position;
			Vector2<f32> uv;
			GPU::BindlessIndex texture;
			Color color;
			Bounds clip;
		};
		using Index = u32;

		Array<Vertex> vertices;
		Array<Index> indices;
	};

	class Shape {
	public:
		struct Text {
			StringView string;
		};

		struct Rect {};

		struct Line {
			Real width;
		};
		using Variant = Core::Variant<Text, Rect, Line>;

		static Shape text(StringView s, const Bounds& bounds, Color color);
		static Shape rect(const Bounds& bounds, Color color);
		static Shape line(const Point& start, const Point& end, Real width, Color color);

		void tesselate(Mesh& mesh, const Bounds& clip) const;

	private:
		explicit Shape(Shape::Variant&& variant, const Bounds& bounds, Color color)
			: m_variant(Mach::move(variant))
			, m_bounds(bounds)
			, m_color(color) {}

		Shape::Variant m_variant;
		Bounds m_bounds;
		Color m_color;
	};

	class Canvas {
	public:
		explicit Canvas() = default;

		MACH_ALWAYS_INLINE void set_clip(Option<Bounds> clip) { m_active_clip = clip; }

		usize push(const Shape& shape);
		void insert(usize index, const Shape& shape);

		Mesh tesselate() const;

	private:
		Option<Bounds> m_active_clip;

		struct ShapeAndClip {
			Bounds clip;
			Shape shape;
		};
		Array<ShapeAndClip> m_shapes;
	};
} // namespace Mach::GUI
