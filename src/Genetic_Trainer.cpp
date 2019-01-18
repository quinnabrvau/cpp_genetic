//
//  Genetic_Trainer.cpp
//  Cpp_Genetic
//
//  Created by Quinn Abrahams-Vaughn on 1/14/19.
//

#include "Genetic_Trainer.hpp"

#ifdef TESTING
#include <iostream>
#include <list>
#include <vector>
#include "unity.h"

using namespace genetic;
using namespace std;

template<class A, class T>
void Genetic_Trainer<A,T>::test__random_init_points(void) {
    std::vector<T> points(agents.size());
    random_init<std::vector<T>,T>(points,0,100);
    TEST_ASSERT_EQUAL(agents.size(),points.size());
    
    //copy points into agents
    auto pit = points.begin();
    auto ait = agents.begin();
    while (ait != agents.end() && pit != points.end()) {
        (*ait++).first = *pit++;
    }
}

template<class A, class T>
void test__GT__sort_agents(void) {
    Genetic_Trainer<A,T> GT;
    GT.test__random_init_points();
    GT.sort_agents();
    T val = GT(0);
    for (int i = 1; i < GT.size(); i++) {
        TEST_ASSERT_GREATER_OR_EQUAL(val, GT(i));
        val = GT(i);
    }
}
template<class A, class T>
void test__GT__breed_agents(void) {
    Genetic_Trainer<A,T> GT;
    GT.reinit(10,10,11);
    GT.test__random_init_points();
    Genetic_Trainer<A,T> GT_copy = GT;
    
    for (int i = 1; i < GT.size();i++) {
        Genetic_Trainer<A,T> GT_copy = GT;
        GT_copy.breed_agents(i);
        for (int j = 0; j < GT.size(); j++) {
            TEST_G_IN_RANGE(A,T,GT[j],10,11);
        }
        GT_copy = GT;
        GT_copy.set_breed_mutation_rates(0.5,0.2,0.1);
        GT_copy.breed_agents(i);
        for (int j = 0; j < GT.size(); j++) {
            TEST_G_IN_RANGE(A,T,GT[j],8,14);
        }
    }
}
template<class A, class T>
void test__GT__return_agents(void) {
    Genetic_Trainer<A,T> GT;
    GT.reinit(10,10,11);
    std::vector<A> agents = GT.return_agents();
    TEST_ASSERT_EQUAL(GT.size(), agents.size());
    
    auto it = agents.begin();
    int index = 0;
    while(it != agents.end() && index < GT.size()) {
        TEST_ASSERT_EQUAL((*it).size(), GT[index].size());
        auto a = (*it).begin();
        auto b = GT[index].begin();
        while(a != (*it).end() && b != GT[index].end()) {
            TEST_ASSERT_EQUAL(*b++, *a++);
        }
        it++; index++;
    }
}

void test__Genetic_Trainer(void) {
    _GEN_RUN_TEST_NUM_TYPE(test__GT__sort_agents);
    _GEN_RUN_TEST_NUM_TYPE(test__GT__breed_agents);
    _GEN_RUN_TEST_NUM_TYPE(test__GT__return_agents);
}

#endif//TESTING
