add_executable(example_schema example_schema.c)
target_link_libraries(example_schema db)
add_custom_command(TARGET example_schema POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:example_schema> ../example_schema)