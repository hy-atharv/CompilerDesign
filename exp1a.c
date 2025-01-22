#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX 50

// Stacks for operands and operators
int operandStack[MAX], operandTop = -1;
char operatorStack[MAX];
int operatorTop = -1;

void pushOperand(int val) {
    if (operandTop < MAX - 1) {
        operandStack[++operandTop] = val;
    } else {
        printf("Operand Stack Overflow\n");
    }
}

int popOperand() {
    if (operandTop >= 0) {
        return operandStack[operandTop--];
    } else {
        printf("Operand Stack Underflow\n");
        exit(1);
    }
}

void pushOperator(char op) {
    if (operatorTop < MAX - 1) {
        operatorStack[++operatorTop] = op;
    } else {
        printf("Operator Stack Overflow\n");
    }
}

char popOperator() {
    if (operatorTop >= 0) {
        return operatorStack[operatorTop--];
    } else {
        printf("Operator Stack Underflow\n");
        exit(1);
    }
}

char peekOperator() {
    if (operatorTop >= 0) {
        return operatorStack[operatorTop];
    } else {
        return '\0';
    }
}

int precedence(char op) {
    if (op == '+' || op == '-') {
        return 1;
    }
    if (op == '*' || op == '/') {
        return 2;
    }
    return 0;
}

int applyOperator(int a, int b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
    }
    return 0;
}

void evaluateTop() {
    int b = popOperand();
    int a = popOperand();
    char op = popOperator();
    pushOperand(applyOperator(a, b, op));
}

int evaluate(char* expr) {
    for (int i = 0; i < strlen(expr); i++) {
        if (isdigit(expr[i])) {
            // Push the operand to the operand stack
            pushOperand(expr[i] - '0');
        } else if (expr[i] == '(') {
            // Push '(' to the operator stack
            pushOperator(expr[i]);
        } else if (expr[i] == ')') {
            // Process everything until '('
            while (peekOperator() != '(') {
                evaluateTop();
            }
            popOperator(); // Pop '('
        } else if (expr[i] == '+' || expr[i] == '-' || expr[i] == '*' || expr[i] == '/') {
            // Process operators with higher or equal precedence
            while (operatorTop >= 0 && precedence(peekOperator()) >= precedence(expr[i])) {
                evaluateTop();
            }
            pushOperator(expr[i]);
        }
    }

    // Process remaining operators
    while (operatorTop >= 0) {
        evaluateTop();
    }

    return popOperand(); // Final result
}

int main() {
    char expr[MAX];
    printf("Enter infix expression: ");
    scanf("%s", expr);

    printf("Result: %d\n", evaluate(expr));
    return 0;
}
