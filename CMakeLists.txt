cmake_minimum_required(VERSION 3.23)
project(mongodb C)

set(CMAKE_C_STANDARD 11)

add_executable(mongodb main.c bson.h document.h)
