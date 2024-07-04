# https://github.com/doctest/doctest

# Set the root
set(DOCTEST_ROOT ${THIRD_PARTY_ROOT}/doctest)

# Source files
set(DOCTEST_SRC_FILES
        ${DOCTEST_ROOT}/doctest.cmake
        ${DOCTEST_ROOT}/doctest.h
        ${DOCTEST_ROOT}/doctest.cpp
)

add_third_party_library(doctest ${DOCTEST_ROOT} ${DOCTEST_SRC_FILES})