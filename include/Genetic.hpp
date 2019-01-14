//
//  Genetic.hpp
//  GAME_TEST_LIB
//
//  Created by Quinn Abrahams-Vaughn on 1/10/19.
//

#ifndef Genetic_hpp
#define Genetic_hpp

#include <vector>
#include <utility>

#ifdef TESTING
    #include "unity.h"
    #define ASSERT(...) TEST_ASSERT(__VA_ARGS__)
#else
    #include <cassert>
    #define ASSERT(...) assert(__VA_ARGS__)
#endif

namespace genetic {

template<class A>
void mutate(A& array, float mutation_rate = 0.0, float mutation_scale = 0.0);

template<class A>
    A breed(const A& parent1, const A& parent2, float div = 0.5);

template<class A>
A bread_mutate(const A& parent1, const A& parent2, float div = 0.5, float mutation_rate = 0.0, float mutation_scale = 0.0);

template<class A, class T>
void random_init(A& array, T min, T max);

}; //namespace genetic

void test__Genetic(void);

#endif /* Genetic_hpp */
