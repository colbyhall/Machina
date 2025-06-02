# Set the src root
set(SOURCE_ROOT ${MACHINA_ROOT}/Source)

function(add_machina_executable target root)
	add_executable(${target} ${ARGN} ${CORE_ROOT}/Main.cpp)
	target_include_directories(${target} PRIVATE ${SOURCE_ROOT})
    source_group(TREE ${root} FILES ${ARGN})

	# Append this list of programs to sign if on MacOS
	if(APPLE)
		set(TARGETS_TO_SIGN ${TARGETS_TO_SIGN} ${target} PARENT_SCOPE)
	endif()
endfunction()

# Special case: Set the core root early so it may be used in the 'test_runtime_library' function
set(CORE_ROOT ${SOURCE_ROOT}/Core)

function(add_machina_library target root)
    add_library(${target} ${ARGN})
	target_include_directories(${target} PRIVATE ${SOURCE_ROOT})
    source_group(TREE ${root} FILES ${ARGN})
endfunction()

# Creates a test executable for the given library.
#
# We include a main.cpp in the library to build an executable that will run the test in the given library. We also maintain
# the libraries the given library is linked to along with the doctest library.
#
# Finally, we turn on the compilation flag MACHINA_ENABLE_TEST to enable the test code to be compiled in the library.
function(test_machina_library target)
    add_executable(${target}Test ${ARGN} ${CORE_ROOT}/Debug/TestMain.cpp)
	target_include_directories(${target}Test PRIVATE ${SOURCE_ROOT})
    set_target_properties(${target}Test PROPERTIES LINKER_LANGUAGE CXX)
    get_target_property(OUT ${target}Test LINK_LIBRARIES)
    target_compile_definitions(${target}Test PRIVATE MACHINA_ENABLE_TEST)
    if (NOT OUT)
		target_link_libraries(${target}Test Doctest Core)
    else ()
		target_link_libraries(${target}Test Doctest Core ${OUT})
    endif ()
    add_test(${target}Test ${target}Test)
endfunction()

include(${SOURCE_ROOT}/Core/Core.cmake)
include(${SOURCE_ROOT}/Doctest/Doctest.cmake)
include(${SOURCE_ROOT}/DXC/DXC.cmake)
include(${SOURCE_ROOT}/GPU/GPU.cmake)
include(${SOURCE_ROOT}/GUI/GUI.cmake)
include(${SOURCE_ROOT}/Sandbox/Sandbox.cmake)
