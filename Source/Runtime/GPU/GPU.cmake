# Set the root
set(GPU_ROOT ${RUNTIME_ROOT}/GPU)

# Source files
set(GPU_SRC_FILES
	${GPU_ROOT}/Buffer.hpp
	${GPU_ROOT}/CommandList.hpp
	${GPU_ROOT}/Device.hpp
	${GPU_ROOT}/Device.cpp
	${GPU_ROOT}/Forward.hpp
	${GPU_ROOT}/GPU.cmake
	${GPU_ROOT}/GraphicsPipeline.hpp
	${GPU_ROOT}/Resource.hpp
	${GPU_ROOT}/Shader.hpp
	${GPU_ROOT}/Swapchain.hpp
	${GPU_ROOT}/Texture.hpp
)

if (APPLE)
	set(GPU_SRC_FILES
		${GPU_SRC_FILES}
		${GPU_ROOT}/Metal/Buffer.mm
		${GPU_ROOT}/Metal/Buffer.hpp
		${GPU_ROOT}/Metal/CommandList.mm
		${GPU_ROOT}/Metal/CommandList.hpp
		${GPU_ROOT}/Metal/Conversion.hpp
		${GPU_ROOT}/Metal/Device.mm
		${GPU_ROOT}/Metal/Device.hpp
		${GPU_ROOT}/Metal/GraphicsPipeline.mm
		${GPU_ROOT}/Metal/GraphicsPipeline.hpp
		${GPU_ROOT}/Metal/Shader.mm
		${GPU_ROOT}/Metal/Shader.hpp
		${GPU_ROOT}/Metal/Swapchain.mm
		${GPU_ROOT}/Metal/Swapchain.hpp
		${GPU_ROOT}/Metal/Texture.mm
		${GPU_ROOT}/Metal/Texture.hpp
	)
elseif(WIN32)
endif()

add_runtime_library(GPU ${GPU_ROOT} ${GPU_SRC_FILES})

if (APPLE)
	target_link_libraries(GPU Core "-framework AppKit" "-framework Metal" "-framework QuartzCore")
elseif(WIN32)
endif()
