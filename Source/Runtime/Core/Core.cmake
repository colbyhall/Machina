# Source files
set(CORE_SRC_FILES
        ${CORE_ROOT}/Atomic.h
        ${CORE_ROOT}/Atomic.cpp
        ${CORE_ROOT}/Concepts.h
        ${CORE_ROOT}/Core.cmake
        ${CORE_ROOT}/Core.h
        ${CORE_ROOT}/Core.natvis
        ${CORE_ROOT}/Hash.h
        ${CORE_ROOT}/Hash.cpp
        ${CORE_ROOT}/InitializerList.h
        ${CORE_ROOT}/Memory.h
        ${CORE_ROOT}/Memory.cpp
        ${CORE_ROOT}/Primitives.h
        ${CORE_ROOT}/TypeTraits.h

        ${CORE_ROOT}/Containers/Array.h
        ${CORE_ROOT}/Containers/Array.cpp
        ${CORE_ROOT}/Containers/Function.h
        ${CORE_ROOT}/Containers/Function.cpp
        ${CORE_ROOT}/Containers/HashMap.h
        ${CORE_ROOT}/Containers/HashMap.cpp
        ${CORE_ROOT}/Containers/NonNull.h
        ${CORE_ROOT}/Containers/NonNull.cpp
        ${CORE_ROOT}/Containers/Option.h
        ${CORE_ROOT}/Containers/Option.cpp
        ${CORE_ROOT}/Containers/Shared.h
        ${CORE_ROOT}/Containers/Shared.cpp
        ${CORE_ROOT}/Containers/Slice.h
        ${CORE_ROOT}/Containers/Slice.cpp
        ${CORE_ROOT}/Containers/String.h
        ${CORE_ROOT}/Containers/String.cpp
        ${CORE_ROOT}/Containers/StringView.h
        ${CORE_ROOT}/Containers/StringView.cpp
        ${CORE_ROOT}/Containers/Unique.h
        ${CORE_ROOT}/Containers/Unique.cpp
        ${CORE_ROOT}/Containers/WString.h
        ${CORE_ROOT}/Containers/WString.cpp
        ${CORE_ROOT}/Containers/WStringView.h
        ${CORE_ROOT}/Containers/WStringView.cpp

        ${CORE_ROOT}/Debug/Assertions.h
        ${CORE_ROOT}/Debug/Test.h

        ${CORE_ROOT}/Math/Math.h
        ${CORE_ROOT}/Math/Math.cpp
        ${CORE_ROOT}/Math/Matrix4.h
        ${CORE_ROOT}/Math/Matrix4.cpp
        ${CORE_ROOT}/Math/Quaternion.h
        ${CORE_ROOT}/Math/Quaternion.cpp
        ${CORE_ROOT}/Math/Vector2.h
        ${CORE_ROOT}/Math/Vector2.cpp
        ${CORE_ROOT}/Math/Vector3.h
        ${CORE_ROOT}/Math/Vector3.cpp
        ${CORE_ROOT}/Math/Vector4.h
        ${CORE_ROOT}/Math/Vector4.cpp
)

add_runtime_library(Core ${CORE_ROOT} ${CORE_SRC_FILES})
test_runtime_library(Core ${CORE_SRC_FILES})
