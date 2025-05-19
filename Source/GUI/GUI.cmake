# Set the root
set(GUI_ROOT ${SOURCE_ROOT}/GUI)

# Source files
set(GUI_SRC_FILES
	${GUI_ROOT}/Application.hpp
	${GUI_ROOT}/GUI.cmake
	${GUI_ROOT}/Window.hpp
)

if (OS_MACOS)
	set(GUI_SRC_FILES
		${GUI_SRC_FILES}

		${GUI_ROOT}/MacOS/Application.mm
		${GUI_ROOT}/MacOS/Window.hpp
		${GUI_ROOT}/MacOS/Window.mm
	)
elseif(OS_WINDOWS)
	set(GUI_SRC_FILES
		${GUI_SRC_FILES}

		${GUI_ROOT}/Win32/Application.cpp
		${GUI_ROOT}/Win32/Window.hpp
		${GUI_ROOT}/Win32/Window.cpp
	)
endif()

add_forge_library(GUI ${GUI_ROOT} ${GUI_SRC_FILES})
target_link_libraries(GUI Core GPU)

if (OS_MACOS)
	target_link_libraries(GUI "-framework AppKit")
elseif(OS_WINDOWS)
	target_link_libraries(GUI user32)
endif()
