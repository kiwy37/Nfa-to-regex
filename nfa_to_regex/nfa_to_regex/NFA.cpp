#include "NFA.h"

using namespace std;

bool NFA::existTransition(int a, const char* s, int b)
{
	for (auto const& [state, transitionsFromState] : transitions) {
		for (auto const& [symbol, nextStates] : transitionsFromState) {
			if(state==a&&symbol==s)
			for (int nextState : nextStates) {
				if (nextState == b)
					return true;
			}
		}
	}
	return false;
}

bool NFA::isRepeatedSubstring(string str, string subStr)
{
	int subLen = subStr.length();
	int strLen = str.length();

	if (strLen % subLen != 0) { 
		return false;
	}

	for (int i = 0; i < strLen; i += subLen) { 
		if (str.substr(i, subLen) != subStr) {
			return false;
		}
	}

	return true;
}

void NFA::eliminateSelfTransitions()
{
	vector<int> stateElim;
	for (auto const& [state, transitionsFromState] : transitions) {
		for (auto const& [symbol, nextStates] : transitionsFromState) {
			for (int nextState : nextStates) {
				if (state == nextState && symbol == EPSILON)
					stateElim.push_back(state);
			}
		}
	}
	for (int i = 0;i < stateElim.size();i++)
		deleteTransition(stateElim[i], "$", stateElim[i]);
}

set<int> NFA::getAllStates()
{
	return allStates;
}

void NFA::addTransition(int state, const char* symbol, int nextState)
{
	transitions[state][std::string(symbol)].push_back(nextState);
}

void NFA::print()
{
	cout << "Start State: " << startState << endl;
	cout << "Accept States: ";
	for (int acceptState : acceptStates) {
		cout << acceptState << " ";
	}
	cout << endl;
	cout << "Transitions: " << endl;
	for (auto const& [state, transitionsFromState] : transitions) {
		for (auto const& [symbol, nextStates] : transitionsFromState) {
			cout << "(" << state << ", " << symbol << ") -> ";
			for (int nextState : nextStates) {
				cout << nextState << " ";
			}
			cout << endl;
		}
	}
}

void NFA::regex()
{
	for (auto const& [state, transitionsFromState] : transitions) {
		for (auto const& [symbol, nextStates] : transitionsFromState) {
			cout << symbol;
		}
	}
}

bool NFA::nullRegex()
{
	for (auto const& [state, transitionsFromState] : transitions) {
		for (auto const& [symbol, nextStates] : transitionsFromState) {
			if (symbol != "$")
				return false;
		}
	}
	return true;
}

NFA NFA::readNFAFromFile(string fileName)
{
	ifstream inFile(fileName);
	if (!inFile.is_open()) {
		cerr << "Error: Unable to open file " << fileName << endl;
		exit(1);
	}

	int startState;
	vector<int> acceptStates;
	int numTransitions;
	unordered_map<int, unordered_map<string, vector<int>>> transitions;

	inFile >> startState;
	int numAcceptStates;
	inFile >> numAcceptStates;
	for (int i = 0; i < numAcceptStates; i++) {
		int acceptState;
		inFile >> acceptState;
		acceptStates.push_back(acceptState);
	}

	inFile >> numTransitions;
	for (int i = 0; i < numTransitions; i++) {
		int currentState, nextState;
		string symbol;
		inFile >> currentState >> symbol >> nextState;
		transitions[currentState][symbol].push_back(nextState);
		allStates.insert(currentState);
		allStates.insert(nextState);
	}
	return NFA(startState, acceptStates, transitions);
}

void NFA::addNewStartState()
{
	int newStartState = 0;
	unordered_map<string, vector<int>> epsilonTransitions = { {EPSILON, {startState}} };
	transitions[newStartState] = epsilonTransitions;
	startState = newStartState;
	allStates.insert(0);
}

void NFA::addFinalState()
{
	int newState = getAllStates().size();
	allStates.insert(newState);
	unordered_map<string, vector<int>> epsilonTransitions; 

	for (int acceptState : acceptStates) {
		addTransition(acceptState, "$", newState);
		transitions[newState] = epsilonTransitions;
	}

	acceptStates.clear();
	acceptStates.push_back(newState);
}

