cmake_minimum_required(VERSION 3.10.0)

SET (XO_LIB_NAMES
	container
	filesystem
	geometry
	shape
	numerical
	serialization
	string
	system
	thread
	time
	utility
	)

SET (XO_ROOT xo_types.h)

foreach(xolib ${XO_LIB_NAMES})
	file(GLOB XO_${xolib} ${xolib}/*.h ${xolib}/*.cpp)
	list(APPEND XO_SOURCES ${XO_${xolib}})
endforeach()

list ( APPEND XO_SOURCES ${XO_ROOT} )

add_library(xo SHARED ${XO_SOURCES} )

set_target_properties(xo PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)

target_include_directories(xo PUBLIC ${XO_INCLUDE_DIR})

if (UNIX)
    target_link_libraries(xo pthread)
endif()

if (MSVC)
	target_precompile_headers(xo PRIVATE <string> <vector> <algorithm> <memory> <limits>)
	file (GLOB_RECURSE PRECOMPILED_HEADER_FILES ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/cmake_pch.*)
	source_group("CMakePCH" FILES ${PRECOMPILED_HEADER_FILES})
	source_group("" FILES ${XO_ROOT})
	source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${XO_SOURCES} )
	foreach(xolib ${XO_LIB_NAMES})
		source_group("${xolib}" FILES ${XO_${xolib}} )
	endforeach()
	target_compile_definitions(xo PRIVATE XO_EXPORTS)
	target_compile_options(xo PRIVATE "/MP") # multithreaded compilation on MSVC
	#set_target_properties(xo PROPERTIES FOLDER "xo")
endif()
