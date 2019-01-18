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
    Genetic_Trainer<A,T> GT(200, //200 agents
                            eval_func<A,T>, // function to evalutate single
                            NULL, // function to evaluate multiple
                            &one, // pointer to context, in this case a pointer to 1 
                            0,    // min start value
                            255   // max start value
                            );
    GT.train_single(    50,// epochs
                        20,// keep the best 20
                        1, // one trial because it is deterministic
                        false//minimize
                        );
}


int main(int argc, const char * argv[]) {
    UNITY_BEGIN();
    _GEN_RUN_TEST_NUM_TYPE(find_ones);
    return UNITY_END();
}
