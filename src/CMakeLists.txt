project( GAME )
cmake_minimum_required (VERSION 3.4)

file(GLOB_RECURSE headers "../include/*.hpp")

set( GAME_SOURCES
    ./Board.cpp
    ./Game.cpp
    ./Player.cpp
    ./b_common.cpp
    ../include/Board.hpp
    ../include/Game.hpp
    ../include/Player.hpp
    ../include/b_common.hpp
    ../include/common.hpp
)
set( GENETIC_SOURCES
    ./Array.cpp
    ./Genetic.cpp
    ../include/Genetic.hpp
    ../include/Array.hpp
    ../include/common.hpp
)
set( ADV_GAME_SOURCES
    ./A_Game.cpp
    ./A_Player.cpp
    ../include/A_Game.hpp
    ../include/A_Player.hpp
)

include_directories(
    ../include
)

add_library(GAME_TEST_LIB
    ../unity/src/unity.c
    ${GAME_SOURCES}
)
target_include_directories(GAME_TEST_LIB
    PRIVATE ../unity/src
)
target_compile_definitions(GAME_TEST_LIB
    PRIVATE TESTING
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

add_library(ADV_GAME_TEST_LIB
../unity/src/unity.c
${ADV_GAME_SOURCES}
)
target_include_directories(ADV_GAME_TEST_LIB
PRIVATE ../unity/src
)
target_compile_definitions(ADV_GAME_TEST_LIB
PRIVATE TESTING
)

add_executable(catan
    ${GAME_SOURCES}
    ${GENETIC_SOURCES}
    ${ADV_GAME_SOURCES}
    ./main.cpp
)
