//
// Created by Olcay Taner Yıldız on 17.12.2018.
//

#ifndef HMM_HMM1_H
#define HMM_HMM1_H


#include <Vector.h>
#include <unordered_set>
#include "Hmm.h"

template <class State, class Symbol> class Hmm1 : public Hmm<State, Symbol> {
private:
    Vector pi = Vector(0, 0.0);
    Vector logOfColumn(int column);
public:
    void calculatePi(int observationCount, vector<State>* observations);
    void calculateTransitionProbabilities(int observationCount, vector<State>* observations);
    vector<State> viterbi(vector<Symbol> s);
    Hmm1(unordered_set<State> states, int observationCount, vector<State> *observations, vector<Symbol> *emittedSymbols);
};

#endif //HMM_HMM1_H
