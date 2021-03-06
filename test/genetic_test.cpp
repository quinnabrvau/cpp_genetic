//
//  genetic_test.cpp
//  GAME_TEST_LIB
//
//  Created by Quinn Abrahams-Vaughn on 1/11/19.
//

#include "Genetic.hpp"
#include "Genetic_Trainer.hpp"

#include "unity.h"

int main(int argc, const char * argv[]) {
    UNITY_BEGIN();
    test__Genetic();
    test__Genetic_Trainer();
    return UNITY_END();
}
