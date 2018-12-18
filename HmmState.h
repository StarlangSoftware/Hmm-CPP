//
// Created by Olcay Taner Yıldız on 16.12.2018.
//

#ifndef HMM_HMMSTATE_H
#define HMM_HMMSTATE_H
#include<map>
using namespace std;

template <class State, class Symbol> class HmmState {
protected:
    map<Symbol, double> emissionProbabilities;
    State state;
public:
    HmmState(State state, map<Symbol, double> emissionProbabilities);
    State getState();
    double getEmitProb(Symbol symbol);
};

template<class State, class Symbol> HmmState<State, Symbol>::HmmState(State state, map<Symbol, double> emissionProbabilities) {
    this->state = state;
    this->emissionProbabilities = emissionProbabilities;
}

template<class State, class Symbol> State HmmState<State, Symbol>::getState() {
    return state;
}

template<class State, class Symbol> double HmmState<State, Symbol>::getEmitProb(Symbol symbol) {
    if (emissionProbabilities.find(symbol) != emissionProbabilities.end()){
        return emissionProbabilities.find(symbol)->second;
    } else {
        return 0.0;
    }
}

#endif //HMM_HMMSTATE_H
