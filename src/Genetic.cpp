//
//  Genetic.cpp
//  GAME_TEST_LIB
//
//  Created by Quinn Abrahams-Vaughn on 1/10/19.
//

#include "Genetic.hpp"
#include <random>
#include <type_traits>

template<class T>
void Genetic_Array<T>::mutate(float mutation_rate) {
    ASSERT(mutation_rate > 0);
    Shape s = this->shape();
    int size = s.first*s.second;
    std::default_random_engine generator;
    std::uniform_int_distribution<int> dist(0, size);
    std::uniform_real_distribution<float>  mute(1-mutation_rate, 1+mutation_rate);
    dist.reset();
    int changes = size * mutation_rate;
    while (changes-- > 0) {
        int r = dist(generator);
        int col = r%s.first; int row = r%s.second;
        (*this)(col,row) *= mute( generator );
    }
}

template<class T>
Genetic_Array<T> f_breed(Genetic_Array<T>& A, Genetic_Array<T>& B ) {
    Shape s = A.shape(); Shape t = B.shape();
    ASSERT(s==t);
    std::default_random_engine generator;
    std::uniform_int_distribution<int> dist(0, 1);
    dist.reset();
    Genetic_Array<T> out(s);
    for (int i = 0; i < s.first; i++) {
        for (int j = 0; j < s.second; j++) {
            if (dist(generator)) {
                out(i,j) = A(i,j);
            } else {
                out(i,j) = B(i,j);
            }
        }
    }
    return out;
}

template<class T>
Genetic_Array<T> Genetic_Array<T>::breed(Genetic_Array& G ) {
    return f_breed(*this, G);
}

template<class T>
Genetic_Array<T> Genetic_Array<T>::bread_mutate(Genetic_Array<T>& G, float mutation_rate) {
    Genetic_Array<T> out = breed(G);
    out.mutate(mutation_rate);
    return out;
}
template<class T>
void Genetic_Array<T>::r_random(T min, T max) {
    std::default_random_engine generator;
    std::uniform_real_distribution<T> dist(min,max);
    for (auto rit = this->begin(); rit != this->end(); rit++) {
        for (auto cit = (*rit).begin(); cit != (*rit).end(); cit++) {
            *cit = dist(generator);
        }
    }
}

template<class T>
void Genetic_Array<T>::i_random(T min, T max) {
    std::default_random_engine generator;
    std::uniform_int_distribution<T> dist(min,max);
    for (auto rit = this->begin(); rit != this->end(); rit++) {
        for (auto cit = (*rit).begin(); cit != (*rit).end(); cit++) {
            *cit = dist(generator);
        }
    }
}

template<class T>
void Genetic_Array<T>::random(T min, T max) {
    if (std::is_integral<T>::value) {
        i_random(min, max);
    } else if (std::is_floating_point<T>::value) {
        r_random(min, max);
    } else {
        ASSERT(false);
    }
}


#ifdef  TESTING
#include "unity.h"
#include <iostream>


template<class T>
void test__Genetic__random(void) {
    for (int i = 1; i < 20; i++) {
        for (int j = 1; j < i; j++) {
            Genetic_Array<T> G(i,j);
            TEST_ASSERT(G>=0);
            TEST_ASSERT(G<=10);
            G.random(10,20);
            TEST_ASSERT(G>=10);
            TEST_ASSERT(G<=20);
        }
    }
}
template<class T>
void test__Genetic__breed(void) {
    for (int i = 1; i < 20; i++) {
        for (int j = 1; j < i; j++) {
            Genetic_Array<T> A(i,j), B(i,j);
            Genetic_Array<T> C = A.breed(B);
            TEST_ASSERT(C.shape()==A.shape());
            for (int x = 0; x < i; x++) {
                for (int y = 0; y < j; y++) {
                    TEST_ASSERT(A(x,y)==C(x,y) || B(x,y)==C(x,y));
                }
            }
            Genetic_Array<T> D = f_breed(C, A);
            TEST_ASSERT(D.shape()==A.shape());
            for (int x = 0; x < i; x++) {
                for (int y = 0; y < j; y++) {
                    TEST_ASSERT(A(x,y)==D(x,y) || C(x,y)==D(x,y));
                }
            }
        }
    }
}
template<class T>
void test__Genetic__mutate(void) {
    for (int i = 1; i < 20; i++) {
        for (int j = 1; j < i; j++) {
            Genetic_Array<T> G = Array_Ones<T>(i,j);
            TEST_ASSERT(G==1);
            G.mutate(1);
            TEST_ASSERT(G!=1);
            G = Array_Ones<T>(i,j);
            G *= 10;
            TEST_ASSERT(G==10);
            G.mutate(1);
            TEST_ASSERT(G!=10);
        }
    }
}

void test__Genetic(void) {
    _NUM_T_RUN_TEST(test__Genetic__random);
    _NUM_T_RUN_TEST(test__Genetic__breed);
    _NUM_T_RUN_TEST(test__Genetic__mutate);
}

#endif//TESTING


