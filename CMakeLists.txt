cmake_minimum_required(VERSION 3.20)

project(c166_tests NONE)

enable_testing()

set(C166_TEST_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")
include(cmake/C166Tests.cmake)
