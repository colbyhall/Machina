function(add_grizzly_executable target root)
    add_executable(${target} ${ARGN})
    target_include_directories(${target} PRIVATE ${RUNTIME_ROOT} ${THIRD_PARTY_ROOT})
    set_target_properties(${target} PROPERTIES FOLDER "Programs")
    source_group(TREE ${root} FILES ${ARGN})

	# Append this list of programs to sign if on MacOS
	if(APPLE)
		set(TARGETS_TO_SIGN ${TARGETS_TO_SIGN} ${target} PARENT_SCOPE)
	endif()
endfunction()

include(${PROGRAMS_ROOT}/Sandbox/Sandbox.cmake)
