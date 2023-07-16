cmake_minimum_required(VERSION 3.7)

project(svalbard)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(MSVC)
	add_compile_options(/W4)
else()
	add_compile_options(-Wall -Wextra -pedantic)
	add_compile_options (-fdiagnostics-color=always)
endif()

add_executable(svalbard)
target_sources(svalbard PRIVATE
	src/main.cpp
	src/dan_string.cpp
	src/parser.cpp
)

# Uncomment bellow to build all source files in one translation unit.
# set_target_properties(arctic PROPERTIES UNITY_BUILD TRUE UNITY_BUILD_BATCH_SIZE 0)
