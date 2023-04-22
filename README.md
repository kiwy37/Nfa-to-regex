# Nfa-to-regex

This C++ application converts a nondeterministic finite automaton (NFA) into a regular expression using the reduction method. To achieve this, it first adds a new start and finish state to the NFA and then eliminates the states one by one. Once all the states have been eliminated, the final transition in the map represents the equivalent regular expression. This application was developed for the ACC course (Automate, Calculabilitate și Complexitate).
