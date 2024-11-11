/**
 * copyright (c) 2024 colby hall <me@cobeh.com>
 *
 * this software is released under the mit license.
 */

#include <GPU/Metal/CommandList.hpp>

namespace Grizzly::GPU {
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
			MTLRenderPassDescriptor* descriptor = [MTLRenderPassDescriptor renderPassDescriptor];
			id<MTLRenderCommandEncoder> encoder = [m_command_buffer renderCommandEncoderWithDescriptor:descriptor];
			MetalRenderPassRecorder recorder(encoder);
			f(recorder);
		}
		return *this;
	}

	RenderPassRecorder& MetalRenderPassRecorder::clear_color(Vector4<f32> const& color) { return *this; }
	RenderPassRecorder& MetalRenderPassRecorder::clear_depth_stencil(f32 depth, u8 stencil) { return *this; }
	RenderPassRecorder& MetalRenderPassRecorder::set_pipeline(GraphicsPipeline const& pipeline) { return *this; }
	RenderPassRecorder& MetalRenderPassRecorder::set_vertices(Buffer const& buffer) { return *this; }
	RenderPassRecorder& MetalRenderPassRecorder::set_indices(Buffer const& buffer) { return *this; }
	RenderPassRecorder& MetalRenderPassRecorder::push_constant(const void* ptr) { return *this; }
	RenderPassRecorder& MetalRenderPassRecorder::draw(usize vertex_count, usize first_vertex) { return *this; }
	RenderPassRecorder& MetalRenderPassRecorder::draw_indexed(usize index_count, usize first_index) { return *this; }
} // namespace Grizzly::GPU
