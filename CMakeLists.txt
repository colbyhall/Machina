cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(Forge LANGUAGES C CXX)

# ------------- Options Declerations -------------
option(GENERATE_DEBUG_SYMBOLS "Generate debug symbols for libraries and executables" ON)
option(ENABLE_ALL_WARNINGS "Enables \"all\" compiler warnings and treats them like errors" ON)
option(GENERATE_COMPILE_COMMANDS "Generates compile_commands.json for clangd" ON)

# Apply GENERATE_COMPILE_COMMANDS to cmake
if(GENERATE_COMPILE_COMMANDS)
	set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

# Declare our own OS variables for readability
set(OS_MACOS NO)
set(OS_WINDOWS NO)
set(OS_LINUX NO)
set(OS_SUPPORTS_POSIX NO)

# Determine what OS variable should be set to true
if(APPLE)
	set(OS_MACOS YES)
	set(OS_SUPPORTS_POSIX YES)
elseif(UNIX AND NOT APPLE)
	set(OS_LINUX NO)
	set(OS_SUPPORTS_POSIX YES)
elseif(WIN32)
	set(OS_WINDOWS YES)
endif()

# Declare our own arch variables for ease of use
set(ARCH_X86_64 NO)
set(ARCH_AARCH64 NO)

# Determine what arch variable should be set to true
if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(x86_64|amd64)$")
	set(ARCH_X86_64 YES)
elseif(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "^(aarch64|AARCH64|arm64|ARM64)$")
	set(ARCH_AARCH64 YES)
endif()

# Throw an error if the CPU arch is not supported
if((NOT ARCH_X86_64) AND (NOT ARCH_AARCH64))
	# message(FATAL_ERROR, "This project does not support the target CPU arch")
endif()

