cmake_minimum_required(VERSION 3.24)

include_directories("Delay/inc")

file(GLOB_RECURSE USR_SRC "./*.c")

add_library(USER_LIB ${USR_SRC})