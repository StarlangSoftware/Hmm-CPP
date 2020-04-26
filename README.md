# Hmm-CPP

For Developers
============
You can also see [Java](https://github.com/starlangsoftware/Hmm), [Python](https://github.com/starlangsoftware/Hmm-Py),  or [C#](https://github.com/starlangsoftware/Hmm-CS) repository.

Detailed Description
============
+ [Hmm](#hmm)

## Hmm

Hmm modelini üretmek için

	Hmm(Set<State> states, ArrayList<State>[] observations, ArrayList<Symbol>[] emittedSymbols)


Viterbi algoritması ile en olası State listesini elde etmek için

	ArrayList<State> viterbi(ArrayList<Symbol> s)
