# Source files
set(CORE_SRC_FILES
        ${CORE_ROOT}/assertions.h
        ${CORE_ROOT}/concepts.h
        ${CORE_ROOT}/core.h
        ${CORE_ROOT}/memory.h
        ${CORE_ROOT}/memory.cpp
        ${CORE_ROOT}/primitives.h
        ${CORE_ROOT}/test.h
        ${CORE_ROOT}/type_traits.h

        ${CORE_ROOT}/containers/array.h
        ${CORE_ROOT}/containers/array.cpp
        ${CORE_ROOT}/containers/non_null.h
        ${CORE_ROOT}/containers/non_null.cpp
        ${CORE_ROOT}/containers/option.h
        ${CORE_ROOT}/containers/option.cpp
)

add_runtime_library(core ${CORE_ROOT} ${CORE_SRC_FILES})
test_runtime_library(core ${CORE_SRC_FILES})