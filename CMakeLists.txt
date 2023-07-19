CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

project(kiri_snake)

add_executable(kiri_snake main.c)

target_link_libraries(kiri_snake termmanip)
