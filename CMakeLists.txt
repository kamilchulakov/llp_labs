cmake_minimum_required(VERSION 3.23)
project(enormedb C)

set(CMAKE_C_STANDARD 11)

add_executable(enormedb main.c bson.h document.h)
