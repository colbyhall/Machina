# Source files
set(CORE_SRC_FILES
        ${CORE_ROOT}/assertions.h
        ${CORE_ROOT}/concepts.h
        ${CORE_ROOT}/core.h
        ${CORE_ROOT}/core.cpp
        ${CORE_ROOT}/primitives.h
        ${CORE_ROOT}/test.h
        ${CORE_ROOT}/type_traits.h
)

add_runtime_library(core ${CORE_ROOT} ${CORE_SRC_FILES})
test_runtime_library(core ${CORE_SRC_FILES})