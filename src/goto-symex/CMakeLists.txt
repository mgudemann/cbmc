file(GLOB_RECURSE sources "*.cpp" "*.h")
add_library(goto-symex SHARED ${sources})

generic_includes(goto-symex)

target_link_libraries(goto-symex util)
