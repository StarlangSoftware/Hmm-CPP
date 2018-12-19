//
// Created by Olcay Taner Yıldız on 17.12.2018.
//

#ifndef HMM_HMM2_H
#define HMM_HMM2_H


#include <Matrix.h>
#include "Hmm.h"
#include <VectorSizeMismatch.h>

template <class State, class Symbol> class Hmm2 : public Hmm<State, Symbol> {
private:
    Matrix pi = Matrix(0, 0);
    Vector logOfColumn(int column);
public:
    vector<State> viterbi(vector<Symbol> s);
    void calculatePi(int observationCount, vector<State>* observations);
    void calculateTransitionProbabilities(int observationCount, vector<State>* observations);
    Hmm2(unordered_set<State> states, int observationCount, vector<State> *observations, vector<Symbol> *emittedSymbols);
};

template<class State, class Symbol> void Hmm2<State, Symbol>::calculatePi(int observationCount, vector<State> *observations) {
    pi = Matrix(this->stateCount, this->stateCount);
    for (int i = 0; i < observationCount; i++) {
        vector<State> observation = observations[i];
        int first = this->stateIndexes.find(observation.at(0))->second;
        int second = this->stateIndexes.find(observation.at(1))->second;
        pi.increment(first, second);
    }
    pi.columnWiseNormalize();
}

template<class State, class Symbol> void Hmm2<State, Symbol>::calculateTransitionProbabilities(int observationCount, vector<State> *observations) {
    this->transitionProbabilities = Matrix(this->stateCount * this->stateCount, this->stateCount);
    for (int i = 0; i < observationCount; i++) {
        vector<State> current = observations[i];
        for (int j = 0; j < current.size() - 2; j++) {
            int from1 = this->stateIndexes.find(current.at(j))->second;
            int from2 = this->stateIndexes.find(current.at(j + 1))->second;
            int to = this->stateIndexes.find(current.at(j + 2))->second;
            this->transitionProbabilities.increment(from1 * this->stateCount + from2, to);
        }
    }
    this->transitionProbabilities.columnWiseNormalize();
}

template<class State, class Symbol> Vector Hmm2<State, Symbol>::logOfColumn(int column) {
    Vector result = Vector(0, 0.0);
    int i;
    for (i = 0; i < this->stateCount; i++){
        result.add(this->safeLog(this->transitionProbabilities.getValue(i * this->stateCount + column / this->stateCount, column % this->stateCount)));
    }
    return result;
}

template<class State, class Symbol> vector<State> Hmm2<State, Symbol>::viterbi(vector<Symbol> s) {
    int i, j, t, maxIndex;
    int sequenceLength = s.size();
    Symbol emission, emission1, emission2;
    vector<State> result;
    Matrix gamma = Matrix(sequenceLength, this->stateCount * this->stateCount);
    Vector qs = Vector(sequenceLength, 0);
    double observationLikelihood;
    Matrix phi = Matrix(sequenceLength, this->stateCount * this->stateCount);
    /*Initialize*/
    emission1 = s.at(0);
    emission2 = s.at(1);
    for (i = 0; i < this->stateCount; i++){
        for (j = 0; j < this->stateCount; j++){
            observationLikelihood = this->states.at(i).getEmitProb(emission1) * this->states.at(j).getEmitProb(emission2);
            gamma.setValue(1, i * this->stateCount + j, this->safeLog(pi.getValue(i, j)) + this->safeLog(observationLikelihood));
        }
    }
    /*Iterate Dynamic Programming*/
    for (t = 2; t < sequenceLength; t++){
        emission = s.at(t);
        for (j = 0; j < this->stateCount * this->stateCount; j++){
            Vector current = logOfColumn(j);
            try {
                Vector previous = gamma.getRow(t - 1).skipVector(this->stateCount, j / this->stateCount);
                current.add(previous);
            } catch (VectorSizeMismatch& vectorSizeMismatch) {
            }
            maxIndex = current.maxIndex();
            observationLikelihood = this->states.at(j % this->stateCount).getEmitProb(emission);
            gamma.setValue(t, j, current.getValue(maxIndex) + this->safeLog(observationLikelihood));
            phi.setValue(t, j, maxIndex * this->stateCount + j / this->stateCount);
        }
    }
    /*Backtrack pointers*/
    qs.setValue(sequenceLength - 1, gamma.getRow(sequenceLength - 1).maxIndex());
    result.insert(result.begin(), this->states[((int) qs.getValue(sequenceLength - 1)) % this->stateCount].getState());
    for (i = sequenceLength - 2; i >= 1; i--){
        qs.setValue(i, phi.getValue(i + 1, (int) qs.getValue(i + 1)));
        result.insert(result.begin(), this->states[((int) qs.getValue(i)) % this->stateCount].getState());
    }
    result.insert(result.begin(), this->states[((int) qs.getValue(1)) / this->stateCount].getState());
    return result;
}

template<class State, class Symbol> Hmm2<State, Symbol>::Hmm2(unordered_set<State> states, int observationCount, vector<State> *observations, vector<Symbol> *emittedSymbols) {
    int i = 0;
    this->stateCount = states.size();
    for (State state : states){
        this->stateIndexes.emplace(state, i);
        i++;
    }
    calculatePi(observationCount, observations);
    for (State state : states){
        map<Symbol, double> emissionProbabilities = this->calculateEmissionProbabilities(state, observationCount, observations, emittedSymbols);
        this->states.emplace_back(state, emissionProbabilities);
    }
    calculateTransitionProbabilities(observationCount, observations);
}

#endif //HMM_HMM2_H
