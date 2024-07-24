# Source files
set(CORE_SRC_FILES
        ${CORE_ROOT}/concepts.h
        ${CORE_ROOT}/core.cmake
        ${CORE_ROOT}/core.h
        ${CORE_ROOT}/core.natvis
        ${CORE_ROOT}/hash.h
        ${CORE_ROOT}/hash.cpp
        ${CORE_ROOT}/initializer_list.h
        ${CORE_ROOT}/memory.h
        ${CORE_ROOT}/memory.cpp
        ${CORE_ROOT}/primitives.h
        ${CORE_ROOT}/type_traits.h

        ${CORE_ROOT}/containers/array.h
        ${CORE_ROOT}/containers/array.cpp
        ${CORE_ROOT}/containers/function.h
        ${CORE_ROOT}/containers/function.cpp
        ${CORE_ROOT}/containers/hash_map.h
        ${CORE_ROOT}/containers/hash_map.cpp
        ${CORE_ROOT}/containers/non_null.h
        ${CORE_ROOT}/containers/non_null.cpp
        ${CORE_ROOT}/containers/option.h
        ${CORE_ROOT}/containers/option.cpp
        ${CORE_ROOT}/containers/shared.h
        ${CORE_ROOT}/containers/shared.cpp
        ${CORE_ROOT}/containers/slice.h
        ${CORE_ROOT}/containers/slice.cpp
        ${CORE_ROOT}/containers/string.h
        ${CORE_ROOT}/containers/string.cpp
        ${CORE_ROOT}/containers/string_view.h
        ${CORE_ROOT}/containers/string_view.cpp
        ${CORE_ROOT}/containers/unique.h
        ${CORE_ROOT}/containers/unique.cpp
        ${CORE_ROOT}/containers/wstring.h
        ${CORE_ROOT}/containers/wstring.cpp
        ${CORE_ROOT}/containers/wstring_view.h
        ${CORE_ROOT}/containers/wstring_view.cpp

        ${CORE_ROOT}/debug/assertions.h
        ${CORE_ROOT}/debug/test.h

        ${CORE_ROOT}/math/math.h
        ${CORE_ROOT}/math/math.cpp
        ${CORE_ROOT}/math/quaternion.h
        ${CORE_ROOT}/math/quaternion.cpp
        ${CORE_ROOT}/math/vector2.h
        ${CORE_ROOT}/math/vector2.cpp
        ${CORE_ROOT}/math/vector3.h
        ${CORE_ROOT}/math/vector3.cpp
        ${CORE_ROOT}/math/vector4.h
        ${CORE_ROOT}/math/vector4.cpp

        ${CORE_ROOT}/sync/atomic.h
        ${CORE_ROOT}/sync/atomic.cpp
)

add_runtime_library(core ${CORE_ROOT} ${CORE_SRC_FILES})
test_runtime_library(core ${CORE_SRC_FILES})