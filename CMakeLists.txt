cmake_minimum_required(VERSION 3.20)

project(cga VERSION 0.1 LANGUAGES C)

# The sources predate C23 (implicit declarations, K&R-era idioms); GCC 15+
# defaults to C23 and rejects them, so pin the standard explicitly.
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

find_package(Threads REQUIRED)

add_executable(cga
    src/main.c
    src/ca.c
    src/cga.c
    src/utils.c
    src/params.c
)

# Same flags the original autotools build used (src/Makefile.am).
target_compile_options(cga PRIVATE -O1 -ggdb -g3)

target_link_libraries(cga PRIVATE m Threads::Threads)

install(TARGETS cga RUNTIME DESTINATION bin)
