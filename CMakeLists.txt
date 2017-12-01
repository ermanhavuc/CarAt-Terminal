cmake_minimum_required(VERSION 3.8)
project(Project2)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c bookmark.c print.c)
#add_executable(Project2 ${SOURCE_FILES})
add_executable(Project2 print.c)