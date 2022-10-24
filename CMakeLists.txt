cmake_minimum_required(VERSION 3.23)
project(enormedb C)

set(CMAKE_C_STANDARD 11)

add_executable(enormedb src/main/main.c src/main/bson.h src/main/document.c src/main/document.h src/main/query.h src/main/collection.h src/main/db_file.c src/main/db_file.h src/main/page.c src/main/page.h src/main/pager.c src/main/pager.h src/main/element.c src/main/element.h src/main/types.h src/main/types.c src/main/io.h src/main/operation.h src/main/api.c src/main/api.h)
add_executable(test src/test/test_runner.c src/main/bson.h src/main/types.h src/main/types.c src/main/io.h src/test/test_document.c src/test/test.h src/test/test_element.c src/test/test_element.h src/main/element.h src/main/element.c src/test/test.c src/test/test_document.h src/main/document.c src/main/document.h)