# Setup project settings for MSVC
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    # Only support 64 bit architecture
    set(CMAKE_VS_PLATFORM_TOOLSET_HOST_ARCHITECTURE "x64")

    # ------------- Always On Settings -------------

    # Set C++ standard to C++20
    # https://learn.microsoft.com/en-us/cpp/build/reference/std-specify-language-standard-version
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++20")

    # Set __cplusplus macro to the correct version
    # https://learn.microsoft.com/en-us/cpp/build/reference/zc-cplusplus
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:__cplusplus")

    # Build with multiple processes
    # https://learn.microsoft.com/en-us/cpp/build/reference/mp-build-with-multiple-processes
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")

    # Suppress startup banner
    # https://learn.microsoft.com/en-us/cpp/build/reference/nologo-suppress-startup-banner-c-cpp
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /nologo")

    # Only report the line the error has occurred on
    # https://learn.microsoft.com/en-us/cpp/build/reference/diagnostics-compiler-diagnostic-options
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /diagnostics:classic")

    # Display the full path of source files when displaying diagnostics
    # https://learn.microsoft.com/en-us/cpp/build/reference/fc-full-path-of-source-code-file-in-diagnostics
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /FC")

    # Optimize floating point operations for speed and space
    # https://learn.microsoft.com/en-us/cpp/build/reference/fp-specify-floating-point-behavior
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /fp:fast")

    # Disable floating point exceptions
    # https://learn.microsoft.com/en-us/cpp/build/reference/fp-specify-floating-point-behavior
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /fp:except-")

    # Removed unreferenced COMDAT data
    # https://learn.microsoft.com/en-us/cpp/build/reference/zc-inline-remove-unreferenced-comdat
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:inline")

    # Disable RTTI
    # https://learn.microsoft.com/en-us/cpp/build/reference/gr-enable-run-time-type-information
    string(REPLACE "/GR" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GR-")

    # Enable UTF-8 source and execution character set
    # https://learn.microsoft.com/en-us/cpp/build/reference/utf-8-set-source-and-executable-character-sets-to-utf-8
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")

    # Disable C++ exceptions handling
    # https://learn.microsoft.com/en-us/cpp/build/reference/eh-exception-handling-model
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHa-")

    # Append generate symbols if option is enabled
    # https://learn.microsoft.com/en-us/cpp/build/reference/z7-zi-zi-debug-information-format
    if (GENERATE_DEBUG_SYMBOLS)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi")
    endif ()

    # Append enable all warnings and throw error for warnings if option is enabled
    # https://learn.microsoft.com/en-us/cpp/build/reference/compiler-option-warning-level
    if (ENABLE_ALL_WARNINGS)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Wall /WX")
    endif ()

    # ------------- Always Disabled Warnings -------------
    # Every warning that is disabled here should have a comment explaining why it is disabled. Disabling warnings should
    # be avoided as much as possible

    # Disable warning C4514: 'function' : unreferenced inline function has been removed. Not every function will be used
    # in the current translation unit.
    # https://learn.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-4-c4514
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4514")

    # Disable warning for function not being inlined when marked as inlined. This may not always happen based on
    # compiler optimizations.
    # https://learn.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-4-c4710
    # https://learn.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-4-c4714
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4710 /wd4714")

	# Disable selected MSVC warnings that are either noisy or irrelevant for our codebase:
	# - 5027: Move assignment operator was implicitly deleted (common with const members or deleted base ops)
	#         https://learn.microsoft.com/en-us/cpp/error-messages/compiler-warnings/c5027
	# - 4626: Assignment operator was implicitly deleted (same reasoning as 5027)
	#         https://learn.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-4-c4626
	# - 4623: Destructor was implicitly deleted (common with deleted base destructors or const members)
	#         https://learn.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-4-c4623
	# - 4668: Preprocessor macro is not defined (typical in cross-platform builds when checking Windows macros)
	#         https://learn.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-4-c4668
	# - 5267: Definition of implicit special member function not visible (can happen with incomplete types)
	#         https://learn.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-4-c5267
	# - 4625: Copy constructor was implicitly deleted (often benign, similar to 4626)
	#         https://learn.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-4-c4625
	# - 5039: Potentially unsafe function or object in a security-critical context
	#         (very common in low-level or cross-platform code; warning noise without /sdl)
	#         https://learn.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-4-c5039
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd5027 /wd4626 /wd4623 /wd4668 /wd5267 /wd4625 /wd5039")

    # Disable warning for adding struct padding due to alignment specifier. We want to use the alignment specifier and
    # this will add padding to structures.
    # https://learn.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-4-c4324
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4324")

    # https://learn.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-4-c4820
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4820")

    # Disable warning for use of QSpectre notification. This warning seems to be acting just as a notification.
    # https://learn.microsoft.com/en-us/cpp/error-messages/compiler-warnings/c5045
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd5045")

    # Setup build configurations
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release")

    # ------------- Debug Configuration Settings -------------

    # Enable buffer overrun detection
    # https://learn.microsoft.com/en-us/cpp/build/reference/gs-buffer-security-check
    set(CMAKE_CXX_FLAGS_DEBUG "/GS")

    # Disable all optimizations and enables inline expansion only for functions marked as inline
    # https://learn.microsoft.com/en-us/cpp/build/reference/o-options-optimize-code
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Od /Ob1")

    # Enables stack frame runtime error checking
    # https://learn.microsoft.com/en-us/cpp/build/reference/rtc-run-time-error-checks
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /RTC1")

    # ------------- Release Configuration Settings -------------

    # Disables buffer overrun detection
    # https://learn.microsoft.com/en-us/cpp/build/reference/gs-buffer-security-check
    set(CMAKE_CXX_FLAGS_RELEASE "/GS-")

    # Enable function level linking
    # https://learn.microsoft.com/en-us/cpp/build/reference/gy-enable-function-level-linking
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Gy")

    # Enable maximize speed optimizations
    # https://learn.microsoft.com/en-us/cpp/build/reference/o1-o2-minimize-size-maximize-speed
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2")

    # Enable the use of intrinsic functions
    # https://learn.microsoft.com/en-us/cpp/build/reference/oi-generate-intrinsic-functions
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Oi")

    # Enable favoring fast codeover small code.
    # https://learn.microsoft.com/en-us/cpp/build/reference/os-ot-favor-small-code-favor-fast-code
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Ot")

    # ------------- Default Linker Settings -------------

    # Disable common linker error
    # https://learn.microsoft.com/en-us/cpp/error-messages/tool-errors/linker-tools-warning-lnk4221
    set(CMAKE_EXE_LINKER_FLAGS "/ignore:4221")

    # Generate full debug information in one PDB per library or executable
    # https://learn.microsoft.com/en-us/cpp/build/reference/debug-generate-debug-info
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /DEBUG:FULL")

    # ------------- Release Linker Settings -------------

    # Disable incremental linking
    # https://learn.microsoft.com/en-us/cpp/build/reference/incremental-link-incrementally
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "/INCREMENTAL:NO")

    # Enable link time optimizations. Even with incremental linking disabled, the linker will still perform optimizations
    # https://learn.microsoft.com/en-us/cpp/build/reference/ltcg-link-time-code-generation
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /LTCG:incremental")

    # Remove unreferenced or redundant functions and data
    # https://learn.microsoft.com/en-us/cpp/build/reference/opt-optimizations
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /OPT:ICF /OPT:REF")

    # Enable link time optimizations for static libraries
    # https://learn.microsoft.com/en-us/cpp/build/reference/ltcg-link-time-code-generation
    set(CMAKE_STATIC_LINKER_FLAGS_RELEASE "/LTCG")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release;ReleaseASAN;ReleaseUBSAN;ReleaseCoverage;Distribution")

	# Use C++20
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++20")

    # Append enable all warnings and throw error for warnings if option is enabled
    if (ENABLE_ALL_WARNINGS)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
    endif ()

    # Optionally generate debug symbols
    if (GENERATE_DEBUG_SYMBOLS)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
    endif ()

    # Set compiler flag for disabling RTTI
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")

    # Set compiler flag for disabling exception-handling
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions")

    # Do not use -ffast-math since it cannot be turned off in a single compilation unit under clang, see Core.hpp
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffp-model=fast")

    # On clang 14 and later we can turn off float contraction through a pragma, older versions and deterministic versions need it off always, see Core.hpp
    if (CMAKE_CXX_COMPILER_VERSION LESS 14 OR CROSS_PLATFORM_DETERMINISTIC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffp-contract=off")
    endif ()

    set(CMAKE_CXX_FLAGS_DISTRIBUTION "${CMAKE_CXX_FLAGS_RELEASE}")
    set(CMAKE_CXX_FLAGS_RELEASEASAN "-fsanitize=address")
    set(CMAKE_CXX_FLAGS_RELEASEUBSAN "-fsanitize=undefined,implicit-conversion,float-divide-by-zero,local-bounds -fno-sanitize-recover=all")
    set(CMAKE_CXX_FLAGS_RELEASECOVERAGE "-O0 -DJPH_NO_FORCE_INLINE -fprofile-instr-generate -fcoverage-mapping")
    set(CMAKE_CXX_FLAGS_DEBUG "-D_DEBUG")
else ()
    message(FATAL_ERROR, "Compiler not supported.")
endif ()

message("Using " ${CMAKE_CXX_COMPILER_ID} " (" ${CMAKE_CXX_COMPILER_VERSION} ")")

# Set repository root
set(FORGE_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# Enable testing using CTest
enable_testing()

set(TARGETS_TO_SIGN)

# Include the source directory cmake file
include(${FORGE_ROOT}/Source/Source.cmake)

if (APPLE)
	set(ENTITLEMENTS_FILE "${CMAKE_BINARY_DIR}/Forge.entitlements")
	execute_process(COMMAND /usr/libexec/PlistBuddy -c "Add :com.apple.security.get-task-allow bool true" "${ENTITLEMENTS_FILE}")
	foreach(target "${TARGETS_TO_SIGN}")
		add_custom_command(TARGET "${target}" POST_BUILD
			COMMAND codesign -s - -f --entitlements "${ENTITLEMENTS_FILE}" "$<TARGET_FILE:${target}>"
			COMMENT "sign target \"${target}\" to enable core dump on MacOS"
		VERBATIM
	)
	endforeach()
endif()

