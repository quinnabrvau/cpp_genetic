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
T eval_func(A* array, void* context) {
    T val = *(T*)context;
    T out = 0;
    for (auto it = array->begin(); it != array->end(); it++) {
        out += ((*it)-val)*((*it)-val);
    }
    return out;
}

template<class A, class T> 
void find_ones(void) {

    T one = 1;
    Genetic_Trainer<A,T> GT(200, // 200 agents
                            20,  // 20 points per agent
                            eval_func<A,T>, // function to evalutate single
                            NULL, // function to evaluate multiple
                            &one, // pointer to context, in this case a pointer to 1 
                            0,    // min start value
                            127);   // max start value
    
    std::vector<A> res = GT.train_single(    
                                50,// epochs
                                20,// keep the best 20
                                1, // one trial because it is deterministic
                                false );//minimize
    cout << "res[0]\tlen " << res[0].size() << "\n";
    for (auto it = res[0].begin(); it != res[0].end(); it++) {
        T foo = *it;
        cout << "\t" << T << "\n";
        TEST_ASSERT_LESS_THAN(2, *it);
    }
}


int main(int argc, const char * argv[]) {
    UNITY_BEGIN();
    _GEN_RUN_TEST_NUM_TYPE(find_ones);
    return UNITY_END();
}
