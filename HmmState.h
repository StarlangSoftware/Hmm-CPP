//
// Created by Olcay Taner Yıldız on 16.12.2018.
//

#ifndef HMM_HMMSTATE_H
#define HMM_HMMSTATE_H
#include<map>
#include <fstream>
using namespace std;

template <class State, class Symbol> class HmmState {
protected:
    map<Symbol, double> emissionProbabilities;
    State state;
public:
    explicit HmmState(ifstream& inputFile);
    HmmState(State state, map<Symbol, double> emissionProbabilities);
    State getState();
    double getEmitProb(Symbol symbol);
    void serialize(ostream& outputFile);
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

template<class State, class Symbol>
void HmmState<State, Symbol>::serialize(ostream &outputFile) {
    outputFile << state << "\n";
    outputFile << emissionProbabilities.size() << "\n";
    for (auto& iterator : emissionProbabilities){
        outputFile << iterator.first << "\n";
        outputFile << iterator.second << "\n";
    }
}

template<class State, class Symbol>
HmmState<State, Symbol>::HmmState(ifstream &inputFile) {
    inputFile >> state;
    int size;
    inputFile >> size;
    for (int i = 0; i < size; i++){
        Symbol symbol;
        inputFile >> symbol;
        double p;
        inputFile >> p;
        emissionProbabilities.emplace(symbol, p);
    }
}

#endif //HMM_HMMSTATE_H
