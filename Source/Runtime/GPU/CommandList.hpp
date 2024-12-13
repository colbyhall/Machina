/**
 * copyright (c) 2024 colby hall <me@cobeh.com>
 *
 * this software is released under the mit license.
 */

#pragma once

#include <Core/Containers/Array.hpp>
#include <Core/Containers/Function.hpp>
#include <Core/Containers/Shared.hpp>
#include <Core/Containers/Unique.hpp>
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

	class Receipt {
	public:
		virtual void wait_until_complete() const = 0;

		virtual ~Receipt() {}
	};

	class CommandList : public SharedFromThis<CommandList> {
	public:
		GRIZZLY_NO_DISCARD virtual Unique<Receipt> submit() const = 0;

		virtual ~CommandList() {}
	};

	class RenderPassRecorder {
	public:
		virtual RenderPassRecorder& set_pipeline(GraphicsPipeline const& pipeline) = 0;
		virtual RenderPassRecorder& set_vertices(Buffer const& buffer) = 0;
		virtual RenderPassRecorder& set_indices(Buffer const& buffer) = 0;
		virtual RenderPassRecorder& push_constant(const void* ptr) = 0;
		virtual RenderPassRecorder& draw(usize vertex_count, usize first_vertex) = 0;
		virtual RenderPassRecorder& draw_indexed(usize index_count, usize first_index) = 0;

		virtual ~RenderPassRecorder() {}
	};

	enum class LoadAction : u8 {
		DontCare,
		Load,
		Clear,
	};

	enum class StoreAction : u8 {
		DontCare,
		Store,
	};

	struct ColorAttachment {
		Texture const& texture;
		LoadAction load_action;
		StoreAction store_action;
		Vector4<f32> clear_color = { 0.f, 0.f, 0.f, 1.f };
	};

	struct DepthAttachment {
		Texture const& texture;
		LoadAction load_action;
		StoreAction store_action;
		f32 clear_depth = 1.f;
	};

	struct RenderPass {
		Slice<ColorAttachment const> color_attachments;
		Option<DepthAttachment const> depth_attachment = nullopt;
	};

	class CommandRecorder {
	public:
		virtual CommandRecorder& copy_buffer_to_texture(Texture const& dst, Buffer const& src) = 0;
		virtual CommandRecorder& copy_buffer_to_buffer(Buffer const& dst, Buffer const& src) = 0;
		virtual CommandRecorder& texture_barrier(Texture const& texture, Layout before, Layout after) = 0;
		virtual CommandRecorder& render_pass(RenderPass const& info, FunctionRef<void(RenderPassRecorder&)> f) = 0;

		virtual ~CommandRecorder() {}
	};
} // namespace Grizzly::GPU
