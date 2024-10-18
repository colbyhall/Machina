# Set the root
set(GUI_ROOT ${RUNTIME_ROOT}/gui)

# Source files
set(GUI_SRC_FILES
	${GUI_ROOT}/application.h
	${GUI_ROOT}/gui.cmake
	${GUI_ROOT}/window.h
)

if (APPLE)
	set(GUI_SRC_FILES
		${GUI_SRC_FILES}
		${GUI_ROOT}/macos/application.mm
		${GUI_ROOT}/macos/window.h
		${GUI_ROOT}/macos/window.mm
	)
elseif(WIN32)
	set(GUI_SRC_FILES
		${GUI_SRC_FILES}
		${GUI_ROOT}/win32/application.cpp
		${GUI_ROOT}/win32/window.h
		${GUI_ROOT}/win32/window.cpp
	)
endif()

add_runtime_library(gui ${GUI_ROOT} ${GUI_SRC_FILES})

if (APPLE)
target_link_libraries(gui core "-framework AppKit")
elseif(WIN32)
target_link_libraries(gui core user32)
endif()
