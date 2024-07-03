function(add_runtime_library target root)
    add_library(${target} ${ARGN})
    target_include_directories(${target} PRIVATE ${RUNTIME_ROOT} ${THIRD_PARTY_ROOT})
    set_target_properties(${target} PROPERTIES FOLDER "runtime")
    source_group(TREE ${root} FILES ${ARGN})
    set_target_properties(${target} PROPERTIES LINKER_LANGUAGE CXX)
endfunction()

include(${RUNTIME_ROOT}/core/core.cmake)