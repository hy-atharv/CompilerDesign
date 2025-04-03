#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <algorithm>
using namespace std;

// Inline function to check if a character is an operator
inline bool isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
}

// Inline function to get operator precedence
inline int precedence(char c) {
    if (c == '+' || c == '-') return 1;
    if (c == '*' || c == '/') return 2;
    if (c == '^') return 3;
    return 0;
}

// Inline function to check if operator is right-associative
inline bool isRightAssociative(char c) {
    return (c == '^');
}

// Convert infix to postfix (pass-by-reference)
void infixToPostfix(const string& infix, string& postfix) {
    stack<char> s;
    postfix.clear();
    
    for (char c : infix) {
        if (isalnum(c)) {
            postfix += c;
        } else if (c == '(') {
            s.push(c);
        } else if (c == ')') {
            while (!s.empty() && s.top() != '(') {
                postfix += s.top();
                s.pop();
            }
            if (!s.empty()) s.pop(); // Remove '('
        } else if (isOperator(c)) {
            while (!s.empty() && s.top() != '(') {
                if (isRightAssociative(c)) {
                    if (precedence(s.top()) > precedence(c)) {
                        postfix += s.top();
                        s.pop();
                    } else {
                        break;
                    }
                } else {
                    if (precedence(s.top()) >= precedence(c)) {
                        postfix += s.top();
                        s.pop();
                    } else {
                        break;
                    }
                }
            }
            s.push(c);
        }
    }
    
    while (!s.empty() && s.top() != '(') {
        postfix += s.top();
        s.pop();
    }
}

// Structure for Quadruple
struct Quadruple {
    char op;
    string arg1, arg2, result;
    Quadruple(char o, const string& a1, const string& a2, const string& r) 
        : op(o), arg1(a1), arg2(a2), result(r) {}
};

// Structure for Triple
struct Triple {
    char op;
    string arg1, arg2;
    Triple(char o, const string& a1, const string& a2) 
        : op(o), arg1(a1), arg2(a2) {}
};

// Generate Quadruples from postfix
void generateQuadruples(const string& postfix, vector<Quadruple>& quads) {
    vector<string> stack;
    int tempCount = 0;

    for (char c : postfix) {
        if (isOperator(c)) {
            string arg2 = stack.back(); stack.pop_back();
            string arg1 = stack.back(); stack.pop_back();
            string result = "t" + to_string(tempCount++);
            quads.emplace_back(c, arg1, arg2, result);
            stack.push_back(result);
        } else {
            stack.push_back(string(1, c));
        }
    }
}

// Generate Triples from postfix (single pass for both Triples and Indirect Triples)
void generateTriples(const string& postfix, vector<Triple>& trips) {
    vector<string> stack;
    int tempCount = 0;

    for (char c : postfix) {
        if (isOperator(c)) {
            string arg2 = stack.back(); stack.pop_back();
            string arg1 = stack.back(); stack.pop_back();
            trips.emplace_back(c, arg1, arg2);
            stack.push_back("(" + to_string(tempCount++) + ")");
        } else {
            stack.push_back(string(1, c));
        }
    }
}

// Generate Indirect Triples indices (reuses triples, assigns memory-like addresses)
void generateIndirectTriples(const vector<Triple>& trips, vector<int>& indirect) {
    indirect.clear();
    int memoryAddress = 30; // Start at 30
    for (size_t i = 0; i < trips.size(); ++i) {
        indirect.push_back(memoryAddress++);
    }
}

// Print Quadruples
void printQuadruples(const vector<Quadruple>& quads) {
    cout << "\nQuadruples:\n";
    cout << "Op\tArg1\tArg2\tResult\n";
    for (const auto& q : quads) {
        cout << q.op << "\t" << q.arg1 << "\t" << q.arg2 << "\t" << q.result << "\n";
    }
}

// Print Triples
void printTriples(const vector<Triple>& trips) {
    cout << "\nTriples:\n";
    cout << "Index\tOp\tArg1\tArg2\n";
    for (size_t i = 0; i < trips.size(); ++i) {
        cout << i << "\t" << trips[i].op << "\t" << trips[i].arg1 << "\t" << trips[i].arg2 << "\n";
    }
}

// Print Indirect Triples with memory-like indices
void printIndirectTriples(const vector<Triple>& trips, const vector<int>& indirect) {
    cout << "\nIndirect Triples:\n";
    cout << "Address\tOp\tArg1\tArg2\n";
    for (size_t i = 0; i < trips.size(); ++i) {
        cout << indirect[i] << "\t" << trips[i].op << "\t" << trips[i].arg1 << "\t" << trips[i].arg2 << "\n";
    }
}

int main() {
    string infix, postfix;
    cout << "Enter the expression): ";
    getline(cin, infix);
    
    // Remove spaces
    infix.erase(std::remove(infix.begin(), infix.end(), ' '), infix.end());
    
    // Convert to postfix
    infixToPostfix(infix, postfix);
    // Uncomment if you want to see postfix: cout << "Postfix (for reference): " << postfix << "\n";

    vector<Quadruple> quads;
    vector<Triple> trips;
    vector<int> indirect;

    // Generate all representations
    generateQuadruples(postfix, quads);
    generateTriples(postfix, trips);           // Generate triples once
    generateIndirectTriples(trips, indirect);  // Reuse triples, assign memory addresses

    // Print results
    printQuadruples(quads);
    printTriples(trips);
    printIndirectTriples(trips, indirect);

    return 0;
}
