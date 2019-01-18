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

#include <random>
#include <type_traits>
#include <algorithm>

#ifdef TESTING
    #include "unity.h"
    #define ASSERT(...) TEST_ASSERT(__VA_ARGS__)
#else
    #include <cassert>
    #define ASSERT(...) assert(__VA_ARGS__)
#endif

namespace genetic {

template<class A, class T>
    void random_init(A& array, T min, T max);

template<class A>
    void mutate(
            A& array, 
            float mutation_rate = 0.0, 
            float mutation_scale = 0.0) {
        int size = array.size();

        // initialize the locations to be changed
        std::vector<int> locs(size * mutation_rate);
        random_init<std::vector<int>, int>(locs, 0, size );
        // sort locations to be changed
        std::sort(locs.begin(), locs.end());

        // initiaize the vector of changes to apply
        std::vector<float> changes(locs.size());
        random_init<std::vector<float>, float>(changes, 1-mutation_scale, 1+mutation_scale );

        // create iterators
        auto lit = locs.begin();    
        auto cit = changes.begin(); 
        auto ait = array.begin();   
        int index = 0;

        // iterate through arrays
        while (lit != locs.end() && cit != changes.end() && ait != array.end()) {
            if (index == *lit) { // if we are at the next change location
                (*ait) *= (*cit); // change by scale
                cit++; lit++; // increment change iterators
            }
            ait++; index++; //increment array iterators
        }
    }

template<class A>
    A breed(
            const A& parent1, 
            const A& parent2, 
            float div = 0.5) {
        ASSERT(div>=0 && div<=1); //check that ratio is in [0,1]
        if (div < 0.5) { // switch order to minimize copy operations
            return breed (parent1, parent2, 1-div);
        }
        
        // check the two arrays are the same size
        ASSERT(parent1.size() == parent2.size());
        // create the random number generator
        std::default_random_engine generator;
        std::uniform_real_distribution<float> dist(0, 1);
        
        // initialize the output as parent 1;
        A out(parent1.cbegin(),parent1.cend());

        // initialize iterators
        auto pit = parent2.cbegin(); //const
        auto oit = out.begin();
        
        // iterate through array
        while (pit != parent2.cend() && oit != out.end()) {
            if (dist(generator) > div) { // check if this data should be parent 2;
                (*oit) = (*pit);
            }
            pit++; oit++; //increment array iterators
        }
        return out; //return child
    }

template<class A>
    A bread_mutate(
            const A& parent1, 
            const A& parent2, 
            float div = 0.5, 
            float mutation_rate = 0.0, 
            float mutation_scale = 0.0) {
        A out = breed<A>(parent1, parent2, div);
        mutate<A>(out, mutation_rate, mutation_scale);
        return out;
    }
template< class A, typename T >
typename std::enable_if<std::is_floating_point<T>::value, bool>::type
    _random_init(A& array, T min, T max) {
        std::default_random_engine generator;
        std::uniform_real_distribution<T> dist(min, max);
        // iterate through array
        for (auto it = array.begin(); it != array.end(); it++) {
            (*it) = dist(generator); // set value to random number
        }
        return true;
    }

template< class A, typename T >
typename std::enable_if<std::is_integral<T>::value, bool>::type
    _random_init(A& array, T min, T max) {
        std::default_random_engine generator;
        std::uniform_int_distribution<T> dist(min, max);
        // iterate through array
        for (auto it = array.begin(); it != array.end(); it++) {
            (*it) = dist(generator); // set value to random number
        }
        return true;
    }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
template<class A, class T>
    void random_init(A& array, T min, T max) {
        ASSERT(max>=min);  // check the max > min
        bool done = _random_init(array, min, max);
    }
#pragma GCC diagnostic pop

}; //namespace genetic

void test__Genetic(void);

#ifdef TESTING

template<class A, class T>
bool test__Genetic__equal(const A& array, T val);
template<class A, class T>
bool test__Genetic__in_range(const A& array, T min, T max);

#define TEST_G_IN_RANGE(_A,_T,A,min,max)                    \
do {                                                        \
bool foo = test__Genetic__in_range<_A,_T>(A,min,max);       \
TEST_ASSERT(foo);                                           \
} while(0);

#define TEST_G_EQUAL(_A,_T,A,val)                           \
do {                                                        \
bool foo = test__Genetic__equal<_A,_T>(A,val);              \
TEST_ASSERT(foo);                                           \
} while(0);

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
