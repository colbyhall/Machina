function(add_runtime_library target root files)
    add_library(${target} ${files})
    target_include_directories(${target} PRIVATE ${RUNTIME_ROOT} ${THIRD_PARTY_ROOT})
    set_target_properties(${target} PROPERTIES FOLDER "runtime")
    source_group(TREE ${root} FILES ${files})
endfunction()

include(${RUNTIME_ROOT}/core/core.cmake)