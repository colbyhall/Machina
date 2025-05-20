/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * this software is released under the mit license.
 */

#include <Foundation/Foundation.h>
#include <GPU/Drivers/Metal/Shader.hpp>

namespace Forge::GPU {
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

	Arc<VertexShader> MetalLibrary::create_vertex_shader(StringView function_name) const {
		@autoreleasepool {
			id<MTLFunction> function = get_function_from_library(m_library, function_name);
			FORGE_ASSERT(function != nil);
			FORGE_ASSERT(function.functionType == MTLFunctionTypeVertex);
			return Arc<MetalVertexShader>::create(function);
		}
	}

	Arc<FragmentShader> MetalLibrary::create_fragment_shader(StringView function_name) const {
		@autoreleasepool {
			id<MTLFunction> function = get_function_from_library(m_library, function_name);
			FORGE_ASSERT(function != nil);
			FORGE_ASSERT(function.functionType == MTLFunctionTypeFragment);
			return Arc<MetalFragmentShader>::create(function);
		}
	}
} // namespace Forge::GPU
