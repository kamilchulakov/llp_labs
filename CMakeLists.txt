cmake_minimum_required(VERSION 3.23)
project(enormedb C)

set(CMAKE_C_STANDARD 11)

add_executable(enormedb src/main.c src/bson.h src/document.c src/document.h src/query.h src/collection.h src/db_file.c src/db_file.h src/page.c src/page.h src/pager.c src/pager.h)
