# Set the root
set(GUI_ROOT ${RUNTIME_ROOT}/Gui)

# Source files
set(GUI_SRC_FILES
	${GUI_ROOT}/Application.hpp
	${GUI_ROOT}/Gui.cmake
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

add_runtime_library(Gui ${GUI_ROOT} ${GUI_SRC_FILES})

if (APPLE)
target_link_libraries(Gui Core "-framework AppKit")
elseif(WIN32)
target_link_libraries(Gui Core user32)
endif()
