//
//  Genetic_Trainer.cpp
//  Cpp_Genetic
//
//  Created by Quinn Abrahams-Vaughn on 1/14/19.
//

#include "Genetic_Trainer.hpp"
#include <algorithm>
#include <random>

namespace genetic {

    template<class A, class T>
    void Genetic_Trainer<A,T>::train_epach_single(int num_trials) {
        ASSERT(eval_f_sing != NULL);
        for (int i = 0; i < num_trials; i++) {
            for (auto it = agents.begin(); it != agents.end(); it++) {
                (*it).first += eval_f_sing( &(*it).second, p_context);
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
                    v[j++] = &(*it++).second;
                }
                //evaluate group
                std::vector<T> res = eval_f_multi( v, p_context );
                
                // create iterators to store data
                auto rit = res.end(); // start at end
                auto cit = it;
                
                // iterate backwards and copy the results into the agents score
                do {
                    rit--; cit--;
                    (*cit).first += *rit;
                } while (rit != res.begin() && cit-- != agents.begin());
            }
        }
    }


    template<class A, class T>
    void Genetic_Trainer<A,T>::sort_agents(bool max) {
        std::sort(agents.begin(), agents.end());
        if (!max) {
            std::reverse(agents.begin(), agents.end());
        }
    }
    template<class A, class T>
    void Genetic_Trainer<A,T>::breed_agents(int keep, bool max) {
        sort_agents(max); //sort agents so we keep the current thought of best
        
        auto it = agents.begin();
        int index1, index2, i;
        std::default_random_engine generator;
        std::uniform_int_distribution<int> dist(0, keep);
        
        // move to start of agents to replace
        for ( i = 0; i < keep; i++) {
            (*it).first = 0; // clear score
            it++;
        }
        // replace remaining agents with agents bred from the keep set
        for (; it != agents.end(); it++) {
            (*it).first = 0; // clear score
            index1 = dist(generator);index2 = dist(generator); //choose parents to breed
            (*it).second = bread_mutate<A>(
                    agents[index1].second, //parent 1
                    agents[index2].second, //parent 2
                    div,                   //ratio of parent to use,
                    mutation_rate,         //percent of "genes" to mutate
                    mutation_scale);       //range to mutate "genes"
            
        }
    }

    template<class A, class T>
    std::vector<A> Genetic_Trainer<A,T>::return_agents(void) {
        std::vector<A> out(agents.size()); // initialize output array to the correct size
        //init iterators
        auto oit = out.begin();
        auto ait = agents.begin();

        //copy agents
        while (ait != agents.end() && oit != out.end()) {
            *oit++ = (*ait++).second; // copy ait array into output array
        }
        // check that the arrays are the same size
        ASSERT(ait == agents.end());
        ASSERT(oit == out.end());

        return out;
    }

    template<class A, class T>
    std::vector<A> Genetic_Trainer<A,T>::train_single(
                      int num_epochs,
                      int keep,
                      int num_trials,
                      bool max ) {
        ASSERT( (eval_f_sing != NULL) );
        // for epochs-1 
        // evaluate the agents
        // keep the best x and breed them
        for (int epoch = 0; epoch < num_epochs-1; epoch++) {
            train_epach_single(num_trials);
            breed_agents(keep, max);
        }
        // for the last epoch
        // evaluate the agents
        // sort the agents
        train_epach_single(num_trials);
        sort_agents();
        // return the vector of agents sorted by current evaluation
        return return_agents();
        
    }
    
    template<class A, class T>
    std::vector<A> Genetic_Trainer<A,T>::train_multi(
                      int num_epochs,
                      int keep,
                      int num_trials,
                      int num_agents_per_train,
                      bool max) {
        ASSERT( (eval_f_mult != NULL) );
        // for epochs-1
        // evaluate the agents
        // keep the best x and breed them
        for (int epoch = 0; epoch < num_epochs-1; epoch++) {
            train_epach_multi(num_trials, num_agents_per_train);
            breed_agents(keep, max);
        }
        // for the last epoch
        // evaluate the agents
        // sort the agents
        train_epach_multi(num_trials, num_agents_per_train);
        sort_agents();
        // return the vector of agents sorted by current evaluation
        return return_agents();
    }

};// namespace genetic


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
