/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * this software is released under the mit license.
 */

#include <GPU/Metal/Buffer.hpp>
#include <GPU/Metal/CommandList.hpp>
#include <GPU/Metal/Conversion.hpp>
#include <GPU/Metal/GraphicsPipeline.hpp>
#include <GPU/Metal/Texture.hpp>

namespace Grizzly::GPU {
	void MetalReceipt::wait_until_complete() const {
		@autoreleasepool {
			[m_command_buffer waitUntilCompleted];
		}
	}

	Unique<Receipt> MetalCommandList::submit() const {
		@autoreleasepool {
			[m_command_buffer commit];
			return Unique<MetalReceipt>::create(m_command_buffer);
		}
	}

	CommandRecorder& MetalCommandRecorder::copy_buffer_to_texture(Texture const& dst, Buffer const& src) {
		return *this;
	}

	CommandRecorder& MetalCommandRecorder::copy_buffer_to_buffer(Buffer const& dst, Buffer const& src) { return *this; }
	CommandRecorder& MetalCommandRecorder::texture_barrier(Texture const& texture, Layout before, Layout after) {
		return *this;
	}

	CommandRecorder&
	MetalCommandRecorder::render_pass(RenderPass const& info, FunctionRef<void(RenderPassRecorder&)> f) {
		@autoreleasepool {
			// Convert the render pass info to a Metal render pass descriptor
			MTLRenderPassDescriptor* descriptor = [MTLRenderPassDescriptor renderPassDescriptor];
			for (usize index = 0; index < info.color_attachments.len(); index += 1) {
				auto const& my_attachment = info.color_attachments[index];
				MTLRenderPassColorAttachmentDescriptor* const metal_attachment = descriptor.colorAttachments[index];

				id<MTLTexture> const texture = *(static_cast<MetalTexture const&>(my_attachment.texture)).texture();
				metal_attachment.texture = texture;
				metal_attachment.loadAction = load_action_to_mtl_load_action(my_attachment.load_action);
				metal_attachment.storeAction = store_action_to_mtl_store_action(my_attachment.store_action);
				metal_attachment.clearColor = MTLClearColorMake(
					my_attachment.clear_color.x,
					my_attachment.clear_color.y,
					my_attachment.clear_color.z,
					my_attachment.clear_color.w);
			}
			if (info.depth_attachment.is_set()) {
				auto const& my_attachment = info.depth_attachment.as_const_ref().unwrap();
				MTLRenderPassDepthAttachmentDescriptor* const metal_attachment = descriptor.depthAttachment;

				id<MTLTexture> const texture = *(static_cast<MetalTexture const&>(my_attachment.texture)).texture();
				metal_attachment.texture = texture;
				metal_attachment.loadAction = load_action_to_mtl_load_action(my_attachment.load_action);
				metal_attachment.storeAction = store_action_to_mtl_store_action(my_attachment.store_action);
				metal_attachment.clearDepth = my_attachment.clear_depth;
			}

			// Initialize the render pass encoder
			id<MTLRenderCommandEncoder> encoder = [m_command_buffer renderCommandEncoderWithDescriptor:descriptor];
			MetalRenderPassRecorder recorder(encoder);

			// Execute the callback to record the render pass
			f(recorder);

			[encoder endEncoding];
		}
		return *this;
	}

	RenderPassRecorder& MetalRenderPassRecorder::set_pipeline(GraphicsPipeline const& pipeline) {
		@autoreleasepool {
			auto& casted = reinterpret_cast<MetalGraphicsPipeline const&>(pipeline);
			[m_encoder setRenderPipelineState:casted.render_pipeline_state()];
			const auto cull_mode_converted = cull_mode_to_mtl_cull_mode(pipeline.cull_mode());
			[m_encoder setCullMode:cull_mode_converted];
		}
		return *this;
	}
	RenderPassRecorder& MetalRenderPassRecorder::set_vertices(Buffer const& buffer) {
		@autoreleasepool {
			auto& casted = reinterpret_cast<MetalBuffer const&>(buffer);
			[m_encoder setVertexBuffer:casted.buffer() offset:0 atIndex:0];
		}
		return *this;
	}
	RenderPassRecorder& MetalRenderPassRecorder::set_indices(Buffer const& buffer) { return *this; }
	RenderPassRecorder& MetalRenderPassRecorder::push_constant(const void* ptr) { return *this; }
	RenderPassRecorder& MetalRenderPassRecorder::draw(usize vertex_count, usize first_vertex) {
		@autoreleasepool {
			[m_encoder drawPrimitives:MTLPrimitiveTypeTriangle
						  vertexStart:first_vertex
						  vertexCount:vertex_count
						instanceCount:1];
		}
		return *this;
	}
	RenderPassRecorder& MetalRenderPassRecorder::draw_indexed(usize index_count, usize first_index) { return *this; }
} // namespace Grizzly::GPU
