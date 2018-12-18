//
// Created by Olcay Taner Yıldız on 17.12.2018.
//

#ifndef HMM_HMM2_H
#define HMM_HMM2_H


#include <Matrix.h>
#include "Hmm.h"

template <class State, class Symbol> class Hmm2 : public Hmm<State, Symbol> {
private:
    Matrix pi = Matrix(0, 0);
    Vector logOfColumn(int column);
public:
    vector<State> viterbi(vector<Symbol> s);
    void calculatePi(int observationCount, vector<State>* observations);
    void calculateTransitionProbabilities(int observationCount, vector<State>* observations);
    Hmm2(unordered_set<State> states, int observationCount, vector<State> *observations, vector<Symbol> *emittedSymbols);
};


#endif //HMM_HMM2_H
