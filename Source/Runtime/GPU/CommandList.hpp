/**
 * copyright (c) 2024 colby hall <me@cobeh.com>
 *
 * this software is released under the mit license.
 */

#pragma once

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

	struct RenderPassInfo {};

	class CommandList : public SharedFromThis<CommandList> {
	public:
		virtual ~CommandList() {}

	protected:
		friend class CommandRecorder;
		friend class RenderPassRecorder;

		virtual void begin() = 0;
		virtual void end() = 0;

		virtual void copy_buffer_to_texture(Texture const& dst, Buffer const& src) = 0;
		virtual void copy_buffer_to_buffer(Buffer const& dst, Buffer const& src) = 0;
		virtual void texture_barrier(Texture const& texture, Layout before, Layout after) = 0;

		virtual void begin_render_pass(RenderPassInfo const& info) = 0;
		virtual void end_render_pass() = 0;
		virtual void clear_color(Vector4<f32> const& color) = 0;
		virtual void clear_depth_stencil(f32 depth, u8 stencil) = 0;
		virtual void set_pipeline(GraphicsPipeline const& pipeline) = 0;
		virtual void set_vertices(Buffer const& buffer) = 0;
		virtual void set_indices(Buffer const& buffer) = 0;
		virtual void push_constant(const void* ptr) = 0;
		virtual void draw(usize vertex_count, usize first_vertex) = 0;
		virtual void draw_indexed(usize index_count, usize first_index) = 0;
	};

#define DEFINE_PASSTHROUGH_ONE_ARG(class, method_name, arg_type, arg_name)                                             \
	GRIZZLY_ALWAYS_INLINE class& method_name(arg_type arg_name) {                                                      \
		m_command_list.method_name(arg_name);                                                                          \
		return *this;                                                                                                  \
	}

#define DEFINE_PASSTHROUGH_TWO_ARGS(class, method_name, arg1_type, arg1_name, arg2_type, arg2_name)                    \
	GRIZZLY_ALWAYS_INLINE class& method_name(arg1_type arg1_name, arg2_type arg2_name) {                               \
		m_command_list.method_name(arg1_name, arg2_name);                                                              \
		return *this;                                                                                                  \
	}

#define DEFINE_PASSTHROUGH_THREE_ARGS(                                                                                 \
	class,                                                                                                             \
	method_name,                                                                                                       \
	arg1_type,                                                                                                         \
	arg1_name,                                                                                                         \
	arg2_type,                                                                                                         \
	arg2_name,                                                                                                         \
	arg3_type,                                                                                                         \
	arg3_name)                                                                                                         \
	GRIZZLY_ALWAYS_INLINE class& method_name(arg1_type arg1_name, arg2_type arg2_name, arg3_type arg3_name) {          \
		m_command_list.method_name(arg1_name, arg2_name, arg3_name);                                                   \
		return *this;                                                                                                  \
	}

	class RenderPassRecorder {
	public:
		DEFINE_PASSTHROUGH_ONE_ARG(RenderPassRecorder, clear_color, Vector4<f32> const&, color);
		DEFINE_PASSTHROUGH_TWO_ARGS(RenderPassRecorder, clear_depth_stencil, f32, depth, u8, stencil);
		DEFINE_PASSTHROUGH_ONE_ARG(RenderPassRecorder, set_pipeline, GraphicsPipeline const&, pipeline);
		DEFINE_PASSTHROUGH_ONE_ARG(RenderPassRecorder, set_vertices, Buffer const&, buffer);
		DEFINE_PASSTHROUGH_ONE_ARG(RenderPassRecorder, set_indices, Buffer const&, buffer);
		DEFINE_PASSTHROUGH_ONE_ARG(RenderPassRecorder, push_constant, const void*, ptr);
		DEFINE_PASSTHROUGH_TWO_ARGS(RenderPassRecorder, draw, usize, vertex_count, usize, first_index);
		DEFINE_PASSTHROUGH_TWO_ARGS(RenderPassRecorder, draw_indexed, usize, index_count, usize, first_index);

	private:
		friend class CommandRecorder;
		explicit RenderPassRecorder(CommandList& command_list) : m_command_list{ command_list } {}
		CommandList& m_command_list;
	};

	class CommandRecorder {
	public:
		DEFINE_PASSTHROUGH_TWO_ARGS(CommandRecorder, copy_buffer_to_texture, Texture const&, dst, Buffer const&, src);
		DEFINE_PASSTHROUGH_TWO_ARGS(CommandRecorder, copy_buffer_to_buffer, Buffer const&, dst, Buffer const&, src);
		DEFINE_PASSTHROUGH_THREE_ARGS(
			CommandRecorder,
			texture_barrier,
			Texture const&,
			texture,
			Layout,
			before,
			Layout,
			after);

		GRIZZLY_ALWAYS_INLINE CommandRecorder&
		render_pass(RenderPassInfo const& info, FunctionRef<void(RenderPassRecorder&)> f) {
			m_command_list.begin_render_pass(info);
			RenderPassRecorder rpr{ m_command_list };
			f(rpr);
			m_command_list.end_render_pass();
			return *this;
		}

	private:
		explicit CommandRecorder(CommandList& command_list) : m_command_list{ command_list } {}
		friend class MetalDevice;

		CommandList& m_command_list;
	};

#undef DEFINE_PASSTHROUGH_ONE_ARG
#undef DEFINE_PASSTHROUGH_TWO_ARGS
#undef DEFINE_PASSTHROUGH_THREE_ARGS

} // namespace Grizzly::GPU