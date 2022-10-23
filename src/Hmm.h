//
// Created by Olcay Taner Yıldız on 16.12.2018.
//

#ifndef HMM_HMM_H
#define HMM_HMM_H
#include "Matrix.h"
#include<map>
#include <list>
#include <unordered_set>
#include <fstream>
#include "HmmState.h"
#include "CounterHashMap.h"

template <class State, class Symbol> class Hmm {
protected:
    Matrix transitionProbabilities = Matrix(0, 0);
    map<State, unsigned long> stateIndexes;
    vector<HmmState<State, Symbol>> states;
    unsigned long stateCount = 0;
public:
    Hmm() = default;
    explicit Hmm(ifstream& inputFile);
    map<Symbol, double> calculateEmissionProbabilities(const State& state, int observationCount, vector<State>* observations, vector<Symbol>* emittedSymbols) const;
    double safeLog(double x) const;
    virtual void serialize(ostream& outputFile);
};

/**
 * A constructor of {@link Hmm} class which takes a {@link Set} of states, an array of observations (which also
 * consists of an array of states) and an array of instances (which also consists of an array of emitted symbols).
 * The constructor initializes the state array with the set of states and uses observations and emitted symbols
 * to calculate the emission probabilities for those states.
 *
 * @param states A {@link Set} of states, consisting of all possible states for this problem.
 * @param observations An array of instances, where each instance consists of an array of states.
 * @param emittedSymbols An array of instances, where each instance consists of an array of symbols.
 */
template<class State, class Symbol> map<Symbol, double> Hmm<State, Symbol>::calculateEmissionProbabilities(const State& state, int observationCount, vector<State> *observations, vector<Symbol> *emittedSymbols) const{
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
    for (auto it = counts.begin(); it != counts.end(); it++){
        emissionProbabilities.emplace(it->first, it->second / sum);
    }
    return emissionProbabilities;
}

/**
 * safeLog calculates the logarithm of a number. If the number is less than 0, the logarithm is not defined, therefore
 * the function returns -Infinity.
 *
 * @param x Input number
 * @return the logarithm of x. If x < 0 return -infinity.
 */
template<class State, class Symbol> double Hmm<State, Symbol>::safeLog(double x) const{
    if (x <= 0){
        return INT_MIN;
    }
    else{
        return log(x);
    }
}

template<class State, class Symbol> void Hmm<State, Symbol>::serialize(ostream &outputFile) {
    outputFile << stateCount << "\n";
    transitionProbabilities.serialize(outputFile);
    for (auto& iterator : stateIndexes){
        outputFile << iterator.first << "\n";
        outputFile << iterator.second << "\n";
    }
    for (HmmState<State, Symbol> state : states){
        state.serialize(outputFile);
    }
}

template<class State, class Symbol>
Hmm<State, Symbol>::Hmm(ifstream &inputFile) {
    inputFile >> stateCount;
    transitionProbabilities = Matrix(inputFile);
    for (int i = 0; i < stateCount; i++){
        State state;
        inputFile >> state;
        unsigned long index = 0;
        inputFile >> index;
        stateIndexes.emplace(state, index);
    }
    for (int i = 0; i < stateCount; i++){
        states.emplace_back(HmmState<State, Symbol>(inputFile));
    }
}

#endif //HMM_HMM_H
