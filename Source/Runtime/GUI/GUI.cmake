# Set the root
set(GUI_ROOT ${RUNTIME_ROOT}/GUI)

# Source files
set(GUI_SRC_FILES
	${GUI_ROOT}/Application.hpp
	${GUI_ROOT}/GUI.cmake
	${GUI_ROOT}/Window.hpp
)

if (APPLE)
	set(GUI_SRC_FILES
		${GUI_SRC_FILES}
		${GUI_ROOT}/MacOS/Application.mm
		${GUI_ROOT}/MacOS/Window.hpp
		${GUI_ROOT}/MacOS/Window.mm
	)
elseif(WIN32)
	set(GUI_SRC_FILES
		${GUI_SRC_FILES}
		${GUI_ROOT}/Win32/Application.cpp
		${GUI_ROOT}/Win32/Window.hpp
		${GUI_ROOT}/Win32/Window.cpp
	)
endif()

add_runtime_library(GUI ${GUI_ROOT} ${GUI_SRC_FILES})
target_link_libraries(GUI Core GPU)

if (APPLE)
	target_link_libraries(GUI "-framework AppKit")
elseif(WIN32)
	target_link_libraries(GUI user32)
endif()
