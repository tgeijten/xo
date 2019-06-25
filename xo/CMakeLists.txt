SET ( XO_LIB_NAMES
	container
	filesystem
	geometry
	shape
	numerical
	serialization
	string
	system
	time
	utility
	)

SET ( XO_ROOT xo_types.h )

foreach(xolib ${XO_LIB_NAMES})
  file(GLOB XO_${xolib} ${xolib}/*.h ${xolib}/*.cpp)
  list(APPEND XO_SOURCES ${XO_${xolib}})
endforeach()

list ( APPEND XO_SOURCES ${XO_ROOT} )

include( PrecompiledHeaders )
use_precompiled_headers( XO_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/stdafx.h" "${CMAKE_CURRENT_SOURCE_DIR}/stdafx.cpp" 256 )

add_library(xo SHARED ${XO_SOURCES} )

# Require C++17 standard
set_target_properties(xo PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)

target_include_directories(xo PUBLIC ${XO_INCLUDE_DIR})

if (UNIX)
    target_link_libraries(xo pthread)
endif()

if (MSVC)
	source_group( "" FILES ${PRECOMPILED_HEADER_FILES} ${XO_ROOT} )
	foreach(xolib ${XO_LIB_NAMES})
	source_group("${xolib}" FILES ${XO_${xolib}} )
	endforeach()
	target_compile_definitions( xo PRIVATE XO_EXPORTS )
	target_compile_options( xo PRIVATE "/MP" ) # multithreaded compilation on MSVC
endif()

set_target_properties(xo PROPERTIES 
	PROJECT_LABEL "xo"
	OUTPUT_NAME xo
    FOLDER "xo"
	)
