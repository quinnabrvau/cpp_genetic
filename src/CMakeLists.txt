project( GENETIC_LIB )
cmake_minimum_required (VERSION 3.4)

file(GLOB_RECURSE headers "../include/*.hpp")

set( GENETIC_SOURCES
    ./Genetic.cpp
    ./Genetic_Trainer.cpp
    ../include/Genetic.hpp
    ../include/Genetic_Trainer.hpp
)


include_directories(
    ../include
)
add_library(GENETIC_TEST_LIB
    ${GENETIC_SOURCES}
    ../unity/src/unity.c
)
target_include_directories(GENETIC_TEST_LIB
    PRIVATE ../unity/src
)
target_compile_definitions(GENETIC_TEST_LIB
    PRIVATE TESTING
)

# add_library(GENETIC_LIB
#     ${GENETIC_SOURCES}
# )
# target_compile_options(GENETIC_LIB
#     PRIVATE -O3
# )
