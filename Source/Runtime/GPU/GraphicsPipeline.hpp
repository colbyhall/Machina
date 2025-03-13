/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * this software is released under the mit license.
 */

#pragma once

#include <Core/Containers/Array.hpp>
#include <Core/Containers/Shared.hpp>
#include <GPU/Shader.hpp>
#include <GPU/Texture.hpp>

namespace Forge::GPU {
	class GraphicsPipeline : public ArcFromThis<GraphicsPipeline> {
	public:
		enum class DrawMode : u8 {
			Fill,
			Line,
			Point,
		};

		enum class CullMode : u8 {
			None,
			Front,
			Back,
		};

		enum class Winding : u8 {
			Clockwise,
			CounterClockwise,
		};

		enum class CompareOp : u8 {
			/** False */
			Never,

			/** `A < B` */
			Less,

			/** `A == B` */
			Equal,

			/** `A != B` */
			NotEqual,

			/** `A < B || A == B` */
			LessOrEqual,

			/** `A > B` */
			Greater,

			/** `A > B || A == B` */
			GreaterOrEqual,

			/** `true` */
			Always,
		};

		enum class BlendOp : u8 {
			Add,
			Subtract,
			ReverseSubtract,
			Min,
			Max,
		};

		enum class BlendFactor : u8 {
			Zero,
			One,

			SrcColor,
			OneMinusSrcColor,
			DstColor,
			OneMinusDstColor,

			SrcAlpha,
			OneMinusSrcAlpha,
		};

		enum class ColorComponents : u8 {
			R = (1 << 0),
			G = (1 << 1),
			B = (1 << 2),
			A = (1 << 3),
			RGBA = R | G | B | A,
		};

		struct ColorAttachment {
			Format format;

			bool blend_enabled = false;

			BlendFactor src_color_blend_factor = BlendFactor::One;
			BlendFactor dst_color_blend_factor = BlendFactor::One;
			BlendOp color_blend_op = BlendOp::Add;

			BlendFactor src_alpha_blend_factor = BlendFactor::One;
			BlendFactor dst_alpha_blend_factor = BlendFactor::One;
			BlendOp alpha_blend_op = BlendOp::Add;

			ColorComponents color_write_mask = ColorComponents::RGBA;
		};

		struct DepthAttachment {
			Format format;

			bool depth_test = false;
			bool depth_write = false;
			CompareOp depth_compare = CompareOp::Always;
		};

		struct CreateInfo {
			VertexShader const& vertex_shader;
			FragmentShader const& fragment_shader;

			Array<ColorAttachment, InlineAllocator<8>> color_attachments;
			Option<DepthAttachment> depth_attachment = nullopt;

			DrawMode draw_mode = DrawMode::Fill;
			f32 line_width = 1.f;
			CullMode cull_mode = CullMode::None;
			Winding winding = Winding::Clockwise;
		};

		GraphicsPipeline(const CreateInfo& create_info)
			: m_vertex_shader(create_info.vertex_shader.to_shared())
			, m_fragment_shader(create_info.fragment_shader.to_shared())
			, m_color_attachments(create_info.color_attachments)
			, m_depth_attachment(create_info.depth_attachment)
			, m_draw_mode(create_info.draw_mode)
			, m_line_width(create_info.line_width)
			, m_cull_mode(create_info.cull_mode) {}
		virtual ~GraphicsPipeline() {}

		FORGE_ALWAYS_INLINE VertexShader const& vertex_shader() const { return *m_vertex_shader; }
		FORGE_ALWAYS_INLINE FragmentShader const& fragment_shader() const { return *m_fragment_shader; }
		FORGE_ALWAYS_INLINE Slice<ColorAttachment const> color_attachments() const {
			return m_color_attachments.as_const_slice();
		}
		FORGE_ALWAYS_INLINE Option<DepthAttachment> depth_attachment() const { return m_depth_attachment; }
		FORGE_ALWAYS_INLINE DrawMode draw_mode() const { return m_draw_mode; }
		FORGE_ALWAYS_INLINE f32 line_width() const { return m_line_width; }
		FORGE_ALWAYS_INLINE CullMode cull_mode() const { return m_cull_mode; }

	protected:
		Arc<VertexShader> m_vertex_shader;
		Arc<FragmentShader> m_fragment_shader;

		Array<ColorAttachment, InlineAllocator<8>> m_color_attachments;
		Option<DepthAttachment> m_depth_attachment;

		DrawMode m_draw_mode;
		f32 m_line_width;
		CullMode m_cull_mode;
	};
	FORGE_ENUM_CLASS_BITFIELD(GraphicsPipeline::ColorComponents);
} // namespace Forge::GPU
