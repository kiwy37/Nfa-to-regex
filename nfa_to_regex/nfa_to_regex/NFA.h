#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <algorithm>
#include <set>

using namespace std;

class NFA {
private:
    int startState;
    vector<int> acceptStates;
    unordered_map<int, unordered_map<string, vector<int>>> transitions;
    set<int> allStates;

public:
    const string EPSILON = "$";
public:
    NFA(int start, vector<int> accept) : startState(start), acceptStates(accept) {}
    NFA(int start, vector<int> accept, unordered_map<int, unordered_map<string, vector<int>>> trans) :
        startState(start), acceptStates(accept), transitions(trans) {}

    NFA& operator=(const NFA& other) {
        if (this != &other) { // check for self-assignment
            startState = other.startState;
            acceptStates = other.acceptStates;
            transitions = other.transitions;
        }
        return *this;
    }

    bool existTransition(int a, const char* s, int b);
    bool isRepeatedSubstring(string str, string subStr);
    void eliminateSelfTransitions();
    set<int> getAllStates();
    void addTransition(int state, const char* symbol, int nextState);
    void print();
    void regex();
    bool nullRegex();
    NFA readNFAFromFile(string fileName);
    void addNewStartState();
    void addFinalState();
    void deleteTransition(int currentState, const char* symbol, int nextState);
    bool areBucla(int x);   //* de vazut ca am schimbat 
    int drum(int a, int b);
    int amDrum(int a, int b);
    string getSimbol(int a, int b);
    void addPlus(int a, int b);
    bool containsPlus(string str);
    void removeSimpleState(int a, int q, int b);
    void deleteState(int q);
    set<int> getVecini(int q);
};