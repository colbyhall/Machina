function(add_grizzly_executable target root)
    add_executable(${target} ${ARGN})
    target_include_directories(${target} PRIVATE ${RUNTIME_ROOT} ${THIRD_PARTY_ROOT})
    set_target_properties(${target} PROPERTIES FOLDER "programs")
    source_group(TREE ${root} FILES ${ARGN})
endfunction()

include(${PROGRAMS_ROOT}/sandbox/sandbox.cmake)
