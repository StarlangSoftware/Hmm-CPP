//
// Created by Olcay Taner Yıldız on 17.12.2018.
//

#include <VectorSizeMismatch.h>
#include "Hmm1.h"

template<class State, class Symbol> void Hmm1<State, Symbol>::calculatePi(int observationCount, vector<State> *observations) {
    pi = Vector(this->stateCount, 0.0);
    for (int i = 0; i < observationCount; i++) {
        vector<State> observation = observations[i];
        unsigned long index = this->stateIndexes.find(observation.at(0))->second;
        pi.addValue(index, 1.0);
    }
    pi.l1Normalize();
}

template<class State, class Symbol> void Hmm1<State, Symbol>::calculateTransitionProbabilities(int observationCount, vector<State> *observations) {
    this->transitionProbabilities = Matrix(this->stateCount, this->stateCount);
    for (int i = 0; i < observationCount; i++) {
        vector<State> current = observations[i];
        for (int j = 0; j < current.size() - 1; j++) {
            int from = this->stateIndexes.find(current.at(j))->second;
            int to = this->stateIndexes.find(current.at(j + 1))->second;
            this->transitionProbabilities.increment(from, to);
        }
    }
    this->transitionProbabilities.columnWiseNormalize();
}

template<class State, class Symbol> Vector Hmm1<State, Symbol>::logOfColumn(int column) {
    Vector result = Vector(0, 0.0);
    int i;
    for (i = 0; i < this->stateCount; i++){
        result.add(safeLog(this->transitionProbabilities.getValue(i, column)));
    }
    return result;
}

template<class State, class Symbol> vector<State> Hmm1<State, Symbol>::viterbi(vector<Symbol> s) {
    int i, j, t;
    Symbol emission;
    vector<State> result;
    unsigned long sequenceLength = s.size(), maxIndex;
    Matrix gamma = Matrix(sequenceLength, this->stateCount);
    Vector qs = Vector(sequenceLength, 0);
    double observationLikelihood;
    Matrix phi = Matrix(sequenceLength, this->stateCount);
    /*Initialize*/
    emission = s.get(0);
    for (i = 0; i < this->stateCount; i++){
        observationLikelihood = this->states.at(i).getEmitProb(emission);
        gamma.setValue(0, i, safeLog(pi.getValue(i)) + this->safeLog(observationLikelihood));
    }
    /*Iterate Dynamic Programming*/
    for (t = 1; t < sequenceLength; t++){
        emission = s.at(t);
        for (j = 0; j < this->stateCount; j++){
            Vector tempArray = logOfColumn(j);
            try {
                tempArray.add(gamma.getRow(t - 1));
            } catch (VectorSizeMismatch& vectorSizeMismatch) {
            }
            maxIndex = tempArray.maxIndex();
            observationLikelihood = this->states.at(j).getEmitProb(emission);
            gamma.setValue(t, j, tempArray.getValue(maxIndex) + this->safeLog(observationLikelihood));
            phi.setValue(t, j, maxIndex);
        }
    }
    /*Backtrack pointers*/
    qs.setValue(sequenceLength - 1, gamma.getRow(sequenceLength - 1).maxIndex());
    result.add(0, this->states[(int) qs.getValue(sequenceLength - 1)].getState());
    for (i = sequenceLength - 2; i >= 0; i--){
        qs.setValue(i, phi.getValue(i + 1, (int) qs.getValue(i + 1)));
        result.add(0, this->states[(int) qs.getValue(i)].getState());
    }
    return result;
}

template<class State, class Symbol> Hmm1<State, Symbol>::Hmm1(unordered_set<State> states, int observationCount, vector<State> *observations, vector<Symbol> *emittedSymbols) {
    int i = 0;
    this->stateCount = states.size();
    for (State state : states){
        this->stateIndexes.emplace(state, i);
        i++;
    }
    calculatePi(observations);
    for (State state : states){
        map<Symbol, double> emissionProbabilities = calculateEmissionProbabilities(state, observationCount, observations, emittedSymbols);
        this->states.emplace_back(state, emissionProbabilities);
    }
    calculateTransitionProbabilities(observations);
}
