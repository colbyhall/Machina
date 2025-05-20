/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * this software is released under the mit license.
 */

#pragma once

#include <Core/Containers/Array.hpp>
#include <Core/Containers/Function.hpp>
#include <Core/Containers/SharedPtr.hpp>
#include <Core/Containers/UniquePtr.hpp>
#include <Core/Containers/Variant.hpp>
#include <Core/Math/Vector4.hpp>
#include <GPU/Forward.hpp>

namespace Forge::GPU {
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

	class Receipt : public SharedPtrFromThis<Receipt> {
	public:
		virtual void wait_until_complete() const = 0;

		virtual ~Receipt() {}
	};

	class CommandList : public SharedPtrFromThis<CommandList> {
	public:
		FORGE_NO_DISCARD virtual SharedPtr<Receipt> submit() const = 0;

		virtual ~CommandList() {}
	};

	class RenderPassRecorder {
	public:
		virtual RenderPassRecorder& set_pipeline(GraphicsPipeline const& pipeline) = 0;
		virtual RenderPassRecorder& set_vertices(Buffer const& buffer) = 0;
		virtual RenderPassRecorder& set_indices(Buffer const& buffer) = 0;
		virtual RenderPassRecorder& set_constant(u32 index, Buffer const& buffer, u32 offset = 0) = 0;
		virtual RenderPassRecorder& draw(usize vertex_count, usize first_vertex) = 0;
		virtual RenderPassRecorder& draw_indexed(usize index_count, usize first_index) = 0;

		virtual ~RenderPassRecorder() {}
	};

	namespace ColorLoadAction {
		struct DontCare {};
		struct Load {};
		struct Clear {
			f32 r;
			f32 g;
			f32 b;
			f32 a;
		};
		using Variant = Core::Variant<DontCare, Load, Clear>;
	} // namespace ColorLoadAction

	namespace DepthLoadAction {
		struct DontCare {};
		struct Load {};
		struct Clear {
			f32 depth;
			u32 stencil = 0;
		};
		using Variant = Core::Variant<DontCare, Load, Clear>;
	} // namespace DepthLoadAction

	enum class StoreAction : u8 {
		DontCare,
		Store,
	};

	struct RenderPass {
		struct ColorAttachment {
			Texture const& texture;
			ColorLoadAction::Variant load_action;
			StoreAction store_action;
		};

		struct DepthAttachment {
			Texture const& texture;
			DepthLoadAction::Variant load_action;
			StoreAction store_action;
		};

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
} // namespace Forge::GPU
