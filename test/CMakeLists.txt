project( TEST )

cmake_minimum_required (VERSION 3.4)

file(GLOB_RECURSE UNITY_SOURCES "../unity/src/*")

set( GENETIC_SOURCES
    ../src/Genetic.cpp
    ../src/Genetic_Trainer.cpp
    ../include/Genetic.hpp
    ../include/Genetic_Trainer.hpp
)

include_directories (
    ../include
    ../unity/src
)
add_executable (_Genetic_Test 
    genetic_test.cpp
    ${UNITY_SOURCES}
    ${GENETIC_SOURCES}
)
# target_link_libraries(_Genetic_Test
#     GENETIC_TEST_LIB
# )
target_compile_definitions(_Genetic_Test
    PRIVATE TESTING
)

add_test (NAME Genetic_Test COMMAND _Genetic_Test)


add_executable (_Find_Multi 
    find_ones_multi.cpp
    ${UNITY_SOURCES}
    ${GENETIC_SOURCES}
)
# target_link_libraries(_Find_Multi
#     GENETIC_TEST_LIB
# )
target_compile_definitions(_Find_Multi
    PRIVATE TESTING
)
add_test (NAME Multi_Test COMMAND _Find_Multi)

add_executable (_Find_Sing
    find_ones_sing.cpp
    ${UNITY_SOURCES}
    ${GENETIC_SOURCES}
)
# target_link_libraries(_Find_Sing
#     GENETIC_TEST_LIB
# )
target_compile_definitions(_Find_Sing
    PRIVATE TESTING
)
add_test (NAME Sing_Test COMMAND _Find_Sing)
