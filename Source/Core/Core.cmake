# Source files
set(CORE_SRC_FILES
        ${CORE_ROOT}/Atomic.hpp
        ${CORE_ROOT}/Atomic.cpp
        ${CORE_ROOT}/Concepts.hpp
        ${CORE_ROOT}/Core.cmake
		${CORE_ROOT}/Core.hpp
        ${CORE_ROOT}/Core.cpp
        ${CORE_ROOT}/Core.natvis
		${CORE_ROOT}/Format.hpp
		${CORE_ROOT}/Format.cpp
        ${CORE_ROOT}/Hash.hpp
        ${CORE_ROOT}/Hash.cpp
        ${CORE_ROOT}/InitializerList.hpp
        ${CORE_ROOT}/Memory.hpp
        ${CORE_ROOT}/Memory.cpp
        ${CORE_ROOT}/Primitives.hpp
        ${CORE_ROOT}/Time.hpp
        ${CORE_ROOT}/Time.cpp
        ${CORE_ROOT}/TypeTraits.hpp

        ${CORE_ROOT}/Async/Fiber.hpp
        ${CORE_ROOT}/Async/Fiber.cpp
		${CORE_ROOT}/Async/MPMC.hpp
		${CORE_ROOT}/Async/Mutex.hpp
		${CORE_ROOT}/Async/Mutex.cpp
		${CORE_ROOT}/Async/Scheduler.hpp
		${CORE_ROOT}/Async/Scheduler.cpp
        ${CORE_ROOT}/Async/Thread.hpp
        ${CORE_ROOT}/Async/Thread.cpp

        ${CORE_ROOT}/Containers/Array.hpp
        ${CORE_ROOT}/Containers/Array.cpp
        ${CORE_ROOT}/Containers/Function.hpp
        ${CORE_ROOT}/Containers/Function.cpp
        ${CORE_ROOT}/Containers/HashMap.hpp
        ${CORE_ROOT}/Containers/HashMap.cpp
        ${CORE_ROOT}/Containers/NonNull.hpp
        ${CORE_ROOT}/Containers/NonNull.cpp
        ${CORE_ROOT}/Containers/Option.hpp
        ${CORE_ROOT}/Containers/Option.cpp
        ${CORE_ROOT}/Containers/Shared.hpp
        ${CORE_ROOT}/Containers/Shared.cpp
        ${CORE_ROOT}/Containers/Slice.hpp
        ${CORE_ROOT}/Containers/Slice.cpp
        ${CORE_ROOT}/Containers/String.hpp
        ${CORE_ROOT}/Containers/String.cpp
        ${CORE_ROOT}/Containers/StringView.hpp
        ${CORE_ROOT}/Containers/StringView.cpp
        ${CORE_ROOT}/Containers/Unique.hpp
        ${CORE_ROOT}/Containers/Unique.cpp
        ${CORE_ROOT}/Containers/WString.hpp
        ${CORE_ROOT}/Containers/WString.cpp
        ${CORE_ROOT}/Containers/WStringView.hpp
        ${CORE_ROOT}/Containers/WStringView.cpp

        ${CORE_ROOT}/Debug/Assertions.hpp
        ${CORE_ROOT}/Debug/Log.hpp
		${CORE_ROOT}/Debug/StackTrace.hpp
		${CORE_ROOT}/Debug/StackTrace.cpp
        ${CORE_ROOT}/Debug/Test.hpp

		${CORE_ROOT}/IO/File.hpp
		${CORE_ROOT}/IO/Directory.hpp
		${CORE_ROOT}/IO/Reader.hpp
		${CORE_ROOT}/IO/Writer.hpp

        ${CORE_ROOT}/Math/Math.hpp
        ${CORE_ROOT}/Math/Math.cpp
        ${CORE_ROOT}/Math/Matrix4.hpp
        ${CORE_ROOT}/Math/Matrix4.cpp
        ${CORE_ROOT}/Math/Quaternion.hpp
        ${CORE_ROOT}/Math/Quaternion.cpp
        ${CORE_ROOT}/Math/Vector2.hpp
        ${CORE_ROOT}/Math/Vector2.cpp
        ${CORE_ROOT}/Math/Vector3.hpp
        ${CORE_ROOT}/Math/Vector3.cpp
        ${CORE_ROOT}/Math/Vector4.hpp
        ${CORE_ROOT}/Math/Vector4.cpp

        ${CORE_ROOT}/ObjectiveC/Interface.hpp
        ${CORE_ROOT}/ObjectiveC/Protocol.hpp
)

if (APPLE)
	set(CORE_SRC_FILES
		${CORE_SRC_FILES}

		${CORE_ROOT}/Async/Posix/Thread.hpp
		${CORE_ROOT}/Async/Posix/Thread.cpp

		${CORE_ROOT}/IO/Posix/File.hpp
		${CORE_ROOT}/IO/Posix/File.cpp
		${CORE_ROOT}/IO/Posix/Directory.hpp
		${CORE_ROOT}/IO/Posix/Directory.cpp
	)
elseif(WIN32)
	set(CORE_SRC_FILES
		${CORE_SRC_FILES}

		${CORE_ROOT}/Async/Win32/Thread.hpp
		${CORE_ROOT}/Async/Win32/Thread.cpp

		${CORE_ROOT}/IO/Win32/File.hpp
		${CORE_ROOT}/IO/Win32/File.cpp
		${CORE_ROOT}/IO/Win32/Directory.hpp
		${CORE_ROOT}/IO/Win32/Directory.cpp
	)
endif()

add_forge_library(Core ${CORE_ROOT} ${CORE_SRC_FILES})
test_forge_library(Core ${CORE_SRC_FILES})

if (WIN32)
	# Link dbghelp for call stack symbol loading
	target_link_libraries(Core PRIVATE "dbghelp")
endif()
