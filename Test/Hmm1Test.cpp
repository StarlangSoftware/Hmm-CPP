//
// Created by Olcay Taner YILDIZ on 26.12.2020.
//

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "../src/Hmm1.h"

TEST_CASE("Hmm1Test-testViterbi") {
    unordered_set<string> states = {"HOT", "COLD"};
    vector<string> *observations;
    observations = new vector<string>[5];
    observations[0] = {"HOT",  "HOT",  "HOT"};
    observations[1] = {"HOT",  "COLD", "COLD", "COLD"};
    observations[2] = {"HOT",  "COLD", "HOT",  "COLD"};
    observations[3] = {"COLD", "COLD", "COLD", "HOT",  "HOT"},
    observations[4] = {"COLD", "HOT",  "HOT",  "COLD", "COLD"};
    vector<int> *emittedSymbols;
    emittedSymbols = new vector<int>[5];
    emittedSymbols[0] = {3, 2, 3};
    emittedSymbols[1] = {2, 2, 1, 1};
    emittedSymbols[2] = {3, 1, 2, 1};
    emittedSymbols[3] = {3, 1, 2, 2, 3};
    emittedSymbols[4] = {1, 2, 3, 2, 1};
    Hmm1<string, int> hmm = Hmm1<string, int>(states, 5, observations, emittedSymbols);
    vector<int> observed = {1, 1, 1, 1, 1, 1};
    vector<string> observedStates = hmm.viterbi(observed);
    REQUIRE("COLD" == observedStates[0]);
    REQUIRE("COLD" == observedStates[1]);
    REQUIRE("COLD" == observedStates[2]);
    REQUIRE("COLD" == observedStates[3]);
    REQUIRE("COLD" == observedStates[4]);
    REQUIRE("COLD" == observedStates[5]);
    observed = {1, 2, 3, 3, 2, 1};
    observedStates = hmm.viterbi(observed);
    REQUIRE("COLD" == observedStates[0]);
    REQUIRE("HOT" == observedStates[1]);
    REQUIRE("HOT" == observedStates[2]);
    REQUIRE("HOT" == observedStates[3]);
    REQUIRE("HOT" == observedStates[4]);
    REQUIRE("COLD" == observedStates[5]);
    observed = {3, 3, 3, 3, 3, 3};
    observedStates = hmm.viterbi(observed);
    REQUIRE("HOT" == observedStates[0]);
    REQUIRE("HOT" == observedStates[1]);
    REQUIRE("HOT" == observedStates[2]);
    REQUIRE("HOT" == observedStates[3]);
    REQUIRE("HOT" == observedStates[4]);
    REQUIRE("HOT" == observedStates[5]);
}
