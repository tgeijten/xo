file (GLOB TEST_FILES "*.h" "*.cpp")

include_directories(${XO_INCLUDE_DIR})
add_executable(xo_test ${TEST_FILES})

target_link_libraries(xo_test xo)

set_property(TARGET xo_test PROPERTY CXX_STANDARD 17)
set_property(TARGET xo_test PROPERTY CXX_STANDARD_REQUIRED ON)

set_target_properties(xo_test PROPERTIES
	PROJECT_LABEL "xo_test"
	OUTPUT_NAME xo_test
    FOLDER "xo" # Place target inside a xo folder in MSVC
	)

target_compile_definitions( xo_test PRIVATE "$<$<CONFIG:RELWITHDEBINFO>:XO_ENABLE_PROFILER>" )
