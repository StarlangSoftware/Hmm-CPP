//
// Created by LMF319018 on 18/12/2018.
//
#include <vector>
#include <unordered_set>
#include "Hmm1.h"
#include "Hmm2.h"

using namespace std;

int main(){
    int observationCount = 5;
    vector<string>* observations;
    vector<int>* emittedSymbols;
    unordered_set<string> states;
    unordered_set<int> symbols;
    vector<int> observed;
    observed.push_back(1);
    observed.push_back(2);
    observed.push_back(3);
    observed.push_back(3);
    observed.push_back(2);
    observed.push_back(1);
    states.emplace("HOT");
    states.emplace("COLD");
    symbols.emplace(1);
    symbols.emplace(2);
    symbols.emplace(3);
    observations = new vector<string>[observationCount];
    emittedSymbols = new vector<int>[observationCount];
    observations[0].emplace_back("HOT");
    observations[0].emplace_back("HOT");
    observations[0].emplace_back("HOT");
    emittedSymbols[0].push_back(3);
    emittedSymbols[0].push_back(2);
    emittedSymbols[0].push_back(3);
    observations[1].emplace_back("HOT");
    observations[1].emplace_back("COLD");
    observations[1].emplace_back("COLD");
    observations[1].emplace_back("COLD");
    emittedSymbols[1].push_back(2);
    emittedSymbols[1].push_back(2);
    emittedSymbols[1].push_back(1);
    emittedSymbols[1].push_back(1);
    observations[2].emplace_back("HOT");
    observations[2].emplace_back("COLD");
    observations[2].emplace_back("HOT");
    observations[2].emplace_back("COLD");
    emittedSymbols[2].push_back(3);
    emittedSymbols[2].push_back(1);
    emittedSymbols[2].push_back(2);
    emittedSymbols[2].push_back(1);
    observations[3].emplace_back("COLD");
    observations[3].emplace_back("COLD");
    observations[3].emplace_back("COLD");
    observations[3].emplace_back("HOT");
    observations[3].emplace_back("HOT");
    emittedSymbols[3].push_back(3);
    emittedSymbols[3].push_back(1);
    emittedSymbols[3].push_back(2);
    emittedSymbols[3].push_back(2);
    emittedSymbols[3].push_back(3);
    observations[4].emplace_back("COLD");
    observations[4].emplace_back("HOT");
    observations[4].emplace_back("HOT");
    observations[4].emplace_back("COLD");
    observations[4].emplace_back("COLD");
    emittedSymbols[4].push_back(1);
    emittedSymbols[4].push_back(2);
    emittedSymbols[4].push_back(3);
    emittedSymbols[4].push_back(2);
    emittedSymbols[4].push_back(1);
    Hmm1<string, int> hmm(states, 5, observations, emittedSymbols);
    vector<string> observedStates = hmm.viterbi(observed);
}