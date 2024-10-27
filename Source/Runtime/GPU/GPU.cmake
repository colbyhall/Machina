# Set the root
set(GPU_ROOT ${RUNTIME_ROOT}/GPU)

# Source files
set(GPU_SRC_FILES
	${GPU_ROOT}/Device.hpp
	${GPU_ROOT}/Device.cpp
	${GPU_ROOT}/Forward.hpp
	${GPU_ROOT}/GPU.cmake
)

if (APPLE)
	set(GPU_SRC_FILES
		${GPU_SRC_FILES}
		${GPU_ROOT}/Metal/Device.mm
		${GPU_ROOT}/Metal/Device.hpp
	)
elseif(WIN32)
endif()

add_runtime_library(GPU ${GPU_ROOT} ${GPU_SRC_FILES})

if (APPLE)
	target_link_libraries(GPU Core "-framework Metal")
elseif(WIN32)
endif()
