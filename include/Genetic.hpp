//
//  Genetic.hpp
//  GAME_TEST_LIB
//
//  Created by Quinn Abrahams-Vaughn on 1/10/19.
//

#ifndef Genetic_hpp
#define Genetic_hpp
#include "Array.hpp"

template<class T>
class Genetic_Array:public Array<T> {
    void i_random(T min = 0, T max = 10);
    void r_random(T min = 0, T max = 10);
public:
    Genetic_Array(Array<T> A) : Array<T>(A) {};
    Genetic_Array(Shape s) : Array<T>(s) {random();}
    Genetic_Array(int cols = 1, int rows = 1) : Array<T>(cols,rows) {random();}
    
    void random(T min = 0, T max = 10);
    Genetic_Array<T> breed(Genetic_Array& G);
    void mutate(float mutation_rate = 0.0);
    Genetic_Array bread_mutate(Genetic_Array& G, float mutation_rate = 0.0);
};
template<class T>
Genetic_Array<T> f_breed(Genetic_Array<T>& A, Genetic_Array<T>& B );

void test__Genetic(void);
#endif /* Genetic_hpp */
