# https://github.com/doctest/doctest

# Set the root
set(DOCTEST_ROOT ${THIRD_PARTY_ROOT}/Doctest)

# Source files
set(DOCTEST_SRC_FILES
        ${DOCTEST_ROOT}/Doctest.cmake
        ${DOCTEST_ROOT}/Doctest.h
        ${DOCTEST_ROOT}/Doctest.cpp
)

add_third_party_library(Doctest ${DOCTEST_ROOT} ${DOCTEST_SRC_FILES})
