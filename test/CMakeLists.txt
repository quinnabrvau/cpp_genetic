project( TEST )

cmake_minimum_required (VERSION 3.4)

file(GLOB_RECURSE UNITY_SOURCES "../unity/src/*")

include_directories (
    ../include
    ../unity/src
)
add_executable (_Genetic_Test 
    genetic_test.cpp
    ${UNITY_SOURCES}
)
target_link_libraries(_Genetic_Test
    GENETIC_TEST_LIB
)
add_test (NAME Genetic_Test COMMAND _Genetic_Test)
