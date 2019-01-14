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

#ifdef TESTING
    #include "unity.h"
    #define ASSERT(...) TEST_ASSERT(__VA_ARGS__)
#else
    #include <cassert>
    #define ASSERT(...) assert(__VA_ARGS__)
#endif

namespace genetic {

template<class A>
void mutate(A& array, float mutation_rate = 0.0, float mutation_scale = 0.0);

template<class A>
    A breed(const A& parent1, const A& parent2, float div = 0.5);

template<class A>
A bread_mutate(const A& parent1, const A& parent2, float div = 0.5, float mutation_rate = 0.0, float mutation_scale = 0.0);

template<class A, class T>
void random_init(A& array, T min, T max);


template<class A, class T>
class Genetic_Trainer
{
    typedef pair<A,T> agent;
    typedef T (*evaluation_func_sing) (A*, void*);
    typedef std::vector<T> (*evaluation_func_multi) (std::vector<A*>, void*);

    evaluation_func_sing eval_f_sing;
    evaluation_func_multi eval_f_mult;
    std::vector<agent> agents;
    void* p_context;
public:
    Genetic_Trainer(int num_agents, 
            evaluation_func _eval_f_sing = NULL, 
            evaluation_func_multi _eval_f_mult = NULL, 
            void* context = NULL ) :
        agents(num_agents), eval_f_sing(_eval_f_sing), eval_f_mult(_eval_f_mult), p_context(context) {

        }

    void train_epach_single(int num_trials = 1);
    void train_epach_multi(int num_trials = 1);
    
};



}; //namespace genetic

void test__Genetic(void);

#endif /* Genetic_hpp */
