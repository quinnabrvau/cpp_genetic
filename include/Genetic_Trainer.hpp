//
//  Genetic_Trainer.hpp
//  Cpp_Genetic
//
//  Created by Quinn Abrahams-Vaughn on 1/14/19.
//

#ifndef Genetic_Trainer_hpp
#define Genetic_Trainer_hpp

#include "Genetic.hpp"
#include <vector>

namespace genetic {
    template<class A, class T>
    class Genetic_Trainer
    {
        typedef std::pair<T,A> agent;
        typedef T (*evaluation_func_sing) (A*, void*);
        typedef std::vector<T> (*evaluation_func_multi) (std::vector<A*>, void*);
        
        evaluation_func_sing  eval_f_sing;
        evaluation_func_multi eval_f_mult;
        std::vector<agent> agents;
        void* p_context;
        
        //mutaion rates
        float div = 0.5;
        float mutation_rate = 0.0;
        float mutation_scale = 0.0;
    public:
        Genetic_Trainer(int num_agents = 10,
                        evaluation_func_sing  _eval_f_sing = NULL,
                        evaluation_func_multi _eval_f_mult = NULL,
                        void* context = NULL, T min = 0, T max = 1 ) :
        eval_f_sing(_eval_f_sing), eval_f_mult(_eval_f_mult), agents(num_agents), p_context(context) {
            for (auto it = agents.begin(); it != agents.end(); it++) {
                random_init((*it).second, min, max);
            }
        }
        void reinit(int num_agents = 10, T min = 0, T max = 1) {
            agents = std::vector<agent>(num_agents);
            for (auto it = agents.begin(); it != agents.end(); it++) {
                random_init((*it).second, min, max);
            }
        }
        
        void set_breed_mutation_rates(float _div = -1, float _mutation_rate = -1, float _mutation_scale = -1) {
            if (_div >= 0 && _div <= 1) {
                div = _div;
            }
            if (_mutation_rate >= 0 && _mutation_rate <= 1) {
                mutation_rate = _mutation_rate;
            }
            if (_mutation_scale >= 0 && _mutation_scale <= 1) {
                mutation_scale = _mutation_scale;
            }
        }
        
        void train_epach_single(int num_trials = 1);
        void train_epach_multi(int num_trials = 1, int num_agents = 2);
        
        void sort_agents(void);
        void breed_agents(int keep = 2 );
        std::vector<A> return_agents(void);
        
        std::vector<A> train_single(int num_epochs = 1,
                                    int keep = 2,
                                    int num_trials = 1);
        
        std::vector<A> train_multi(int num_epochs = 1,
                                   int keep = 2,
                                   int num_trials = 1,
                                   int num_agents_per_train = 2);
        
        #ifdef TESTING
            void test__sort_agents(void);
        #endif//TESTING
    };
};

void test__Genetic_Trainer(void);



#endif /* Genetic_Trainer_hpp */
