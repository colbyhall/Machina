/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * this software is released under the mit license.
 */

#pragma once

#import <Metal/Metal.h>

#include <Core/ObjectiveC/Protocol.hpp>
#include <GPU/CommandList.hpp>

namespace Mach::GPU {
	class MetalReceipt final : public Receipt {
	public:
		MetalReceipt(id<MTLCommandBuffer> command_buffer) : m_command_buffer(command_buffer) {}
		void wait_until_complete() const final;

	private:
		Core::Protocol m_command_buffer;
	};

	class MetalCommandList final : public CommandList {
	public:
		MetalCommandList(id<MTLCommandBuffer> command_buffer) : m_command_buffer(command_buffer) {}

		SharedPtr<Receipt> submit() const final;

	private:
		Core::Protocol m_command_buffer; // MTLCommandBuffer
	};

	class MetalCommandRecorder final : public CommandRecorder {
	public:
		explicit MetalCommandRecorder(id<MTLCommandBuffer> command_buffer) : m_command_buffer(command_buffer) {}

		CommandRecorder& copy_buffer_to_texture(Texture const& dst, Buffer const& src) final;
		CommandRecorder& copy_buffer_to_buffer(Buffer const& dst, Buffer const& src) final;
		CommandRecorder& texture_barrier(Texture const& texture, Layout before, Layout after) final;
		CommandRecorder& render_pass(RenderPass const& info, FunctionRef<void(RenderPassRecorder&)> f) final;

	private:
		Core::Protocol m_command_buffer; // MTLCommandBuffer
	};

	class MetalRenderPassRecorder final : public RenderPassRecorder {
	public:
		explicit MetalRenderPassRecorder(id<MTLRenderCommandEncoder> encoder) : m_encoder(encoder) {}

		RenderPassRecorder& set_pipeline(GraphicsPipeline const& pipeline) final;
		RenderPassRecorder& set_vertices(Buffer const& buffer) final;
		RenderPassRecorder& set_indices(Buffer const& buffer) final;
		RenderPassRecorder& set_constant(u32 index, Buffer const& buffer, u32 offset = 0) final;
		RenderPassRecorder& draw(usize vertex_count, usize first_vertex) final;
		RenderPassRecorder& draw_indexed(usize index_count, usize first_index) final;

	private:
		Core::Protocol m_encoder; // MTLRenderCommandEncoder
	};

} // namespace Mach::GPU
