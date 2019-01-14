//
//  Genetic_Trainer.hpp
//  Cpp_Genetic
//
//  Created by Quinn Abrahams-Vaughn on 1/14/19.
//

#ifndef Genetic_Trainer_hpp
#define Genetic_Trainer_hpp

#include "Genetic.hpp"

namespace genetic {
    template<class A, class T>
    class Genetic_Trainer
    {
        typedef std::pair<A,T> agent;
        typedef T (*evaluation_func_sing) (A*, void*);
        typedef std::vector<T> (*evaluation_func_multi) (std::vector<A*>, void*);
        
        evaluation_func_sing  eval_f_sing;
        evaluation_func_multi eval_f_mult;
        std::vector<agent> agents;
        void* p_context;
    public:
        Genetic_Trainer(int num_agents,
                        evaluation_func_sing  _eval_f_sing = NULL,
                        evaluation_func_multi _eval_f_mult = NULL,
                        void* context = NULL ) :
        agents(num_agents), eval_f_sing(_eval_f_sing), eval_f_mult(_eval_f_mult), p_context(context) {
            
        }
        
        void train_epach_single(int num_trials = 1);
        void train_epach_multi(int num_trials = 1, int num_agents = 2);
        
    };
};

void test__Genetic_Trainer(void);



#endif /* Genetic_Trainer_hpp */
