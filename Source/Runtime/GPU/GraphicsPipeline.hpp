/**
 * copyright (c) 2024 colby hall <me@cobeh.com>
 *
 * this software is released under the mit license.
 */

#pragma once

#include <Core/Containers/Array.hpp>
#include <Core/Containers/Shared.hpp>
#include <GPU/Shader.hpp>
#include <GPU/Texture.hpp>

namespace Grizzly::GPU {
	class GraphicsPipeline : public SharedFromThis<GraphicsPipeline> {
	public:
		enum class DrawMode : u8 {
			Fill,
			Line,
			Point,
		};

		enum class CullMode : u8 {
			None = 0,
			Font = (1 << 0),
			Back = (1 << 1),
		};

		enum class CompareOp : u8 {
			/** False */
			Never,

			/** `A < B` */
			Less,

			/** `A == B` */
			Equal,

			/** `A < B || A == B` */
			LessOrEqual,

			/** `A > B` */
			Greater,

			/** `A != B` */
			NotEqual,

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

		struct CreateInfo {
			Shared<VertexShader> vertex_shader;
			Shared<PixelShader> pixel_shader;

			Array<Texture::Format, InlineAllocator<8>> color_attachments;
			Option<Texture::Format> depth_attachment = nullopt;

			DrawMode draw_mode = DrawMode::Fill;
			f32 line_width = 1.f;
			CullMode cull_mode = CullMode::None;

			bool blend_enabled = false;

			BlendFactor src_color_blend_factor = BlendFactor::One;
			BlendFactor dst_color_blend_factor = BlendFactor::One;
			BlendOp color_blend_op = BlendOp::Add;

			BlendFactor src_alpha_blend_factor = BlendFactor::One;
			BlendFactor dst_alpha_blend_factor = BlendFactor::One;
			BlendOp alpha_blend_op = BlendOp::Add;

			bool depth_test = false;
			bool depth_write = false;
			CompareOp depth_compare = CompareOp::Always;
		};

		virtual ~GraphicsPipeline() {}

		GRIZZLY_ALWAYS_INLINE VertexShader const& vertex_shader() const { return *m_vertex_shader; }
		GRIZZLY_ALWAYS_INLINE PixelShader const& pixel_shader() const { return *m_pixel_shader; }
		GRIZZLY_ALWAYS_INLINE Slice<Texture::Format const> color_attachments() const {
			return m_color_attachments.as_const_slice();
		}
		GRIZZLY_ALWAYS_INLINE Option<Texture::Format> depth_attachment() const { return m_depth_attachment; }
		GRIZZLY_ALWAYS_INLINE DrawMode draw_mode() const { return m_draw_mode; }
		GRIZZLY_ALWAYS_INLINE f32 line_width() const { return m_line_width; }
		GRIZZLY_ALWAYS_INLINE CullMode cull_mode() const { return m_cull_mode; }

		GRIZZLY_ALWAYS_INLINE bool is_blend_enabled() const { return m_blend_enabled; }
		GRIZZLY_ALWAYS_INLINE BlendFactor src_color_blend_factor() const { return m_src_color_blend_factor; }
		GRIZZLY_ALWAYS_INLINE BlendFactor dst_color_blend_factor() const { return m_dst_color_blend_factor; }
		GRIZZLY_ALWAYS_INLINE BlendOp color_blend_op() const { return m_color_blend_op; }
		GRIZZLY_ALWAYS_INLINE BlendFactor src_alpha_blend_factor() const { return m_src_alpha_blend_factor; }
		GRIZZLY_ALWAYS_INLINE BlendFactor dst_alpha_blend_factor() const { return m_dst_alpha_blend_factor; }
		GRIZZLY_ALWAYS_INLINE BlendOp alpha_blend_op() const { return m_alpha_blend_op; }

		GRIZZLY_ALWAYS_INLINE bool is_depth_test_enabled() const { return m_depth_test; }
		GRIZZLY_ALWAYS_INLINE bool is_depth_write_enabled() const { return m_depth_write; }
		GRIZZLY_ALWAYS_INLINE CompareOp depth_compare_op() const { return m_depth_compare; }

	protected:
		Shared<VertexShader> m_vertex_shader;
		Shared<PixelShader> m_pixel_shader;

		Array<Texture::Format, InlineAllocator<8>> m_color_attachments;
		Option<Texture::Format> m_depth_attachment;

		DrawMode m_draw_mode;
		f32 m_line_width;
		CullMode m_cull_mode;

		bool m_blend_enabled;

		BlendFactor m_src_color_blend_factor;
		BlendFactor m_dst_color_blend_factor;
		BlendOp m_color_blend_op;

		BlendFactor m_src_alpha_blend_factor;
		BlendFactor m_dst_alpha_blend_factor;
		BlendOp m_alpha_blend_op;

		bool m_depth_test;
		bool m_depth_write;
		CompareOp m_depth_compare;
	};
} // namespace Grizzly::GPU
