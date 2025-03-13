# Set the root
set(SANDBOX_ROOT ${SOURCE_ROOT}/Sandbox)

# Source files
set(SANDBOX_SRC_FILES
	${SANDBOX_ROOT}/Main.cpp
	${SANDBOX_ROOT}/Sandbox.cmake
)

add_forge_executable(Sandbox ${SANDBOX_ROOT} ${SANDBOX_SRC_FILES})
target_link_libraries(Sandbox GUI)
