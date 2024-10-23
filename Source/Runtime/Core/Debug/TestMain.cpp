/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Debug/Test.h>

/**
 * This file is the main.cpp for every runtime library that uses our testing framework. It is included in all libraries
 * that have tests enabled when building their test executable.
 *
 * @see src/runtime/runtime.cmake for how to enable testing on runtime libraries.
 */
int main(int argc, char** argv) { return doctest::Context{ argc, argv }.run(); }
