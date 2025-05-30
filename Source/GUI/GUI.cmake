# Set the root
set(GUI_ROOT ${SOURCE_ROOT}/GUI)

# Source files
set(GUI_SRC_FILES
	${GUI_ROOT}/Application.hpp
	${GUI_ROOT}/Application.cpp
	${GUI_ROOT}/GUI.cmake
	${GUI_ROOT}/Window.hpp

	${GUI_ROOT}/Builder.hpp
	${GUI_ROOT}/Builder.cpp
	${GUI_ROOT}/Canvas.hpp
	${GUI_ROOT}/Canvas.cpp
	${GUI_ROOT}/Layout.hpp
	${GUI_ROOT}/Layout.cpp
	${GUI_ROOT}/State.hpp
	${GUI_ROOT}/State.cpp
	${GUI_ROOT}/Math.hpp
	${GUI_ROOT}/Math.cpp
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

add_machina_library(GUI ${GUI_ROOT} ${GUI_SRC_FILES})
target_link_libraries(GUI Core GPU)

if (OS_MACOS)
	target_link_libraries(GUI "-framework AppKit")
elseif(OS_WINDOWS)
	target_link_libraries(GUI user32)
endif()
