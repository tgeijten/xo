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
use_precompiled_headers( XO_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/stdafx.h" "stdafx.cpp" 256 )

add_library(xo SHARED ${XO_SOURCES} )

if (UNIX)
    target_link_libraries(xo pthread)
endif()

if (MSVC)
	foreach(xolib ${XO_LIB_NAMES})
	source_group("${xolib}" FILES ${XO_${xolib}} )
	endforeach()
	source_group("" FILES "${CMAKE_CURRENT_SOURCE_DIR}/stdafx.h" "${CMAKE_CURRENT_SOURCE_DIR}/stdafx.cpp" )
endif()

include_directories(${XO_INCLUDE_DIR})

if (MSVC)
	target_compile_definitions( xo PRIVATE XO_EXPORTS )
endif()

set_target_properties(xo PROPERTIES 
	PROJECT_LABEL "xo"
	OUTPUT_NAME xo
    FOLDER "xo"
	)
