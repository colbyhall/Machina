/**
 * copyright (c) 2024 colby hall <me@cobeh.com>
 *
 * this software is released under the mit license.
 */

#include <GPU/Metal/Shader.hpp>

namespace Grizzly::GPU {
	static id<MTLFunction> get_function_from_library(id<MTLLibrary> library, StringView function_name) {
		@autoreleasepool {
			NSString* name = [[NSString alloc] initWithBytesNoCopy:(void*)*function_name
															length:static_cast<NSUInteger>(function_name.len())
														  encoding:NSUTF8StringEncoding
													  freeWhenDone:NO];
			id<MTLFunction> function = [library newFunctionWithName:name];
			return function;
		}
	}
	Shared<VertexShader> MetalLibrary::create_vertex_shader(StringView function_name) {
		@autoreleasepool {
			id<MTLFunction> function = get_function_from_library(m_library, function_name);
			GRIZZLY_ASSERT(function != nil);
			GRIZZLY_ASSERT(function.functionType == MTLFunctionTypeVertex);
			return Shared<MetalVertexShader>::create(function);
		}
	}
	Shared<PixelShader> MetalLibrary::create_pixel_shader(StringView function_name) {
		@autoreleasepool {
			id<MTLFunction> function = get_function_from_library(m_library, function_name);
			GRIZZLY_ASSERT(function != nil);
			GRIZZLY_ASSERT(function.functionType == MTLFunctionTypeFragment);
			return Shared<MetalPixelShader>::create(function);
		}
	}
} // namespace Grizzly::GPU
