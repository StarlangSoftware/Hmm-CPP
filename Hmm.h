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
#include <CounterHashMap.h>

template <class State, class Symbol> class Hmm {
protected:
    Matrix transitionProbabilities = Matrix(0, 0);
    map<State, unsigned long> stateIndexes;
    vector<HmmState<State, Symbol>> states;
    unsigned long stateCount = 0;
public:
    map<Symbol, double> calculateEmissionProbabilities(State state, int observationCount, vector<State>* observations, vector<Symbol>* emittedSymbols);
    double safeLog(double x);
};

template<class State, class Symbol> map<Symbol, double> Hmm<State, Symbol>::calculateEmissionProbabilities(State state, int observationCount, vector<State> *observations, vector<Symbol> *emittedSymbols) {
    CounterHashMap<Symbol> counts;
    map<Symbol, double> emissionProbabilities;
    State currentState;
    Symbol currentSymbol;
    for (int i = 0; i < observationCount; i++){
        for (int j = 0; j < observations[i].size(); j++){
            currentState = observations[i].at(j);
            currentSymbol = emittedSymbols[i].at(j);
            if (currentState == state){
                counts.put(currentSymbol);
            }
        }
    }
    double sum = counts.sumOfCounts();
    for (auto item :counts){
        emissionProbabilities.emplace(item.first, item.second / sum);
    }
    return emissionProbabilities;
}

template<class State, class Symbol> double Hmm<State, Symbol>::safeLog(double x) {
    if (x <= 0){
        return INT_MIN;
    }
    else{
        return log(x);
    }
}

#endif //HMM_HMM_H
