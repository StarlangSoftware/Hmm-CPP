//
// Created by Olcay Taner Yıldız on 17.12.2018.
//

#ifndef HMM_HMM1_H
#define HMM_HMM1_H

#include <Vector.h>
#include <unordered_set>
#include <VectorSizeMismatch.h>
#include "Hmm.h"

template <class State, class Symbol> class Hmm1 : public Hmm<State, Symbol> {
private:
    Vector pi = Vector(0, 0.0);
    Vector logOfColumn(int column);
public:
    Hmm1();
    explicit Hmm1(ifstream& inputFile);
    void calculatePi(int observationCount, vector<State>* observations);
    void calculateTransitionProbabilities(int observationCount, vector<State>* observations);
    vector<State> viterbi(vector<Symbol> s);
    Hmm1(unordered_set<State> states, int observationCount, vector<State> *observations, vector<Symbol> *emittedSymbols);
    void serialize(ostream& outputFile) override;
};

template<class State, class Symbol> Hmm1<State, Symbol>::Hmm1() {
}

/**
 * calculatePi calculates the prior probability vector (initial probabilities for each state) from a set of
 * observations. For each observation, the function extracts the first state in that observation. Normalizing the
 * counts of the states returns us the prior probabilities for each state.
 *
 * @param observations A set of observations used to calculate the prior probabilities.
 */
template<class State, class Symbol> void Hmm1<State, Symbol>::calculatePi(int observationCount, vector<State> *observations) {
    pi = Vector(this->stateCount, 0.0);
    for (int i = 0; i < observationCount; i++) {
        vector<State> observation = observations[i];
        unsigned long index = this->stateIndexes.find(observation.at(0))->second;
        pi.addValue(index, 1.0);
    }
    pi.l1Normalize();
}

/**
 * calculateTransitionProbabilities calculates the transition probabilities matrix from each state to another state.
 * For each observation and for each transition in each observation, the function gets the states. Normalizing the
 * counts of the pair of states returns us the transition probabilities.
 *
 * @param observations A set of observations used to calculate the transition probabilities.
 */
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

/**
 * logOfColumn calculates the logarithm of each value in a specific column in the transition probability matrix.
 *
 * @param column Column index of the transition probability matrix.
 * @return A vector consisting of the logarithm of each value in the column in the transition probability matrix.
 */
template<class State, class Symbol> Vector Hmm1<State, Symbol>::logOfColumn(int column) {
    Vector result = Vector(0, 0.0);
    int i;
    for (i = 0; i < this->stateCount; i++){
        result.add(this->safeLog(this->transitionProbabilities.getValue(i, column)));
    }
    return result;
}

/**
 * viterbi calculates the most probable state sequence for a set of observed symbols.
 *
 * @param s A set of observed symbols.
 * @return The most probable state sequence as an {@link ArrayList}.
 */
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
    emission = s.at(0);
    for (i = 0; i < this->stateCount; i++){
        observationLikelihood = this->states.at(i).getEmitProb(emission);
        gamma.setValue(0, i, this->safeLog(pi.getValue(i)) + this->safeLog(observationLikelihood));
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
    result.insert(result.begin(), this->states[(int) qs.getValue(sequenceLength - 1)].getState());
    for (i = sequenceLength - 2; i >= 0; i--){
        qs.setValue(i, phi.getValue(i + 1, (int) qs.getValue(i + 1)));
        result.insert(result.begin(), this->states[(int) qs.getValue(i)].getState());
    }
    return result;
}

/**
 * A constructor of {@link Hmm1} class which takes a {@link Set} of states, an array of observations (which also
 * consists of an array of states) and an array of instances (which also consists of an array of emitted symbols).
 * The constructor calls its super method to calculate the emission probabilities for those states.
 *
 * @param states A {@link Set} of states, consisting of all possible states for this problem.
 * @param observations An array of instances, where each instance consists of an array of states.
 * @param emittedSymbols An array of instances, where each instance consists of an array of symbols.
 */
template<class State, class Symbol> Hmm1<State, Symbol>::Hmm1(unordered_set<State> states, int observationCount, vector<State> *observations, vector<Symbol> *emittedSymbols) {
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

template<class State, class Symbol>
void Hmm1<State, Symbol>::serialize(ostream &outputFile) {
    Hmm<State, Symbol>::serialize(outputFile);
    pi.serialize(outputFile);
}

template<class State, class Symbol>
Hmm1<State, Symbol>::Hmm1(ifstream &inputFile):Hmm<State, Symbol>(inputFile) {
    pi = Vector(inputFile);
}

#endif //HMM_HMM1_H
