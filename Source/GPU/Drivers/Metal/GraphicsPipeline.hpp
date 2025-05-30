/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * this software is released under the mit license.
 */

#pragma once

#include <Core/ObjectiveC/Protocol.hpp>
#include <GPU/GraphicsPipeline.hpp>

namespace Mach::GPU {
	class MetalGraphicsPipeline final : public GraphicsPipeline {
	public:
		explicit MetalGraphicsPipeline(
			const GraphicsPipeline::CreateInfo& create_info,
			id<MTLRenderPipelineState> render_pipeline_state)
			: GraphicsPipeline(create_info)
			, m_render_pipeline_state(render_pipeline_state) {}

		MACH_ALWAYS_INLINE Core::Protocol render_pipeline_state() const { return m_render_pipeline_state; }

	private:
		Core::Protocol m_render_pipeline_state;
	};
} // namespace Mach::GPU
