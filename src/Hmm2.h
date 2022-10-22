//
// Created by Olcay Taner Yıldız on 17.12.2018.
//

#ifndef HMM_HMM2_H
#define HMM_HMM2_H


#include "Matrix.h"
#include "Hmm.h"
#include "VectorSizeMismatch.h"

template <class State, class Symbol> class Hmm2 : public Hmm<State, Symbol> {
private:
    Matrix pi = Matrix(0, 0);
    Vector logOfColumn(int column);
public:
    Hmm2();
    explicit Hmm2(ifstream& inputFile);
    vector<State> viterbi(const vector<Symbol>& s);
    void calculatePi(int observationCount, vector<State>* observations);
    void calculateTransitionProbabilities(int observationCount, vector<State>* observations);
    Hmm2(const unordered_set<State>& states, int observationCount, vector<State> *observations, vector<Symbol> *emittedSymbols);
    void serialize(ostream& outputFile) override;
};

template<class State, class Symbol> Hmm2<State, Symbol>::Hmm2() = default;

/**
 * calculatePi calculates the prior probability matrix (initial probabilities for each state combinations)
 * from a set of observations. For each observation, the function extracts the first and second states in
 * that observation.  Normalizing the counts of the pair of states returns us the prior probabilities for each
 * pair of states.
 *
 * @param observations A set of observations used to calculate the prior probabilities.
 */
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

/**
 * calculateTransitionProbabilities calculates the transition probabilities matrix from each state to another state.
 * For each observation and for each transition in each observation, the function gets the states. Normalizing the
 * counts of the three of states returns us the transition probabilities.
 *
 * @param observations A set of observations used to calculate the transition probabilities.
 */
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

/**
 * logOfColumn calculates the logarithm of each value in a specific column in the transition probability matrix.
 *
 * @param column Column index of the transition probability matrix.
 * @return A vector consisting of the logarithm of each value in the column in the transition probability matrix.
 */
template<class State, class Symbol> Vector Hmm2<State, Symbol>::logOfColumn(int column) {
    Vector result = Vector(0, 0.0);
    int i;
    for (i = 0; i < this->stateCount; i++){
        result.add(this->safeLog(this->transitionProbabilities.getValue(i * this->stateCount + column / this->stateCount, column % this->stateCount)));
    }
    return result;
}

/**
 * viterbi calculates the most probable state sequence for a set of observed symbols.
 *
 * @param s A set of observed symbols.
 * @return The most probable state sequence as an {@link ArrayList}.
 */
template<class State, class Symbol> vector<State> Hmm2<State, Symbol>::viterbi(const vector<Symbol>& s) {
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

/**
 * A constructor of {@link Hmm2} class which takes a {@link Set} of states, an array of observations (which also
 * consists of an array of states) and an array of instances (which also consists of an array of emitted symbols).
 * The constructor calls its super method to calculate the emission probabilities for those states.
 *
 * @param states A {@link Set} of states, consisting of all possible states for this problem.
 * @param observations An array of instances, where each instance consists of an array of states.
 * @param emittedSymbols An array of instances, where each instance consists of an array of symbols.
 */
template<class State, class Symbol> Hmm2<State, Symbol>::Hmm2(const unordered_set<State>& states, int observationCount, vector<State> *observations, vector<Symbol> *emittedSymbols) {
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
void Hmm2<State, Symbol>::serialize(ostream &outputFile) {
    Hmm<State, Symbol>::serialize(outputFile);
    pi.serialize(outputFile);
}

template<class State, class Symbol>
Hmm2<State, Symbol>::Hmm2(ifstream &inputFile):Hmm<State, Symbol>(inputFile) {
    pi = Matrix(inputFile);
}

#endif //HMM_HMM2_H
