file (GLOB TEST_FILES "*.h" "*.cpp")

source_group("" FILES ${TEST_FILES})

include_directories(${XO_INCLUDE_DIR})
add_executable(xo_test ${TEST_FILES})

target_link_libraries(xo_test xo)

set_target_properties(xo_test	PROPERTIES
	PROJECT_LABEL "xo_test"
	OUTPUT_NAME xo_test
    FOLDER "xo" # Place target inside a xo folder in MSVC
	)
