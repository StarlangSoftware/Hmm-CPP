# Hmm-CPP
You can also see either [Java](https://github.com/olcaytaner/Hmm) 
or [Python](https://github.com/olcaytaner/Hmm-Py) repository.

Detailed Description
============
+ [Hmm](#hmm)

## Hmm

Hmm modelini üretmek için

	Hmm(Set<State> states, ArrayList<State>[] observations, ArrayList<Symbol>[] emittedSymbols)


Viterbi algoritması ile en olası State listesini elde etmek için

	ArrayList<State> viterbi(ArrayList<Symbol> s)
