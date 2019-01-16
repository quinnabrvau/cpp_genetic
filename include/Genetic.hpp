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

#ifdef TESTING
#include "unity.h"

typedef void (*void_func__g) (void);

typedef unsigned char unchar__g;
typedef unsigned short unshort__g ;
typedef unsigned int unint__g;
typedef unsigned long unlong__g;
typedef long long longlong__g;
typedef long double longdouble__g;

#define __GEN_RUN_TEST(func,type,cont)        \
do {                                          \
void_func__g func ## _ ## cont ## _ ## type      \
= func < cont < type >, type >;                 \
RUN_TEST( (func ## _ ## cont ## _ ## type) ); \
} while(false)

#define _GEN_RUN_TEST_CONTAINER_TYPE(func, type)  \
do {                                              \
__GEN_RUN_TEST( func, type,  list );             \
__GEN_RUN_TEST( func, type,  vector );           \
} while(false)

#define _GEN_RUN_TEST_INUM_TYPE(func)                  \
do {                                                   \
_GEN_RUN_TEST_CONTAINER_TYPE( func, char );            \
_GEN_RUN_TEST_CONTAINER_TYPE( func, unchar__g );        \
_GEN_RUN_TEST_CONTAINER_TYPE( func, int );             \
_GEN_RUN_TEST_CONTAINER_TYPE( func, unint__g );         \
_GEN_RUN_TEST_CONTAINER_TYPE( func, short );           \
_GEN_RUN_TEST_CONTAINER_TYPE( func, unshort__g );       \
_GEN_RUN_TEST_CONTAINER_TYPE( func, long );            \
_GEN_RUN_TEST_CONTAINER_TYPE( func, unlong__g );        \
_GEN_RUN_TEST_CONTAINER_TYPE( func, longlong__g );      \
} while(false)

#define _GEN_RUN_TEST_RNUM_TYPE(func)                   \
do {                                                   \
_GEN_RUN_TEST_CONTAINER_TYPE( func, float );           \
_GEN_RUN_TEST_CONTAINER_TYPE( func, double );          \
_GEN_RUN_TEST_CONTAINER_TYPE( func, longdouble__g );    \
} while(false)

#define _GEN_RUN_TEST_NUM_TYPE(func)   \
do {                                    \
    _GEN_RUN_TEST_INUM_TYPE(func);      \
    _GEN_RUN_TEST_RNUM_TYPE(func);      \
} while (false)

#endif//TESTING

#endif /* Genetic_hpp */
