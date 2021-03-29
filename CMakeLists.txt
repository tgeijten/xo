cmake_minimum_required(VERSION 3.10)

project(xo)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${CMAKE_SOURCE_DIR}/CMakeModules")

# To create a folder hierarchy within Visual Studio.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

find_path(XO_INCLUDE_DIR
    NAMES xo/xo_types.h
	PATHS ${CMAKE_CURRENT_SOURCE_DIR}
	)

option(XO_TEST_ENABLED "Build and add xo_test" ON)
option(XO_LUA_ENABLED "Add xo_lua" OFF)

# Process source code.
add_subdirectory(xo)

if (XO_LUA_ENABLED)
	add_subdirectory(xo_lua)
endif()

if (XO_TEST_ENABLED)
	add_subdirectory(xo_test)
endif()
