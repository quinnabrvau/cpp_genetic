//
//  Genetic.cpp
//  GAME_TEST_LIB
//
//  Created by Quinn Abrahams-Vaughn on 1/10/19.
//

#include "Genetic.hpp"


#ifdef TESTING
#include <iostream>
#include <list>
#include <vector>
#include "unity.h"

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
        TEST_G_IN_RANGE(A,T,array, 0, 10);

        random_init<A,T>(array,10,20);
        TEST_G_IN_RANGE(A,T,array, 10, 20);

        random_init<A,T>(array,0,0); //will intialize to all zeros (not efficient)
        TEST_G_IN_RANGE(A,T,array, 0, 0);
        TEST_G_EQUAL(A,T,array,0);

        random_init<A,T>(array,1,1); //will intialize to all ones (not efficient)
        TEST_G_IN_RANGE(A,T,array, 1, 1);
        TEST_G_EQUAL(A,T,array,1);
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
        TEST_G_IN_RANGE(A,T,G, 1, 1);
        TEST_G_EQUAL(A,T,G,1);
        mutate<A>(G,0,1);
        TEST_G_EQUAL(A,T,G,1);
        mutate<A>(G,1,0);
        TEST_G_EQUAL(A,T,G,1);
        mutate<A>(G,1,1);
        TEST_G_IN_RANGE(A,T,G, 0, 2);
        TEST_ASSERT(!test__Genetic__equal(G,1));
    }
}

template<class A, class T>
void test__Genetic__bread_mutate(void)  {
    for (int i = 1; i < 40; i++) {
        A a(i), b(i);
        random_init<A,T>(a,10,10);
        random_init<A,T>(b,11,11);
        A c = bread_mutate<A>(a,b,0.5,0.2,0.1);
        TEST_G_IN_RANGE(A,T,c, 8, 14);
    }
}

void test__Genetic(void) {
    _GEN_RUN_TEST_NUM_TYPE(test__Genetic__random);
    _GEN_RUN_TEST_NUM_TYPE(test__Genetic__breed);
    _GEN_RUN_TEST_NUM_TYPE(test__Genetic__mutate);
    _GEN_RUN_TEST_NUM_TYPE(test__Genetic__bread_mutate);
}

#endif//TESTING

