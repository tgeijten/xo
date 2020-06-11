file (GLOB TEST_FILES "*.h" "*.cpp")

add_executable(xo_test ${TEST_FILES})

target_link_libraries(xo_test xo)

set_target_properties(xo_test PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)

if (MSVC)
	target_precompile_headers(xo_test PRIVATE <string> <vector> <utility> <algorithm> <memory> <limits> <initializer_list>)
	file (GLOB_RECURSE PRECOMPILED_HEADER_FILES ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/cmake_pch.*)
	source_group("CMakePCH" FILES ${PRECOMPILED_HEADER_FILES})
	source_group( "" FILES ${TEST_FILES} )
	target_compile_options( xo_test PRIVATE "/MP" ) # multithreaded compilation on MSVC
	target_compile_definitions( xo_test PRIVATE "$<$<CONFIG:RELWITHDEBINFO>:XO_ENABLE_PROFILER>" )
	#set_target_properties(xo_test PROPERTIES FOLDER "test")
endif()
