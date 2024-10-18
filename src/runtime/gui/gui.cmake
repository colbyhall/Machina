# Set the root
set(GUI_ROOT ${RUNTIME_ROOT}/gui)

# Source files
set(GUI_SRC_FILES
	${GUI_ROOT}/application.h
	${GUI_ROOT}/macos_application.mm
	${GUI_ROOT}/gui.cmake
	${GUI_ROOT}/window.h
	${GUI_ROOT}/macos_window.h
	${GUI_ROOT}/macos_window.mm
)

add_runtime_library(gui ${GUI_ROOT} ${GUI_SRC_FILES})
target_link_libraries(gui core "-framework AppKit")
