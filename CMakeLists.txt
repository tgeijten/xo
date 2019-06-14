cmake_minimum_required(VERSION 3.12)

project(xo)

# To create a folder hierarchy within Visual Studio.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

find_path(XO_INCLUDE_DIR
    NAMES xo/system/build_config.hpp
	PATHS ${CMAKE_CURRENT_SOURCE_DIR}
	)

# Place build products (libraries, executables) in root
# binary (build) directory. Otherwise, they get scattered around
# the build directory and so the dll's aren't next to the executables.
get_filename_component(CONFIG_NAME "${CMAKE_BINARY_DIR}" NAME)
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin/${CONFIG_NAME}")
set(LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin/${CONFIG_NAME}")

option(XO_TEST_ENABLED "Build and add xo_test" ON)
option(XO_LUA_ENABLED "Add xo_lua" OFF)

# Process source code.
add_subdirectory(xo)

if (XO_LUA_ENABLED)
	add_subdirectory(xo_lua)
endif()

if (XO_TEST_ENABLED)
	add_subdirectory(xo_test)
	enable_testing()
	add_test(xo_test "${CMAKE_SOURCE_DIR}/bin/${CONFIG_NAME}/xo_test")
endif()
