# Special case: Set the core root early so it may be used in the 'test_runtime_library' function
set(CORE_ROOT ${RUNTIME_ROOT}/Core)

function(add_runtime_library target root)
    add_library(${target} ${ARGN})
    target_include_directories(${target} PRIVATE ${RUNTIME_ROOT} ${THIRD_PARTY_ROOT})
    set_target_properties(${target} PROPERTIES FOLDER "Runtime")
    source_group(TREE ${root} FILES ${ARGN})
    set_target_properties(${target} PROPERTIES LINKER_LANGUAGE CXX)
endfunction()

# Creates a test executable for the given runtime library.
#
# We include a main.cpp in the library to build an executable that will run the test in the given library. We also maintain
# the libraries the given library is linked to along with the doctest library.
#
# Finally, we turn on the compilation flag GRIZZLY_ENABLE_TEST to enable the test code to be compiled in the library.
function(test_runtime_library target)
    add_executable(${target}Test ${ARGN} ${CORE_ROOT}/Debug/TestMain.cpp)
    target_include_directories(${target}Test PRIVATE ${RUNTIME_ROOT} ${THIRD_PARTY_ROOT})
    set_target_properties(${target}Test PROPERTIES LINKER_LANGUAGE CXX)
    get_target_property(OUT ${target}Test LINK_LIBRARIES)
    target_compile_definitions(${target}Test PRIVATE GRIZZLY_ENABLE_TEST)
    if (NOT OUT)
        target_link_libraries(${target}Test Doctest)
    else ()
        target_link_libraries(${target}Test Doctest ${OUT})
    endif ()
    add_test(${target}Test ${target}Test)
endfunction()

include(${RUNTIME_ROOT}/Core/Core.cmake)
include(${RUNTIME_ROOT}/Gui/Gui.cmake)
