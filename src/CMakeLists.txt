project( GENETIC_LIB )
cmake_minimum_required (VERSION 3.4)

file(GLOB_RECURSE headers "../include/*.hpp")

set( GENETIC_SOURCES
    ./Genetic.cpp
    ../include/Genetic.hpp
)

include_directories(
    ../include
)

add_library(GENETIC_TEST_LIB
    ../unity/src/unity.c
    ${GENETIC_SOURCES}
)
target_include_directories(GENETIC_TEST_LIB
    PRIVATE ../unity/src
)
target_compile_definitions(GENETIC_TEST_LIB
    PRIVATE TESTING
)

add_library(GENETIC_LIB
    ${GENETIC_SOURCES}
)
