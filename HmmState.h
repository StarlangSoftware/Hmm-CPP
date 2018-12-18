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

#endif //HMM_HMMSTATE_H
