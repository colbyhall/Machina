/**
 * copyright (c) 2024 colby hall <me@cobeh.com>
 *
 * this software is released under the mit license.
 */

#pragma once

#include <Core/Containers/Array.hpp>
#include <Core/Containers/Function.hpp>
#include <Core/Containers/Shared.hpp>
#include <Core/Math/Vector4.hpp>
#include <GPU/Forward.hpp>

namespace Grizzly::GPU {
	enum class Layout : u8 {
		Undefined,
		General,
		ColorAttachment,
		DepthAttachment,
		TransferSrc,
		TransferDst,
		ShaderReadOnly,
		Present,
	};

	class CommandList : public SharedFromThis<CommandList> {
	public:
		virtual ~CommandList() {}
	};

	class RenderPassRecorder {
	public:
		virtual RenderPassRecorder& clear_color(Vector4<f32> const& color) = 0;
		virtual RenderPassRecorder& clear_depth_stencil(f32 depth, u8 stencil) = 0;
		virtual RenderPassRecorder& set_pipeline(GraphicsPipeline const& pipeline) = 0;
		virtual RenderPassRecorder& set_vertices(Buffer const& buffer) = 0;
		virtual RenderPassRecorder& set_indices(Buffer const& buffer) = 0;
		virtual RenderPassRecorder& push_constant(const void* ptr) = 0;
		virtual RenderPassRecorder& draw(usize vertex_count, usize first_vertex) = 0;
		virtual RenderPassRecorder& draw_indexed(usize index_count, usize first_index) = 0;

		virtual ~RenderPassRecorder() {}
	};

	class CommandRecorder {
	public:
		virtual CommandRecorder& copy_buffer_to_texture(Texture const& dst, Buffer const& src) = 0;
		virtual CommandRecorder& copy_buffer_to_buffer(Buffer const& dst, Buffer const& src) = 0;
		virtual CommandRecorder& texture_barrier(Texture const& texture, Layout before, Layout after) = 0;

		struct RenderPass {
			Slice<Texture const&> color_attachments;
			Option<Texture const&> depth_attachment = nullopt;
		};
		virtual CommandRecorder& render_pass(RenderPass const& info, FunctionRef<void(RenderPassRecorder&)> f) = 0;

		virtual ~CommandRecorder() {}
	};
} // namespace Grizzly::GPU