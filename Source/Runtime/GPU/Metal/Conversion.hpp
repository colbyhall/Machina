/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <GPU/CommandList.hpp>
#include <GPU/GraphicsPipeline.hpp>
#include <GPU/Texture.hpp>

#import <Metal/Metal.h>

namespace Grizzly::GPU {
	inline MTLPixelFormat format_to_mtl_pixel_format(Texture::Format format) {
		switch (format) {
		case Texture::Format::Undefined:
			return MTLPixelFormatInvalid;
		case Texture::Format::R_U8:
			return MTLPixelFormatR8Unorm;
		case Texture::Format::RGBA_U8:
			return MTLPixelFormatRGBA8Unorm;
		case Texture::Format::RGBA_U8_SRGB:
			return MTLPixelFormatRGBA8Unorm_sRGB;
		case Texture::Format::RGBA_F16:
			return MTLPixelFormatRGBA16Float;
		case Texture::Format::RGBA_F32:
			return MTLPixelFormatRGBA32Float;
		case Texture::Format::BGRA_U8_SRGB:
			return MTLPixelFormatBGRA8Unorm;
		case Texture::Format::Depth16:
			return MTLPixelFormatDepth16Unorm;
		case Texture::Format::Depth24_Stencil8:
			return MTLPixelFormatDepth24Unorm_Stencil8;
		default:
			GRIZZLY_UNIMPLEMENTED;
			break;
		}
		return MTLPixelFormatInvalid;
	}

	inline MTLCompareFunction compare_op_to_mtl_compare_function(GraphicsPipeline::CompareOp compare_op) {
		switch (compare_op) {
		case GraphicsPipeline::CompareOp::Never:
			return MTLCompareFunctionNever;
		case GraphicsPipeline::CompareOp::Less:
			return MTLCompareFunctionLess;
		case GraphicsPipeline::CompareOp::Equal:
			return MTLCompareFunctionEqual;
		case GraphicsPipeline::CompareOp::NotEqual:
			return MTLCompareFunctionNotEqual;
		case GraphicsPipeline::CompareOp::LessOrEqual:
			return MTLCompareFunctionLessEqual;
		case GraphicsPipeline::CompareOp::Greater:
			return MTLCompareFunctionGreater;
		case GraphicsPipeline::CompareOp::GreaterOrEqual:
			return MTLCompareFunctionGreaterEqual;
		case GraphicsPipeline::CompareOp::Always:
			return MTLCompareFunctionAlways;
		default:
			GRIZZLY_UNIMPLEMENTED;
			break;
		}
	}

	inline MTLBlendOperation blend_op_to_mtl_blend_operation(GraphicsPipeline::BlendOp blend_op) {
		switch (blend_op) {
		case GraphicsPipeline::BlendOp::Add:
			return MTLBlendOperationAdd;
		case GraphicsPipeline::BlendOp::Subtract:
			return MTLBlendOperationSubtract;
		case GraphicsPipeline::BlendOp::ReverseSubtract:
			return MTLBlendOperationReverseSubtract;
		case GraphicsPipeline::BlendOp::Min:
			return MTLBlendOperationMin;
		case GraphicsPipeline::BlendOp::Max:
			return MTLBlendOperationMax;
		default:
			GRIZZLY_UNIMPLEMENTED;
			break;
		}
		return MTLBlendOperationAdd;
	}

	inline MTLBlendFactor blend_factor_to_mtl_blend_factor(GraphicsPipeline::BlendFactor blend_factor) {
		switch (blend_factor) {
		case GraphicsPipeline::BlendFactor::Zero:
			return MTLBlendFactorZero;
		case GraphicsPipeline::BlendFactor::One:
			return MTLBlendFactorOne;
		case GraphicsPipeline::BlendFactor::SrcColor:
			return MTLBlendFactorSourceColor;
		case GraphicsPipeline::BlendFactor::OneMinusSrcColor:
			return MTLBlendFactorOneMinusSourceColor;
		case GraphicsPipeline::BlendFactor::DstColor:
			return MTLBlendFactorDestinationColor;
		case GraphicsPipeline::BlendFactor::OneMinusDstColor:
			return MTLBlendFactorOneMinusDestinationColor;
		case GraphicsPipeline::BlendFactor::SrcAlpha:
			return MTLBlendFactorSourceAlpha;
		case GraphicsPipeline::BlendFactor::OneMinusSrcAlpha:
			return MTLBlendFactorOneMinusSourceAlpha;
		default:
			GRIZZLY_UNIMPLEMENTED;
			break;
		}
		return MTLBlendFactorZero;
	}

	inline MTLPrimitiveTopologyClass draw_mode_to_mtl_primitive_topology_class(GraphicsPipeline::DrawMode draw_mode) {
		switch (draw_mode) {
		case GraphicsPipeline::DrawMode::Point:
			return MTLPrimitiveTopologyClassPoint;
		case GraphicsPipeline::DrawMode::Line:
			return MTLPrimitiveTopologyClassLine;
		case GraphicsPipeline::DrawMode::Fill:
			return MTLPrimitiveTopologyClassTriangle;
		default:
			GRIZZLY_UNIMPLEMENTED;
			break;
		}
		return MTLPrimitiveTopologyClassUnspecified;
	}

	inline MTLWinding winding_to_mtl_winding(GraphicsPipeline::Winding winding) {
		switch (winding) {
		case GraphicsPipeline::Winding::Clockwise:
			return MTLWindingClockwise;
		case GraphicsPipeline::Winding::CounterClockwise:
			return MTLWindingCounterClockwise;
		default:
			GRIZZLY_UNIMPLEMENTED;
			break;
		}
		return MTLWindingClockwise;
	}

	inline MTLLoadAction load_action_to_mtl_load_action(LoadAction action) {
		switch (action) {
		case LoadAction::Clear:
			return MTLLoadActionClear;
		case LoadAction::Load:
			return MTLLoadActionLoad;
		case LoadAction::DontCare:
			return MTLLoadActionDontCare;
		default:
			GRIZZLY_UNIMPLEMENTED;
			break;
		}
		return MTLLoadActionDontCare;
	}

	inline MTLStoreAction store_action_to_mtl_store_action(StoreAction action) {
		switch (action) {
		case StoreAction::Store:
			return MTLStoreActionStore;
		case StoreAction::DontCare:
			return MTLStoreActionDontCare;
		default:
			GRIZZLY_UNIMPLEMENTED;
			break;
		}
		return MTLStoreActionDontCare;
	}

	inline MTLCullMode cull_mode_to_mtl_cull_mode(GraphicsPipeline::CullMode cull_mode) {
		switch (cull_mode) {
		case GraphicsPipeline::CullMode::None:
			return MTLCullModeNone;
		case GraphicsPipeline::CullMode::Front:
			return MTLCullModeFront;
		case GraphicsPipeline::CullMode::Back:
			return MTLCullModeBack;
		default:
			GRIZZLY_UNIMPLEMENTED;
			break;
		}
		return MTLCullModeNone;
	}

} // namespace Grizzly::GPU
