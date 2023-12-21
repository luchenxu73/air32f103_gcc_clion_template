cmake_minimum_required(VERSION 3.22)

include_directories("I2C/inc")
include_directories("delay")

file(GLOB_RECURSE USR_SRC "./*.c")

add_library(USER_LIB ${USR_SRC})