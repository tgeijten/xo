file (GLOB TEST_FILES "*.h" "*.cpp")

include( PrecompiledHeaders )
use_precompiled_headers( TEST_FILES "${CMAKE_CURRENT_SOURCE_DIR}/stdafx.h" "${CMAKE_CURRENT_SOURCE_DIR}/stdafx.cpp" 256 )

add_executable(xo_test ${TEST_FILES})

target_link_libraries(xo_test xo)

set_target_properties(xo_test PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)

set_target_properties(xo_test PROPERTIES
	PROJECT_LABEL "xo_test"
	OUTPUT_NAME xo_test
    FOLDER "xo" # Place target inside a xo folder in MSVC
	)

if (MSVC)
	source_group( "" FILES ${TEST_FILES} )
	target_compile_options( xo_test PRIVATE "/MP" ) # multithreaded compilation on MSVC
	target_compile_definitions( xo_test PRIVATE "$<$<CONFIG:RELWITHDEBINFO>:XO_ENABLE_PROFILER>" )
endif()
