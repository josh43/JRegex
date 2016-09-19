cmake_minimum_required(VERSION 3.6)
project(JRegex)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Regex/Regex.h Regex/Regex.cpp
       RegexIncludeAll.h Utility.cpp Utility.h
       Parser/RegularErrorMessage.h )
add_executable(JRegex ${SOURCE_FILES})