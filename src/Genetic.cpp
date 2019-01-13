//
//  Genetic.cpp
//  GAME_TEST_LIB
//
//  Created by Quinn Abrahams-Vaughn on 1/10/19.
//

#include "Genetic.hpp"
#include <random>
#include <type_traits>
#include <algorithm>

namespace genetic {

template<class A>
void mutate(A& array, float mutation_rate, float mutation_scale) {
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
A breed(const A& parent1, const A& parent2, float div ) {
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
A bread_mutate(const A& parent1, const A& parent2, float div, float mutation_rate, float mutation_scale) {
    A out = breed(parent1, parent2);
    out.mutate(mutation_rate, mutation_scale);
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

template<class A, class T>
void random_init(A& array, T min, T max) {
    ASSERT(max>=min); // check the max > min    
    bool done = _random_init(array, min, max);
}



}; //namespace genetic

#ifdef  TESTING
#include <iostream>
#include <list>
#include <vector>


using namespace genetic;
using namespace std;


template<class A, class T>
bool test__Genetic__equal(const A& array, T val) {
    for (auto it = array.cbegin(); it != array.cend(); it++) {
        if ((*it) != val) {
            return false;
        }
    }
    return true;
}

template<class A, class T>
bool test__Genetic__in_range(const A& array, T min, T max) {
    for (auto it = array.cbegin(); it != array.cend(); it++) {
        if ((*it) > max || (*it) < min) {
            return false;
        }
    }
    return true;
}

template<class A,class T>
void test__Genetic__random(void) {
    for (int i = 0; i < 40; i++) {
        A array(i);
        random_init<A,T>(array,0,10);
        TEST_ASSERT(test__Genetic__in_range(array, 0, 10));

        random_init<A,T>(array,10,20);
        TEST_ASSERT(test__Genetic__in_range(array, 10, 20));

        random_init<A,T>(array,0,0); //will intialize to all zeros (not efficient)
        TEST_ASSERT(test__Genetic__in_range(array, 0, 0));
        TEST_ASSERT(test__Genetic__equal(array,0));

        random_init<A,T>(array,1,1); //will intialize to all ones (not efficient)
        TEST_ASSERT(test__Genetic__in_range(array, 1, 1));
        TEST_ASSERT(test__Genetic__equal(array,1));
    }
}

template<class A>
bool _test__Genetic__breed(const A& a, const A& b, const A& c) {
    //initialize iterators
    auto ait = a.cbegin();
    auto bit = b.cbegin();
    auto cit = c.cbegin();

    while (ait != a.cend() && bit != b.cend() && cit != c.cend()) {
        if (*cit != *ait && *cit != *bit) 
            return false;
        ait++; bit++; cit++;
    } 
    return true;
}

template<class A, class T>
void test__Genetic__breed(void) {
    for (int i = 1; i < 40; i++) {
        A a(i), b(i);
        random_init<A,T>(a,0,10);
        random_init<A,T>(b,0,10);
        A c = breed<A>(a,b);
        TEST_ASSERT(c.size()==a.size());
        TEST_ASSERT(_test__Genetic__breed<A>(a,b,c));
        A d = breed<A>(c, a, 0.2);
        TEST_ASSERT(d.size()==a.size());
        TEST_ASSERT(_test__Genetic__breed<A>(a,b,d));
        
        A e = breed<A>(c, d, 0.8);
        TEST_ASSERT(d.size()==a.size());
        TEST_ASSERT(_test__Genetic__breed<A>(a,b,e));
    }
}
template<class A, class T>
void test__Genetic__mutate(void) {
    for (int i = 1; i < 40; i++) {
        A G(i); random_init(G,1,1); //will intialize to all ones
        TEST_ASSERT(test__Genetic__in_range(G, 1, 1));
        TEST_ASSERT(test__Genetic__equal(G,1));
        mutate<A>(G,1,1);
        TEST_ASSERT(test__Genetic__in_range(G, 0, 2));
        TEST_ASSERT(!test__Genetic__equal(G,1));
    }
}


typedef void (*void_func) (void);

typedef unsigned char unchar__;
typedef unsigned short unshort__ ;
typedef unsigned int unint__;
typedef unsigned long unlong__;
typedef long long longlong__;
typedef long double longdouble__;

#define __GEN_RUN_TEST(func,type,cont)        \
do {                                          \
void_func func ## _ ## cont ## _ ## type      \
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
_GEN_RUN_TEST_CONTAINER_TYPE( func, unchar__ );        \
_GEN_RUN_TEST_CONTAINER_TYPE( func, int );             \
_GEN_RUN_TEST_CONTAINER_TYPE( func, unint__ );         \
_GEN_RUN_TEST_CONTAINER_TYPE( func, short );           \
_GEN_RUN_TEST_CONTAINER_TYPE( func, unshort__ );       \
_GEN_RUN_TEST_CONTAINER_TYPE( func, long );            \
_GEN_RUN_TEST_CONTAINER_TYPE( func, unlong__ );        \
_GEN_RUN_TEST_CONTAINER_TYPE( func, longlong__ );      \
} while(false)

#define _GEN_RUN_TEST_RNUM_TYPE(func)                   \
do {                                                   \
_GEN_RUN_TEST_CONTAINER_TYPE( func, float );           \
_GEN_RUN_TEST_CONTAINER_TYPE( func, double );          \
_GEN_RUN_TEST_CONTAINER_TYPE( func, longdouble__ );    \
} while(false)

#define _GEN_RUN_TEST_NUM_TYPE(func)   \
do {                                    \
    _GEN_RUN_TEST_INUM_TYPE(func);      \
    _GEN_RUN_TEST_RNUM_TYPE(func);      \
} while (false)

void test__Genetic(void) {
    
    _GEN_RUN_TEST_NUM_TYPE(test__Genetic__random);
    _GEN_RUN_TEST_NUM_TYPE(test__Genetic__breed);
    _GEN_RUN_TEST_NUM_TYPE(test__Genetic__mutate);
}


#endif//TESTING


