cmake_minimum_required(VERSION 3.20)

project(cga VERSION 0.1 LANGUAGES C)

# The sources predate C23 (implicit declarations, K&R-era idioms); GCC 15+
# defaults to C23 and rejects them, so pin the standard explicitly.
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

find_package(Threads REQUIRED)

add_library(cga_core STATIC
    src/ca.c
    src/cga.c
    src/utils.c
    src/params.c
    src/templates.c
)
target_include_directories(cga_core PUBLIC src)
# Same flags the original autotools build used (src/Makefile.am).
target_compile_options(cga_core PUBLIC -O1 -ggdb -g3)
target_link_libraries(cga_core PUBLIC m Threads::Threads)

add_executable(cga src/main.c)
target_link_libraries(cga PRIVATE cga_core)

enable_testing()
add_executable(test_templates tests/test_templates.c)
target_link_libraries(test_templates PRIVATE cga_core)
add_test(NAME templates COMMAND test_templates)

install(TARGETS cga RUNTIME DESTINATION bin)
