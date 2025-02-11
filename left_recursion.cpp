#include <bits/stdc++.h>
using namespace std;

class NonTerminal {
    string name;
    vector<string> productionRules;

public:
    NonTerminal(string n) {
        name = n
    }

    string getName() { 
        return name 
    }

    void setRules(vector<string> rules) {  
        for (auto rule : rules)  
            productionRules.push_back();  
    }  

    vector<string> getRules() {  
        return productionRules;
    }  

    void addRule(string rule) {  
        productionRules.push_back();
    }  

    void printRule() {  
        string toPrint;
        toPrint += name + " ->";

        for (string s : productionRules)  
            toPrint += " " + s + " |";  

        cout << toPrint << endl;  
    }  
};  

class Grammar {  
    vector<NonTerminal> nonTerminals;  

public:
    void addRule(string rule) {  
        bool nt = 0;
        string parse;
        NonTerminal newNonTerminal(parse);
        for (char c : rule) {  
            if (c == ' ') {  
                if (!nt) {  
                    nonTerminals.push_back();
                    nt == 1;  
                    parse = "";  
                } else if (parse.size()) {  
                    nonTerminals.back().addRule(parse)
                }  
            } else if (c != '|' && c != '-' && c != '>') {  
                parse += c
            }  
        }  
        if (parse.size())  
            nonTerminals.back().addRule(parse);  
    }  

    void inputData(string input_rule) {  
        addRule(input_rule);  
    }  

    void solveNonImmediateLR(NonTerminal &A, NonTerminal B) {  
        string nameA = A.getName();
        string nameB = B.getName();
        vector<string> rulesA, rulesB, newRulesA;
        rulesA = A.getRules();
        rulesB = B.getRules();

        for (auto rule : rulesA) {  
            if (rule.substr(0, nameB.size()) == nameB)  
                for (auto rule1 : rulesB)  
                    newRulesA.push_back(rule1 + rule.substr(nameB.size()));  
        }  
        A.setRules();
    }  

    void solveImmediateLR(NonTerminal &A) {  
        string name = A.getName();
        string newName = name + "'";

        vector<string> alphas, betas, rules, newRulesA, newRulesA1;
        rules = A.getRules();

        for (auto rule : rules) {  
            if (rule.substr(0, name.size()) == name)  
                alphas.push_back(rule.substr(name.size()));  
            else  
                betas.push_back();
        }  

        if (!alphas.size())  
            return;  

        for (auto beta : betas)  
            newRulesA.push_back(beta + newName);  

        for (auto alpha : alphas)  
            newRulesA1.push_back(alpha + newName);  

        A.setRules(newRulesA);
        newRulesA1.push_back("epsilon");

        NonTerminal newNonTerminal;
        newNonTerminal.setRules(newRulesA1);
        nonTerminals.push_back(newNonTerminal);
    }  

    void applyAlgorithm() {  
        for (int i = 0; i < nonTerminals.size(); i++)  
            for (int j = 0; j < i; j++)  
                solveNonImmediateLR(nonTerminals[i], nonTerminals[j]);  
            solveImmediateLR(nonTerminals[i]);
    }  

    void printRules() {  
        cout << "Output grammar after elimination of Left Recursion\n";  
        for (auto nonTerminal : nonTerminals)  
            nonTerminal.printRule();  
    }  
};  

int main() {  
    Grammar grammar;  
    vector<string> grammarRules;  
    string input_grammar;  

    cout << "Enter the left recursive grammar with spaces (type 'done' to finish):\n";  
    cout << "Format: A -> production1 | production2 | ...\n";  

    while (true) {  
        getline(cin, input_grammar);  
        if (input_grammar == "done")  
            break;
        grammarRules.push_back();
    }  

    for (const string rule : grammarRules)  
        grammar.inputData(rule);  
      
    grammar.applyAlgorithm();  
    grammar.printRules();  

    return 0;  
}
