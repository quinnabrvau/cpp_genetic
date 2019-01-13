//
//  Genetic.hpp
//  GAME_TEST_LIB
//
//  Created by Quinn Abrahams-Vaughn on 1/10/19.
//

#ifndef Genetic_hpp
#define Genetic_hpp

#ifdef TESTING
    #include "unity.h"
    #define ASSERT(...) TEST_ASSERT(__VA_ARGS__)
else
    #ifdef DEBUG
        #include <cassert>
        #define ASSERT(...) assert(__VA_ARGS__)
    #else
        #define ASSERT(...) #__VA_ARGS__
    #endif
#endif

namespace genetic {

template<class A>
void mutate(A& array, float mutation_rate = 0.0, float mutation_scale = 0.0);

template<class A>
A breed(A& parent1, A& parent2 );

template<class A>
A bread_mutate(A& parent1, A& parent2, float mutation_rate = 0.0, float mutation_scale = 0.0);

template<class A, class T>
void random_init(A& array, T min, T max);

}; //namespace genetic

void test__Genetic(void);

#endif /* Genetic_hpp */
