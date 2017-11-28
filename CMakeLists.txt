cmake_minimum_required(VERSION 2.8.11)
# 2.8.11 is required for how we use Qt5.

project(xo)

# To create a folder hierarchy within Visual Studio.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

find_path(XO_INCLUDE_DIR
    NAMES xo/system/build_config.hpp
	PATHS ${CMAKE_CURRENT_SOURCE_DIR}
	)

# Check required compiler versions
# ---------------
if (${CMAKE_CXX_COMPILER_ID} MATCHES "GNU")
	set(XO_REQUIRED_GCC_VERSION 5.1.0)
	if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS ${XO_REQUIRED_GCC_VERSION})
		message(FATAL_ERROR "GNU GCC/G++ version must be at least version: "
							"${XO_REQUIRED_GCC_VERSION}. GCC version is "
                            "${CMAKE_CXX_COMPILER_VERSION}.")
	endif()
endif()
	
# Compiler flags.
# ---------------
# Must compile with C++14 with gcc/clang.
if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR
   ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    if(APPLE)
        if(XCODE)
            set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++14")
            set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
        else()
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
            if(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
                set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
            endif()
        endif()
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
    endif()
endif()
	
# Place build products (libraries, executables) in root
# binary (build) directory. Otherwise, they get scattered around
# the build directory and so the dll's aren't next to the executables.
get_filename_component(CONFIG_NAME "${CMAKE_BINARY_DIR}" NAME)
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin/${CONFIG_NAME}")
set(LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin/${CONFIG_NAME}")

# Process source code.
add_subdirectory(xo)
add_subdirectory(test)

enable_testing()
add_test(xo_test "${CMAKE_SOURCE_DIR}/bin/${CONFIG_NAME}/xo_test")
