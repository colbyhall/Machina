/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * this software is released under the mit license.
 */

#include <GPU/Drivers/Metal/Buffer.hpp>
#include <GPU/Drivers/Metal/CommandList.hpp>
#include <GPU/Drivers/Metal/Conversion.hpp>
#include <GPU/Drivers/Metal/GraphicsPipeline.hpp>
#include <GPU/Drivers/Metal/Texture.hpp>

namespace Forge::GPU {
	void MetalReceipt::wait_until_complete() const {
		@autoreleasepool {
			[m_command_buffer waitUntilCompleted];
		}
	}

	SharedPtr<Receipt> MetalCommandList::submit() const {
		@autoreleasepool {
			[m_command_buffer commit];
			return SharedPtr<MetalReceipt>::create(m_command_buffer);
		}
	}

	CommandRecorder& MetalCommandRecorder::copy_buffer_to_texture(Texture const& dst, Buffer const& src) {
		FORGE_UNUSED(dst);
		FORGE_UNUSED(src);
		FORGE_UNIMPLEMENTED;
		return *this;
	}

	CommandRecorder& MetalCommandRecorder::copy_buffer_to_buffer(Buffer const& dst, Buffer const& src) {
		FORGE_UNUSED(dst);
		FORGE_UNUSED(src);
		FORGE_UNIMPLEMENTED;
		return *this;
	}

	CommandRecorder& MetalCommandRecorder::texture_barrier(Texture const& texture, Layout before, Layout after) {
		FORGE_UNUSED(texture);
		FORGE_UNUSED(before);
		FORGE_UNUSED(after);
		FORGE_UNIMPLEMENTED;
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

				if (auto clear_color = my_attachment.load_action.get<ColorLoadAction::Clear>()) {
					metal_attachment.clearColor =
						MTLClearColorMake(clear_color->r, clear_color->g, clear_color->b, clear_color->a);
				}
			}
			if (auto depth_attachment = info.depth_attachment.as_const_ref()) {
				MTLRenderPassDepthAttachmentDescriptor* const metal_attachment = descriptor.depthAttachment;

				id<MTLTexture> const texture = *(static_cast<MetalTexture const&>(depth_attachment->texture)).texture();
				metal_attachment.texture = texture;
				metal_attachment.loadAction = load_action_to_mtl_load_action(depth_attachment->load_action);
				metal_attachment.storeAction = store_action_to_mtl_store_action(depth_attachment->store_action);

				if (auto clear_color = depth_attachment->load_action.get<DepthLoadAction::Clear>()) {
					metal_attachment.clearDepth = clear_color->depth;
				}
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

	RenderPassRecorder& MetalRenderPassRecorder::set_indices(Buffer const& buffer) {
		@autoreleasepool {
			auto& casted = reinterpret_cast<MetalBuffer const&>(buffer);
			[m_encoder setIndexBuffer:casted.buffer()];
		}
		return *this;
	}

	RenderPassRecorder& MetalRenderPassRecorder::set_constant(u32 index, Buffer const& buffer, u32 offset) {
		@autoreleasepool {
			auto& casted = reinterpret_cast<MetalBuffer const&>(buffer);
			[m_encoder setVertexBuffer:casted.buffer() offset:offset atIndex:index + 1];
		}
		return *this;
	}

	RenderPassRecorder& MetalRenderPassRecorder::draw(usize vertex_count, usize first_vertex) {
		@autoreleasepool {
			[m_encoder drawPrimitives:MTLPrimitiveTypeTriangle
						  vertexStart:first_vertex
						  vertexCount:vertex_count
						instanceCount:1];
		}
		return *this;
	}
	RenderPassRecorder& MetalRenderPassRecorder::draw_indexed(usize index_count, usize first_index) {
		FORGE_UNUSED(index_count);
		FORGE_UNUSED(first_index);
		FORGE_UNIMPLEMENTED;
		return *this;
	}
} // namespace Forge::GPU
