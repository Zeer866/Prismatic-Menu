cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Work around Clang 21 consteval diagnostics in fmt 11 used by Geode SDK 4.0.0.
add_compile_definitions(FMT_USE_CONSTEVAL=0)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

project(PrismaticMenu VERSION 0.1.0)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "GEODE_SDK is not set. The Geode build action should provide it.")
endif()

message(STATUS "Using Geode SDK: $ENV{GEODE_SDK}")
add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)
setup_geode_mod(${PROJECT_NAME})
