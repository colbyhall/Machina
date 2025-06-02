# https://github.com/microsoft/DirectXShaderCompiler/releases/tag/v1.8.2505

# Set the root
set(DXC_ROOT ${SOURCE_ROOT}/DXC)

# Source files
set(DXC_SRC_FILES
	${DXC_ROOT}/DXC.cmake
	${DXC_ROOT}/DXC.hpp
	${DXC_ROOT}/DXC.cpp
)

add_machina_library(DXC ${DXC_ROOT} ${DXC_SRC_FILES})
target_link_libraries(DXC PUBLIC GPU)

if(ARCH_AARCH64)
	target_link_libraries(DXC PUBLIC ${DXC_ROOT}/lib/arm64/dxcompiler ${DXC_ROOT}/lib/arm64/dxil)
elseif(ARCH_X86_64)
	target_link_libraries(DXC PUBLIC ${DXC_ROOT}/lib/x64/dxcompiler ${DXC_ROOT}/lib/x64/dxil)
endif()
