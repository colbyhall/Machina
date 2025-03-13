# Set the src root
set(SOURCE_ROOT ${FORGE_ROOT}/Source)

# Set the programs root
set(PROGRAMS_ROOT ${SOURCE_ROOT}/Programs)

# Set the runtime root
set(RUNTIME_ROOT ${SOURCE_ROOT}/Runtime)

# Set the third_party root
set(THIRD_PARTY_ROOT ${SOURCE_ROOT}/ThirdParty)

# Enable folders for visual studio
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include(${SOURCE_ROOT}/Runtime/Runtime.cmake)
include(${SOURCE_ROOT}/ThirdParty/ThirdParty.cmake)
include(${SOURCE_ROOT}/Programs/Programs.cmake)
