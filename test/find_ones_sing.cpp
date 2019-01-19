//
//  find_identity_sing.cpp
//  GENETIC_LIB
//
//  Created by Quinn Abrahams-Vaughn on 1/18/19.
//

#include "Genetic.hpp"
#include "Genetic_Trainer.hpp"

#include "unity.h"
#include <vector>
#include <list>

#include <iostream>

using namespace genetic;
using namespace std;

template<class A, class T> 
T eval_func_logistic(A* array, void* context) {
    T val = *(T*)context;
    T out = 0;
    for (auto it = array->begin(); it != array->end(); it++) {
        out += (val != *it);
    }
    return out;
}

template<class A, class T>
T eval_func_mse(A* array, void* context) {
    T val = *(T*)context;
    T out = 0;
    for (auto it = array->begin(); it != array->end(); it++) {
        out += (val-*it)*(val-*it);
    }
    return out;
}

template<class A, class T> 
void find_ones_log(void) {

    T one = 4;
    Genetic_Trainer<A,T> GT(200, // 200 agents
                            3,  // 3 points per agent
                            eval_func_logistic<A,T>, // function to evalutate single
                            NULL, // function to evaluate multiple
                            &one, // pointer to context, in this case a pointer to 1 
                            3,    // min start value
                            8);   // max start value
    GT.set_breed_mutation_rates(0.5,1,1,0.5);
    std::vector<A> res = GT.train_single(    
                                10,// epochs
                                20,// keep the best 20
                                1, // one trial because it is deterministic
                                false );//minimize
    for (auto it = res[0].begin(); it != res[0].end(); it++) {
        TEST_ASSERT_LESS_THAN(one+3, *it);
        TEST_ASSERT_GREATER_THAN(one-3, *it);
    }
}

template<class A, class T>
void find_ones_mse(void) {
    T one = 4;
    Genetic_Trainer<A,T> GT(200, // 200 agents
                            3,  // 3 points per agent
                            eval_func_mse<A,T>, // function to evalutate single
                            NULL, // function to evaluate multiple
                            &one, // pointer to context, in this case a pointer to 1
                            3,    // min start value
                            8);   // max start value
    GT.set_breed_mutation_rates(0.5,1,1,0.5);
    std::vector<A> res = GT.train_single(
                                         10,// epochs
                                         20,// keep the best 20
                                         1, // one trial because it is deterministic
                                         false );//minimize
    for (auto it = res[0].begin(); it != res[0].end(); it++) {
        TEST_ASSERT_LESS_THAN(one+3, *it);
        TEST_ASSERT_GREATER_THAN(one-3, *it);
    }
}



int main(int argc, const char * argv[]) {
    UNITY_BEGIN();
    _GEN_RUN_TEST_CNUM_TYPE(find_ones_log);
    _GEN_RUN_TEST_INUM_TYPE(find_ones_log);
    _GEN_RUN_TEST_INUM_TYPE(find_ones_mse);
    _GEN_RUN_TEST_RNUM_TYPE(find_ones_mse);
    return UNITY_END();
}
