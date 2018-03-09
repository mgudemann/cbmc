file(GLOB_RECURSE sources "*.cpp" "*.h")
add_library(analyses SHARED ${sources})

generic_includes(analyses)

target_link_libraries(analyses util pointer-analysis)
