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
    HmmState(const State& state, const map<Symbol, double>& emissionProbabilities);
    State getState();
    double getEmitProb(const Symbol& symbol);
    void serialize(ostream& outputFile);
};

/**
 * A constructor of {@link HmmState} class which takes a {@link State} and emission probabilities as inputs and
 * initializes corresponding class variable with these inputs.
 *
 * @param state Data for this state.
 * @param emissionProbabilities Emission probabilities for this state
 */
template<class State, class Symbol> HmmState<State, Symbol>::HmmState(const State& state, const map<Symbol, double>& emissionProbabilities) {
    this->state = state;
    this->emissionProbabilities = emissionProbabilities;
}

/**
 * Accessor method for the state variable.
 *
 * @return state variable.
 */
template<class State, class Symbol> State HmmState<State, Symbol>::getState() {
    return state;
}

/**
 * getEmitProb method returns the emission probability for a specific symbol.
 *
 * @param symbol Symbol for which the emission probability will be get.
 * @return Emission probability for a specific symbol.
 */
template<class State, class Symbol> double HmmState<State, Symbol>::getEmitProb(const Symbol& symbol) {
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
