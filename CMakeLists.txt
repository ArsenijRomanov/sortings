cmake_minimum_required(VERSION 3.7)
project(SORT_STRINGS)

set(CMAKE_C_STANDARD 99)

include_directories(include)

option(memory_mapping OFF)

if (memory_mapping)
	set(read_file_way src/read_file_mmap.c)
else()
	set(read_file_way src/read_file_API.c)
endif()

add_executable(sort_strings src/main.c src/sortings.c ${read_file_way})

target_compile_options(sort_strings PRIVATE -Wall -Wextra -pedantic -Werror)

