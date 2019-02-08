SET ( XO_LIB_NAMES
	container
	diagnose
	filesystem
	geometry
	numerical
	serialization
	string
	system
	time
	utility
	)

foreach(xolib ${XO_LIB_NAMES})
  file(GLOB XO_${xolib} ${xolib}/*.h ${xolib}/*.cpp)
  list(APPEND XO_SOURCES ${XO_${xolib}})
endforeach()

include( PrecompiledHeaders )
use_precompiled_headers( XO_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/stdafx.h" "${CMAKE_CURRENT_SOURCE_DIR}/stdafx.cpp" 256 )

add_library(xo SHARED ${XO_SOURCES} )

set_target_properties(xo PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)

target_include_directories(xo PUBLIC ${XO_INCLUDE_DIR})

if (UNIX)
    target_link_libraries(xo pthread)
endif()

if (MSVC)
	source_group("" FILES ${PRECOMPILED_HEADER_FILES})
	foreach(xolib ${XO_LIB_NAMES})
	source_group("${xolib}" FILES ${XO_${xolib}} )
	endforeach()
endif()

if (MSVC)
	target_compile_definitions( xo PRIVATE XO_EXPORTS )
endif()

set_target_properties(xo PROPERTIES 
	PROJECT_LABEL "xo"
	OUTPUT_NAME xo
    FOLDER "xo"
	)
