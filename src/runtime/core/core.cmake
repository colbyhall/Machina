# Set the root
set(CORE_ROOT ${RUNTIME_ROOT}/core)

# Source files
set(CORE_SRC_FILES
        ${CORE_ROOT}/core.cpp
)

add_runtime_library(core ${CORE_ROOT} ${CORE_SRC_FILES})