void NFA::deleteTransition(int currentState, const char* symbol, int nextState)
{
	auto symbolTransitions = transitions.find(currentState);
	if (symbolTransitions != transitions.end()) {
		auto nextStateList = symbolTransitions->second.find(symbol);
		if (nextStateList != symbolTransitions->second.end()) {
			auto it = find(nextStateList->second.begin(), nextStateList->second.end(), nextState);
			if (it != nextStateList->second.end()) {
				nextStateList->second.erase(it);
				if (nextStateList->second.empty()) {
					symbolTransitions->second.erase(nextStateList);
				}
				if (symbolTransitions->second.empty()) {
					transitions.erase(symbolTransitions);
				}
			}
		}
	}
}

bool NFA::areBucla(int x)
{
	for (auto const& [state, transitionsFromState] : transitions) {
		for (auto const& [symbol, nextStates] : transitionsFromState) {
			for (int nextState : nextStates) {
				if (state == nextState && state == x) {
					if (symbol != EPSILON)
						return true;
				}
			}
		}
	}
	return false;
}

int NFA::drum(int a, int b)
{
	int dus = 0, intors = 0;
	for (auto const& [state, transitionsFromState] : transitions) {
		for (auto const& [symbol, nextStates] : transitionsFromState) {
			for (int nextState : nextStates) {
				if (state == a && nextState == b && symbol != "$") {
					dus++;
				}
				if (state == b && nextState == a && symbol != "$") {
					intors++;
				}
			}
		}
	}
	if (dus == 1 && intors == 1)
		return 0;
	if (dus == 1 && intors == 0)
		return 1;
	if (dus == 0 && intors == 1)
		return -1;
	else
		return 9;
}

int NFA::amDrum(int a, int b)
{
	for (auto const& [state, transitionsFromState] : transitions)
		for (auto const& [symbol, nextStates] : transitionsFromState) {
			for (int nextState : nextStates) {
				if (state == a && nextState == b) {
					return 1;
				}
			}
		}
	return 0;
}

string NFA::getSimbol(int a, int b)
{
	for (auto const& [state, transitionsFromState] : transitions) {
		for (auto const& [symbol, nextStates] : transitionsFromState) {
			for (int nextState : nextStates) {
				if (state == a && nextState == b) {
					if (symbol != EPSILON)
						return symbol;
				}
			}
		}
	}
	return "-";
}

void NFA::addPlus(int a, int b)
{
	int count = 0;
	string str = "";
	vector<string> tranzitiiDeSters;
	for (auto const& [state, transitionsFromState] : transitions) {
		for (auto const& [symbol, nextStates] : transitionsFromState) {
			for (int nextState : nextStates) {
				if (state == a && nextState == b)
					if (symbol != EPSILON) {
						if (count > 0)
							str += "|" + symbol;
						else
							str += symbol;
						count++;
						tranzitiiDeSters.push_back(symbol);
					}
			}
		}
	}
	for (auto i : tranzitiiDeSters)
		deleteTransition(a, i.c_str(), b);
	if (str != "")
		addTransition(a, str.c_str(), b);
}

bool NFA::containsPlus(string str)
{
	for (char c : str) {
		if (c == '|') {
			return 1;
		}
	}
	return 0;
}

