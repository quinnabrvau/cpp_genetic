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
    int size = s.size();

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

    while (lit != locs.end() && cit != changes.end() && ait != array.end()) {
        if (index == *lit) {
            (*ait) *= (*cit);
            cit++; lit++;
        }
        ait++; index++;
    }
}

template<class A>
A breed(A& parent1, A& parent2 ) {
    ASSERT(parent1.size() == parent2.size());
    std::default_random_engine generator;
    std::uniform_int_distribution<int> dist(0, 1);
    A out(parent1.begin(),parent1.end());

    auto pit = parent2.begin(); 
    auto oit = out.begin(); 

    while (pit != parent2.end() && oit != out.end()) {
        if (dist(generator)) {
            (*oit) = (*pit);
        }
    }
    return out;
}

template<class A>
A bread_mutate(A& parent1, A& parent2, float mutation_rate, float mutation_scale) {
    A out = breed(parent1, parent2);
    out.mutate(mutation_rate, mutation_scale);
    return out;
}
template<class A, class T>
void r_random(A& array, T min, T max) {
    std::default_random_engine generator;
    std::uniform_real_distribution<T> dist(min,max);
    for (auto it = array.begin(); it != array.end(); it++) {
        (*cit) = dist(generator);
    }
}

template<class A, class T>
void i_random(A& array, T min, T max) {
    std::default_random_engine generator;
    std::uniform_int_distribution<T> dist(min,max);
    for (auto it = array.begin(); it != array.end(); it++) {
        (*cit) = dist(generator);
    }
}

template<class A, class T>
void random_init(A& array, T min, T max) {
    ASSERT(max>=min);
    if (std::is_integral<T>::value) {
        i_random<A,T>(min, max);
    } else if (std::is_floating_point<T>::value) {
        r_random<A,T>(min, max);
    } else {
        ASSERT(false);
    }
}

}; //namespace genetic

#ifdef  TESTING
#include <iostream>

using namespace genetic {


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
        A d = breed<A>(c, a);
        TEST_ASSERT(D.shape()==A.shape());
        TEST_ASSERT(_test__Genetic__breed<A>(a,c,d));
    }
}
template<class T>
void test__Genetic__mutate(void) {
    for (int i = 1; i < 40; i++) {
        A a(i); random_init(a,1,1); //will intialize to all ones
        TEST_ASSERT(test__Genetic__in_range(G, 1, 1));
        TEST_ASSERT(test__Genetic__equal(G,1));
        mutate<A>(G,1,1);
        TEST_ASSERT(test__Genetic__in_range(G, 0, 2));
        TEST_ASSERT(!test__Genetic__equal(G,1));
        }
    }
}


typedef void (*void_func) (void);

typedef unsigned int unchar__;
typedef unsigned unshort__ ;
typedef unsigned int unint__;
typedef unsigned long unlong__;
typedef long long longlong__;
typedef long double longdouble__;

#define __GEN_RUN_TEST(func,type,cont)        \
do {                                          \
void_func func ## _ ## cont ## _ ## type      \
= func < std::cont< type > >;                 \
RUN_TEST( (func ## _ ## cont ## _ ## type) ); \
} while(false)

#define _GEN_RUN_TEST_CONTAINER_TYPE(func, type)  \
do {                                              \
___RUN_TEST( func,type,  list );             \
___RUN_TEST( func,type,  vector );           \
___RUN_TEST( func,type,  array );            \
} while(false)

#define _GEN_RUN_TEST_NUM_TYPE(func)                   \
do {                                                   \
_GEN_RUN_TEST_CONTAINER_TYPE( func, char );            \
_GEN_RUN_TEST_CONTAINER_TYPE( func, unchar__ );        \
_GEN_RUN_TEST_CONTAINER_TYPE( func, int );             \
_GEN_RUN_TEST_CONTAINER_TYPE( func, short );           \
_GEN_RUN_TEST_CONTAINER_TYPE( func, unshort__ );       \
_GEN_RUN_TEST_CONTAINER_TYPE( func, long );            \
_GEN_RUN_TEST_CONTAINER_TYPE( func, longlong__ );      \
_GEN_RUN_TEST_CONTAINER_TYPE( func, unint__ );         \
_GEN_RUN_TEST_CONTAINER_TYPE( func, unshort__ );       \
_GEN_RUN_TEST_CONTAINER_TYPE( func, unlong__ );        \
_GEN_RUN_TEST_CONTAINER_TYPE( func, char );            \
_GEN_RUN_TEST_CONTAINER_TYPE( func, float );           \
_GEN_RUN_TEST_CONTAINER_TYPE( func, double );          \
_GEN_RUN_TEST_CONTAINER_TYPE( func, longdouble__ );    \
} while(false)


void test__Genetic(void) {
    /*
    _NUM_T_RUN_TEST(test__Genetic__random);
    _NUM_T_RUN_TEST(test__Genetic__breed);
    _NUM_T_RUN_TEST(test__Genetic__mutate);
    */
}

};//using namespace genetic

#endif//TESTING


