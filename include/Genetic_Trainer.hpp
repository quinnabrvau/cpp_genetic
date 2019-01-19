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
#include <algorithm>
#include <random>

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
        float swap_rate = 0;
    public:
        //init with known agents
        Genetic_Trainer(std::vector<A> _agents, int num_agents=-1,
                        evaluation_func_sing  _eval_f_sing = NULL,
                        evaluation_func_multi _eval_f_mult = NULL,
                        void* context = NULL, T min = 0, T max = 1 ):
        eval_f_sing(_eval_f_sing), eval_f_mult(_eval_f_mult), p_context(context) {
            
            if (agents.size() < num_agents) {
                agents = std::vector<A> (num_agents);
                for (int i = 0; i < num_agents; i++) {
                    agents[i].first = 0;
                    if (i < agents.size()) {
                        agents[i].second = _agents[i];
                    } else {
                        random_init(agents[i], min, max);
                    }
                }
            } else {
                agents = std::vector<A>(_agents.size());
                for (int i = 0; i < agents.size(); i++) {
                    agents[i].first = 0;
                    agents[i].second = _agents[i];
                }
            }
        }
        
        Genetic_Trainer(int num_agents = 10, int agents_size = 10,
                        evaluation_func_sing  _eval_f_sing = NULL,
                        evaluation_func_multi _eval_f_mult = NULL,
                        void* context = NULL, T min = 0, T max = 1 ) :
        eval_f_sing(_eval_f_sing), eval_f_mult(_eval_f_mult), agents(num_agents), p_context(context) {
            for (auto it = agents.begin(); it != agents.end(); it++) {
                (*it).second = A(agents_size);
                random_init((*it).second, min, max);
            }
        }


        // basic index access functions
        int size() const {return agents.size();}
        A& operator[](int index) {return agents[index].second;}
        T& operator()(int index) {return agents[index].first;}
        

        void reinit(int num_agents = 10, T min = 0, T max = 1) {
            agents = std::vector<agent>(num_agents);
            for (auto it = agents.begin(); it != agents.end(); it++) {
                random_init((*it).second, min, max);
            }
        }
        
        void set_breed_mutation_rates(float _div = -1, float _mutation_rate = -1, float _mutation_scale = -1, float _swap_rate = -1) {
            if (_div >= 0 && _div <= 1) {
                div = _div;
            }
            if (_mutation_rate >= 0) {
                mutation_rate = _mutation_rate;
            }
            if (_mutation_scale >= 0) {
                mutation_scale = _mutation_scale;
            }
            if (_swap_rate >= 0) {
                swap_rate = 0;
            }
        }

        void sort_agents(bool max = true) {
            std::sort(agents.begin(), agents.end());
            if (max) {
                std::reverse(agents.begin(), agents.end());
            }
        }
        void breed_agents(int keep = 2, bool max = true) {
            sort_agents(max); //sort agents so we keep the current thought of best
            
            auto it = agents.begin();
            int index1, index2, i;
            std::random_device rd;
            std::uniform_int_distribution<int> dist(0, keep);
            
            // move to start of agents to replace
            for ( i = 0; i < keep; i++) {
                (*it).first = 0; // clear score
                it++;
            }
            // replace remaining agents with agents bred from the keep set
            for (; it != agents.end(); it++) {
                (*it).first = 0; // clear score
                index1 = dist(rd);index2 = dist(rd); //choose parents to breed
                (*it).second = bread_mutate<A>(
                        agents[index1].second, //parent 1
                        agents[index2].second, //parent 2
                        div,                   //ratio of parent to use,
                        mutation_rate,         //percent of "genes" to mutate
                        mutation_scale,        //range to mutate "genes"
                        swap_rate);            //rate to swap genes
                
            }
        }
        std::vector<A> return_agents(void) {
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

        void train_epach_single(int num_trials = 1) {
            ASSERT(eval_f_sing != NULL);
            for (int i = 0; i < num_trials; i++) {
                for (auto it = agents.begin(); it != agents.end(); it++) {
                    (*it).first += eval_f_sing( &(*it).second, p_context);
                }
            }
        }

        void train_epach_multi(int num_trials = 1, int num_agents = 2) {
            ASSERT(eval_f_mult != NULL);
            ASSERT(num_agents <= agents.size());
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
                    if (j != num_agents) break; // check that we copied over the right number of agents
                    //evaluate group
                    std::vector<T> res = eval_f_mult( v, p_context );
                    
                    // create iterators to store data
                    auto rit = res.end(); // start at end
                    auto cit = it;
                    
                    // iterate backwards and copy the results into the agents score
                    while (rit != res.begin() && cit != agents.begin()) {
                        rit--; cit--;
                        (*cit).first += *rit;
                    }
                }
            }
        }
        
        
        std::vector<A> train_single(int num_epochs = 1,
                                    int keep = 2,
                                    int num_trials = 1,
                                    bool max = true) {
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
            sort_agents(max);
            // return the vector of agents sorted by current evaluation
            return return_agents();
            
        }
        
        std::vector<A> train_multi(int num_epochs = 1,
                                   int keep = 2,
                                   int num_trials = 1,
                                   int num_agents_per_train = 2,
                                   bool max = true) {
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
            sort_agents(max);
            // return the vector of agents sorted by current evaluation
            return return_agents();
        }
        
        
        void test__random_init_points(void);
    };
};

void test__Genetic_Trainer(void);

#endif /* Genetic_Trainer_hpp */