void NFA::removeSimpleState(int a, int q, int b)
{
	//BUCLA A

	string strA = "", ceAmInitialA = "", ceSePuneA = "";
	if (areBucla(a) == 1 && drum(a, q) == 0) {
		if (getSimbol(a, a).size() > 1) {
			strA = "(" + getSimbol(a, a) + ")|";
		}
		else
			strA = getSimbol(a, a) + "|";
		ceAmInitialA = getSimbol(a, a);
		deleteTransition(a, getSimbol(a, a).c_str(), a);
	}

	//bucla efectiva

	if (amDrum(a,q)==1&&amDrum(q,a)==1) {                      //dus-intors ca in curs     adaug bucla la primul
		string dus="", intors="";

		if(getSimbol(a, q)!="-")
			dus = getSimbol(a, q);
		if(getSimbol(q,a)!="-")
			intors = getSimbol(q, a);

		//dus
		if (containsPlus(dus) == 1) {
			strA += "(" + dus + ")";
			ceSePuneA += "(" + dus + ")";
		}
		else {
			strA += dus;
			ceSePuneA += dus;
		}

		//bucla
		if (areBucla(q) == 1) {
			if (getSimbol(q, q).size() > 1) {
				strA += "(" + getSimbol(q, q) + ")*";
				ceSePuneA += "(" + getSimbol(q, q) + ")*";
			}
			else {
				strA += getSimbol(q, q) + "*";
				ceSePuneA += getSimbol(q, q) + "*";
			}
		}

		//intors
		if (containsPlus(intors) == 1) {
			strA += "(" + intors + ")";
			ceSePuneA += "(" + intors + ")";
		}
		else {
			strA += intors;
			ceSePuneA += intors;
		}

		if (ceSePuneA == ceAmInitialA) {
			if (ceAmInitialA.size()&&existTransition(a, ceAmInitialA.c_str(), a)==false)
				addTransition(a, ceAmInitialA.c_str(), a);
		}
		else
			if (strA.size()&&existTransition(a, strA.c_str(), a)==false)
				addTransition(a, strA.c_str(), a);
	}

	//BUCLA B

	string strB = "", ceAmInitialB = "", ceSePuneB = "";
	if (areBucla(b) == 1 && drum(q, b) == 0) {
		if (getSimbol(b, b).size() > 1) {
			strB = "(" + getSimbol(b, b) + ")|";
		}
		else
			strB = getSimbol(b, b) + "|";
		ceAmInitialB = getSimbol(b, b);
		deleteTransition(b, getSimbol(b, b).c_str(), b);
	}

	if (amDrum(q,b)==1&&amDrum(b,q)==1) {                      //dus-intors ca in curs     adaug bucla la al doilea
		string dus="", intors="";
		
		if(getSimbol(q, b)!="-")
			dus = getSimbol(q, b);
		if(getSimbol(b, q)!="-")
			intors = getSimbol(b, q);

		if (containsPlus(intors) == 1) {
			strB += "(" + intors + ")";
			ceSePuneB += "(" + intors + ")";
		}
		else {
			strB += intors;
			ceSePuneB += intors;
		}

		//bucla
		if (areBucla(q) == 1) {
			if (getSimbol(q, q).size() > 1) {
				strB += "(" + getSimbol(q, q) + ")*";
				ceSePuneB += "(" + getSimbol(q, q) + ")*";
			}
			else {
				strB += getSimbol(q, q) + "*";
				ceSePuneB += getSimbol(q, q) + "*";
			}
		}

		if (containsPlus(dus) == 1) {
			strB += "(" + dus + ")";
			ceSePuneB += "(" + dus + ")";
		}
		else {
			strB += dus;
			ceSePuneB += dus;
		}
		if (ceSePuneB == ceAmInitialB) {
			if (ceSePuneB.size()&&existTransition(b, ceSePuneB.c_str(), b)==false)
				addTransition(b, ceSePuneB.c_str(), b);
		}
		else
			if (strB.size()&&existTransition(b, strB.c_str(), b)==false)
				addTransition(b, strB.c_str(), b);
	}

	//CONECTARE A SI B
	string AlaB = "", BlaA = "";

	//A la B
	string AlaBinit = "", AlaBfinal = "";
	if (amDrum(a, b) == 1) {						//am drum inclusiv $
		if (getSimbol(a, b) != "-")								//daca arcul nu e $
		{
			if (amDrum(a, q) == 1 && amDrum(q, b) == 1) {           //inclusiv $
				if (getSimbol(a, b).size() > 1)
					AlaB = "(" + getSimbol(a, b) + ")|";
				else
					AlaB += getSimbol(a, b) + "|";
				AlaBinit = getSimbol(a, b);
				deleteTransition(a, getSimbol(a, b).c_str(), b);
			}
		}
		else													//daca arcul e $
		{
			if (amDrum(a, q) == 1 && amDrum(q, b) == 1) 
			{           
				AlaB = "$|";
				AlaBinit = "$";
				deleteTransition(a, "$", b);
			}
		}
	}
	//implementare efectiva (mai sus am verificat daca aveam ceva)

	if (amDrum(a, q) == 1 && amDrum(q, b) == 1) {		//am drum prin intermediar, inclusiv $

		if (getSimbol(a, q) != "-") {
			if (containsPlus(getSimbol(a, q)) == 1) {
				AlaB += "(" + getSimbol(a, q) + ")";
				AlaBfinal += "(" + getSimbol(a, q) + ")";
			}
			else {
				AlaB += getSimbol(a, q);
				AlaBfinal += getSimbol(a, q);
			}
		}

		//bucla de mijloc
		if (areBucla(q) == 1) {
			if (getSimbol(q, q).size() > 1) {
				AlaB += "(" + getSimbol(q, q) + ")*";
				AlaBfinal += "(" + getSimbol(q, q) + ")*";
			}
			else {
				AlaB += getSimbol(q, q) + "*";
				AlaBfinal += getSimbol(q, q) + "*";
			}
		}

		//final
		if (getSimbol(q, b) != "-") {
			if (containsPlus(getSimbol(q, b)) == 1) {
				AlaB += "(" + getSimbol(q, b) + ")";
				AlaBfinal += "(" + getSimbol(q, b) + ")";
			}
			else {
				AlaB += getSimbol(q, b);
				AlaBfinal += getSimbol(q, b);
			}
		}

		if (AlaBinit == AlaBfinal||((isRepeatedSubstring(AlaBinit,AlaBfinal)==1|| isRepeatedSubstring(AlaBfinal,AlaBinit) == 1) &&AlaBinit.size()!=0)) {
			if (AlaBinit.size())
			{
				addTransition(a, AlaBinit.c_str(), b);
			}
		}
		else {
			if (AlaB.size())
			{
				addTransition(a, AlaB.c_str(), b);
			}
		}
	}

	//B la A
	string BlaAinit = "", BlaAfinal = "";
	if (amDrum(b, a) == 1) {						//am drum inclusiv $
		if (getSimbol(b, a) != "-")								//daca arcul nu e $
		{
			if (amDrum(b, q) == 1 && amDrum(q, a) == 1) {           //inclusiv $
				if (getSimbol(b, a).size() > 1)
					BlaA = "(" + getSimbol(b, a) + ")|";
				else
					BlaA += getSimbol(b, a) + "|";
				BlaAinit = getSimbol(b, a);
				deleteTransition(b, getSimbol(b, a).c_str(), a);
			}
		}
		else													//daca arcul e $
		{
			if (amDrum(b, q) == 1 && amDrum(q, a) == 1)
			{
				BlaA = "$|";
				BlaAinit = "$";
				deleteTransition(b, "$", a);
			}
		}
	}


	if (amDrum(b, q) == 1 && amDrum(q, a) == 1) {
		if (getSimbol(b, q) != "-") {
			if (containsPlus(getSimbol(b, q)) == 1) {
				BlaA += "(" + getSimbol(b, q) + ")";
				BlaAfinal += "(" + getSimbol(b, q) + ")";
			}
			else {
				BlaA += getSimbol(b, q);
				BlaAfinal += getSimbol(b, q);
			}
		}

		//bucla de mijloc
		if (areBucla(q) == 1) {
			if (getSimbol(q, q).size() > 1) {
				BlaA += "(" + getSimbol(q, q) + ")*";
				BlaAfinal += "(" + getSimbol(q, q) + ")*";
			}
			else {
				BlaA += getSimbol(q, q) + "*";
				BlaAfinal += getSimbol(q, q) + "*";
			}
		}

		if (getSimbol(q, a) != "-") {
			if (containsPlus(getSimbol(q, a)) == 1) {
				BlaA += "(" + getSimbol(q, a) + ")";
				BlaAfinal += "(" + getSimbol(q, a) + ")";
			}
			else {
				BlaA += getSimbol(q, a);
				BlaAfinal += getSimbol(q, a);
			}
		}

		if (BlaAinit == BlaAfinal|| ((isRepeatedSubstring(BlaAinit, BlaAfinal) == 1|| isRepeatedSubstring(BlaAfinal, BlaAinit) == 1) && BlaAinit.size() != 0)) {
			if (BlaAinit.size())
			{
				addTransition(b, BlaAinit.c_str(), a);
			}
		}
		else {
			if (BlaA.size())
			{
				addTransition(b, BlaA.c_str(), a);
			}
		}
	}
}

void NFA::deleteState(int q)
{
	vector<pair<int, int>> deSters;
	for (auto const& [state, transitionsFromState] : transitions) {
		for (auto const& [symbol, nextStates] : transitionsFromState) {
			for (int nextState : nextStates) {
				if (state == q || nextState == q)
					deSters.push_back(make_pair(state, nextState));
			}
		}
	}
	for (auto i : deSters) {
		string simbol = getSimbol(i.first, i.second);
		if (simbol == "-")
			simbol = "$";
		deleteTransition(i.first, simbol.c_str(), i.second);
	}
}

set<int> NFA::getVecini(int q)
{
	set<int> vecini;
	for (auto const& [state, transitionsFromState] : transitions) {
		for (auto const& [symbol, nextStates] : transitionsFromState) {
			for (int nextState : nextStates) {
				if (nextState == q && state != q)
					vecini.insert(state);
				if (nextState != q && state == q)
					vecini.insert(nextState);
			}
		}
	}
	return vecini;
}
