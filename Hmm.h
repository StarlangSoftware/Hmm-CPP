//
// Created by Olcay Taner Yıldız on 16.12.2018.
//

#ifndef HMM_HMM_H
#define HMM_HMM_H
#include <Matrix.h>
#include<map>
#include <list>
#include <unordered_set>
#include "HmmState.h"

template <class State, class Symbol> class Hmm {
protected:
    Matrix transitionProbabilities;
    map<State, unsigned long> stateIndexes;
    vector<HmmState<State, Symbol>> states;
    unsigned long stateCount = 0;
public:
    map<Symbol, double> calculateEmissionProbabilities(State state, int observationCount, vector<State>* observations, vector<Symbol>* emittedSymbols);
    double safeLog(double x);
};


#endif //HMM_HMM_H
