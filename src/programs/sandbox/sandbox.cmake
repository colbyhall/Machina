# Set the root
set(SANDBOX_ROOT ${PROGRAMS_ROOT}/sandbox)

# Source files
set(SANDBOX_SRC_FILES
	${SANDBOX_ROOT}/main.cpp
	${SANDBOX_ROOT}/sandbox.cmake
)

add_grizzly_executable(sandbox ${SANDBOX_ROOT} ${SANDBOX_SRC_FILES})
target_link_libraries(sandbox gui)
