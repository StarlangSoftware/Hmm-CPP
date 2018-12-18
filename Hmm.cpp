//
// Created by Olcay Taner Yıldız on 16.12.2018.
//

#include <CounterHashMap.h>
#include "Hmm.h"

template<class State, class Symbol> Hmm<State, Symbol>::Hmm(unordered_set<State> states, int observationCount, vector<State> *observations, vector<Symbol> *emittedSymbols) {
    int i = 0;
    stateCount = states.size();
    for (State state : states){
        stateIndexes.emplace(state, i);
        i++;
    }
    calculatePi(observations);
    for (State state : states){
        map<Symbol, double> emissionProbabilities = calculateEmissionProbabilities(state, observationCount, observations, emittedSymbols);
        this->states.emplace_back(state, emissionProbabilities);
    }
    calculateTransitionProbabilities(observations);
}

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
    for (iterator<Symbol, int> iterator = counts.begin(); iterator != counts.end; iterator++){
        emissionProbabilities.emplace(iterator->first, iterator->second / sum);
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
