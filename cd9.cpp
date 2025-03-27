#include <iostream>
#include <stack>
#include <string>
#include <algorithm>
using namespace std;

// Function to check if a character is an operator
bool isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
}

// Function to get operator precedence
int precedence(char c) {
    if (c == '+' || c == '-') return 1;
    if (c == '*' || c == '/') return 2; // Division and multiplication same level
    if (c == '^') return 3; // Higher precedence for ^
    return 0;
}

// Function to check if operator is right-associative
bool isRightAssociative(char c) {
    return (c == '^'); // Only ^ is right-associative
}

// Function to convert infix to postfix
string infixToPostfix(string infix) {
    stack<char> s;
    string postfix = "";
    
    for (char c : infix) {
        if (isalnum(c)) { // If operand, add to output
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
            // Handle associativity and precedence
            while (!s.empty() && s.top() != '(') {
                if (isRightAssociative(c)) {
                    // For right-associative (^), pop if precedence is higher
                    if (precedence(s.top()) > precedence(c)) {
                        postfix += s.top();
                        s.pop();
                    } else {
                        break;
                    }
                } else {
                    // For left-associative (+, -, *, /), pop if precedence >=
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
    
    return postfix;
}

// Function to convert infix to prefix
string infixToPrefix(string infix) {
    // Reverse the infix expression
    reverse(infix.begin(), infix.end());
    
    // Swap '(' and ')'
    for (char& c : infix) {
        if (c == '(') c = ')';
        else if (c == ')') c = '(';
    }
    
    // Get postfix of reversed expression
    string postfix = infixToPostfix(infix);
    
    // Reverse the postfix to get prefix
    reverse(postfix.begin(), postfix.end());
    
    return postfix;
}

int main() {
    string infix;
    cout << "Enter an infix expression (e.g., a+b*c, (a+b)/c^d): ";
    getline(cin, infix);
    
    // Remove spaces if any
    infix.erase(remove(infix.begin(), infix.end(), ' '), infix.end());
    
    string postfix = infixToPostfix(infix);
    string prefix = infixToPrefix(infix);
    
    cout << "Postfix: " << postfix << endl;
    cout << "Prefix: " << prefix << endl;
    
    return 0;
}