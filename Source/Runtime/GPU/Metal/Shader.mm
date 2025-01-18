/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * this software is released under the mit license.
 */

#include <Foundation/Foundation.h>
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

	Rc<VertexShader> MetalLibrary::create_vertex_shader(StringView function_name) {
		@autoreleasepool {
			id<MTLFunction> function = get_function_from_library(m_library, function_name);
			GRIZZLY_ASSERT(function != nil);
			GRIZZLY_ASSERT(function.functionType == MTLFunctionTypeVertex);
			return Rc<MetalVertexShader>::create(function);
		}
	}

	Rc<FragmentShader> MetalLibrary::create_fragment_shader(StringView function_name) {
		@autoreleasepool {
			id<MTLFunction> function = get_function_from_library(m_library, function_name);
			GRIZZLY_ASSERT(function != nil);
			GRIZZLY_ASSERT(function.functionType == MTLFunctionTypeFragment);
			return Rc<MetalFragmentShader>::create(function);
		}
	}
} // namespace Grizzly::GPU
