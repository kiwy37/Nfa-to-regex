#include "NFA.h"

int main() {
    NFA nfa = NFA(0, {}, {});
    nfa = nfa.readNFAFromFile("fileName.txt");
    cout << "Automatul este:\n";
    nfa.print();

    nfa.eliminateSelfTransitions();
    if (!nfa.nullRegex()) {
        set<int> allStates = nfa.getAllStates();
        for (auto i : allStates) {
            for (auto j : allStates) {
                nfa.addPlus(i, j);
            }
        }
        nfa.addNewStartState();
        nfa.addFinalState();

        allStates = nfa.getAllStates();

        allStates.erase(allStates.size() - 1);
        allStates.erase(0);

        for (auto x : allStates) {

            set<pair<int, int>> perechi;
            set<int> vecini = nfa.getVecini(x);
            for (auto i : vecini)
                for (auto j : vecini) {
                    if (i != j) {
                        nfa.removeSimpleState(i, x, j);
                    }
                }
            nfa.deleteState(x);
        }
    
        cout << "\nExpresia regulata aferenta automatului este:\n";
        nfa.regex();
    }
    else
    {
        cout << "\nRegexul rezultat este regexul nul.";
    }
    return 0;
}
