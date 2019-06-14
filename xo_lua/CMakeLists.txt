file (GLOB XO_LUA_FILES "*.h" "*.cpp")

add_library(xo_lua STATIC)
set_target_properties(xo_lua PROPERTIES LINKER_LANGUAGE CXX)
target_sources(xo_lua PUBLIC "${XO_LUA_FILES}")
target_include_directories(xo_lua PRIVATE "${XO_INCLUDE_DIR}")

set_target_properties(xo_lua PROPERTIES 
	PROJECT_LABEL "xo_lua"
    FOLDER "xo"
	)

if (MSVC)
	source_group( "" FILES ${XO_LUA_FILES} )
endif()

