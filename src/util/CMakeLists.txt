file(GLOB_RECURSE sources "*.cpp" "*.h")
add_library(util SHARED ${sources})

generic_includes(util)

target_link_libraries(util big-int)
