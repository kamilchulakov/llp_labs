add_library(
        db
        STATIC
        bson.h
        document.c
        document.h
        query.h
        collection.h
        db_file.c
        db_file.h
        page.c
        page.h
        pager.c
        pager.h
        element.c
        element.h
        types.h
        types.c
        io.h
        operation.h
        api.c
        api.h query_executor.c query_executor.h schema.c schema.h bson.c logger.c logger.h)
add_executable(app main.c)
target_link_libraries(app db)
add_custom_command(TARGET app POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:app> ${PROJECT_SOURCE_DIR}/app)