//
//  Genetic_Trainer.cpp
//  Cpp_Genetic
//
//  Created by Quinn Abrahams-Vaughn on 1/14/19.
//

#include "Genetic_Trainer.hpp"
#include <algorithm>

namespace genetic {

template<class A, class T>
void Genetic_Trainer<A,T>::train_epach_single(int num_trials) {
    ASSERT(eval_f_sing != NULL);
    for (int i = 0; i < num_trials; i++) {
        for (auto it = agents.begin(); it != agents.end(); it++) {
            (*it).second += eval_f_sing( &(*it).first, p_context);
        }
    }
}

template<class A, class T>
void Genetic_Trainer<A,T>::train_epach_multi(int num_trials, int num_agents) {
    ASSERT(eval_f_mult != NULL);
    ASSERT(num_agents >= agents.size());
    //TODO add warning if training amount will be unequal
    
    int i, j;
    for ( i = 0; i < num_trials; i++) {
        std::random_shuffle(agents.begin(), agents.end());
        std::vector<A*> v(num_agents);
        auto it = agents.begin();
        
        while (it != agents.end()) {
            j = 0;
            while (j<num_agents && it != agents.end()) {
                v[j++] = &(*it++).first;
            }
            //evaluate group
            std::vector<T> res = eval_f_multi( v, p_context );
            
            // create iterators to store data
            auto rit = res.end(); // start at end
            auto cit = it;
            
            // iterate backwards to the start
            do {
                rit--; cit--;
                (*cit).second += *rit;
            } while (rit != res.begin() && cit-- != agents.begin());
        }
    }
}

};// namespace genetic

#ifdef  TESTING
#include <iostream>
#include <list>
#include <vector>


void test__Genetic_Trainer(void) {

}

#endif//TESTING
