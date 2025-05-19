# Set the root
set(GPU_ROOT ${SOURCE_ROOT}/GPU)

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

# Declare variables that represent what GPU API is supported
set(OS_SUPPORTS_METAL NO)
set(OS_SUPPORTS_VULKAN NO)
set(OS_SUPPORTS_D3D12 NO)

# Define what GPU APIs are supported
if(OS_MACOS)
	set(OS_SUPPORTS_METAL YES)
elseif(OS_WINDOWS)
	set(OS_SUPPORTS_VULKAN YES)
	set(OS_SUPPORTS_D3D12 YES)
endif()

# Append the drivers C++ files if the OS supports it
if(OS_SUPPORTS_METAL)
	set(GPU_SRC_FILES
		${GPU_SRC_FILES}

		${GPU_ROOT}/Drivers/Metal/Buffer.mm
		${GPU_ROOT}/Drivers/Metal/Buffer.hpp
		${GPU_ROOT}/Drivers/Metal/CommandList.mm
		${GPU_ROOT}/Drivers/Metal/CommandList.hpp
		${GPU_ROOT}/Drivers/Metal/Conversion.hpp
		${GPU_ROOT}/Drivers/Metal/Device.mm
		${GPU_ROOT}/Drivers/Metal/Device.hpp
		${GPU_ROOT}/Drivers/Metal/GraphicsPipeline.mm
		${GPU_ROOT}/Drivers/Metal/GraphicsPipeline.hpp
		${GPU_ROOT}/Drivers/Metal/Shader.mm
		${GPU_ROOT}/Drivers/Metal/Shader.hpp
		${GPU_ROOT}/Drivers/Metal/Swapchain.mm
		${GPU_ROOT}/Drivers/Metal/Swapchain.hpp
		${GPU_ROOT}/Drivers/Metal/Texture.mm
		${GPU_ROOT}/Drivers/Metal/Texture.hpp
	)
endif()

add_forge_library(GPU ${GPU_ROOT} ${GPU_SRC_FILES})

# Link the drivers if the OS supports them
if(OS_SUPPORTS_METAL)
	target_link_libraries(GPU Core "-framework AppKit" "-framework Metal" "-framework QuartzCore")
endif()
