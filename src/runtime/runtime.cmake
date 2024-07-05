# Special case: Set the core root early so it may be used in the 'test_runtime_library' function
set(CORE_ROOT ${RUNTIME_ROOT}/core)

function(add_runtime_library target root)
    add_library(${target} ${ARGN})
    target_include_directories(${target} PRIVATE ${RUNTIME_ROOT} ${THIRD_PARTY_ROOT})
    set_target_properties(${target} PROPERTIES FOLDER "runtime")
    source_group(TREE ${root} FILES ${ARGN})
    set_target_properties(${target} PROPERTIES LINKER_LANGUAGE CXX)
endfunction()

# Creates a test executable for the given runtime library.
#
# We include a main.cpp in the library to build an executable that will run the test in the given library. We also maintain
# the libraries the given library is linked to along with the doctest library.
#
# Finally, we turn on the compilation flag OP_ENABLE_TEST to enable the test code to be compiled in the library.
function(test_runtime_library target)
    add_executable(${target}_test ${ARGN} ${CORE_ROOT}/debug/test_main.cpp)
    target_include_directories(${target}_test PRIVATE ${RUNTIME_ROOT} ${THIRD_PARTY_ROOT})
    set_target_properties(${target}_test PROPERTIES LINKER_LANGUAGE CXX)
    get_target_property(OUT ${target}_test LINK_LIBRARIES)
    target_compile_definitions(${target}_test PRIVATE OP_ENABLE_TEST)
    if (NOT OUT)
        target_link_libraries(${target}_test doctest)
    else ()
        target_link_libraries(${target}_test doctest ${OUT})
    endif ()
    add_test(${target}_test ${target}_test)
endfunction()

include(${RUNTIME_ROOT}/core/core.cmake)