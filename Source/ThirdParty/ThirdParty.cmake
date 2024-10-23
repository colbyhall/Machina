function(add_third_party_library target root)
    add_library(${target} ${ARGN})
    target_include_directories(${target} PRIVATE ${RUNTIME_ROOT} ${THIRD_PARTY_ROOT})
    set_target_properties(${target} PROPERTIES FOLDER "ThirdParty")
    source_group(TREE ${root} FILES ${ARGN})
endfunction()

include(${THIRD_PARTY_ROOT}/Doctest/Doctest.cmake)
