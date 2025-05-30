/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <GUI/Canvas.hpp>

namespace Mach::GUI {
	Shape Shape::text(StringView s, const Bounds& bounds, Color color) {
		return Shape(MyVariant(Text{ .string = s }), bounds, color);
	}

	Shape Shape::rect(const Bounds& bounds, Color color) { return Shape(MyVariant(Rect{}), bounds, color); }

	Shape Shape::line(const Point& start, const Point& end, Real width, Color color) {
		return Shape(
			MyVariant(Line{ .width = width }),
			Bounds{
				.min = start,
				.max = end,
			},
			color);
	}

	void Shape::tesselate(Mesh& mesh, const Bounds& clip) const {
		if (m_variant.is<Rect>()) {
			// Insert the four corners of the rect into the vertex buffer
			// and keep track of what index we inserted them in
			const auto bottom_left = mesh.vertices.push(Mesh::Vertex{
				.position = m_bounds.min,
				.uv = {},
				.texture = 0,
				.color = m_color,
				.clip = clip,
			});
			const auto bottom_right = mesh.vertices.push(Mesh::Vertex{
				.position = Point(m_bounds.max.x, m_bounds.min.x),
				.uv = {},
				.texture = 0,
				.color = m_color,
				.clip = clip,
			});
			const auto top_left = mesh.vertices.push(Mesh::Vertex{
				.position = Point(m_bounds.min.x, m_bounds.max.y),
				.uv = {},
				.texture = 0,
				.color = m_color,
				.clip = clip,
			});
			const auto top_right = mesh.vertices.push(Mesh::Vertex{
				.position = m_bounds.max,
				.uv = {},
				.texture = 0,
				.color = m_color,
				.clip = clip,
			});

			// Define the first triangle based on the vertex indices
			mesh.indices.push(static_cast<Mesh::Index>(bottom_left));
			mesh.indices.push(static_cast<Mesh::Index>(top_left));
			mesh.indices.push(static_cast<Mesh::Index>(top_right));

			// Define the second triangle based on the vertex indices
			mesh.indices.push(static_cast<Mesh::Index>(bottom_left));
			mesh.indices.push(static_cast<Mesh::Index>(top_right));
			mesh.indices.push(static_cast<Mesh::Index>(bottom_right));
		} else {
			MACH_UNIMPLEMENTED;
		}
	}

	usize Canvas::push(const Shape& shape) {
		const auto clip = m_active_clip.unwrap_or(Bounds{ .min = {}, .max = {} });
		return m_shapes.push({ .clip = clip, .shape = shape });
	}

	void Canvas::insert(usize index, const Shape& shape) {
		const auto clip = m_active_clip.unwrap_or(Bounds{ .min = {}, .max = {} });
		m_shapes.insert(index, { .clip = clip, .shape = shape });
	}

	Mesh Canvas::tesselate() const {
		Mesh result;
		result.vertices.reserve(m_shapes.len() * 4);
		result.indices.reserve(m_shapes.len() * 4);

		for (ShapeAndClip const& shape_and_clip : m_shapes) {
			shape_and_clip.shape.tesselate(result, shape_and_clip.clip);
		}

		return result;
	}

} // namespace Mach::GUI
