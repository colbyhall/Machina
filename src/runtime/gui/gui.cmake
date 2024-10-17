# Set the root
set(GUI_ROOT ${RUNTIME_ROOT}/gui)

# Source files
set(GUI_SRC_FILES
	${GUI_ROOT}/application.h
	${GUI_ROOT}/application.mm
	${GUI_ROOT}/gui.cmake
)

add_runtime_library(gui ${GUI_ROOT} ${GUI_SRC_FILES})
