#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

// Structure to hold expression components
typedef struct {
    char result[10];
    char var1[10];
    char var2[10];
    char op;
    int is_parenthesized;
    char sub_var1[10];
    char sub_var2[10];
    char sub_op;
} Expression;

// Function to trim whitespace from a string
void trim(char *str) {
    int start = 0, end = strlen(str) - 1;
    while (isspace(str[start])) start++;
    while (end >= start && isspace(str[end])) end--;
    str[end + 1] = '\0';
    memmove(str, str + start, end - start + 2);
}

// Function to parse the input statement
int parseStatement(const char *input, Expression *expr) {
    char temp[100];
    strcpy(temp, input);
    trim(temp);

    // Initialize expression
    memset(expr, 0, sizeof(Expression));

    // Parse: result = expression
    char *eq = strchr(temp, '=');
    if (!eq) return 0;
    *eq = '\0';
    trim(temp);
    trim(eq + 1);
    strncpy(expr->result, temp, sizeof(expr->result) - 1);

    char *expr_str = eq + 1;

    // Check for parentheses
    if (expr_str[0] == '(') {
        expr->is_parenthesized = 1;
        char *end_paren = strrchr(expr_str, ')');
        if (!end_paren) return 0;
        *end_paren = '\0';
        expr_str++; // Skip '('
        trim(expr_str);

        // Parse sub-expression: var1 op var2
        char sub_var1[10], sub_var2[10], sub_op;
        if (sscanf(expr_str, "%s %c %s", sub_var1, &sub_op, sub_var2) != 3) return 0;
        if (sub_op != '+' && sub_op != '-') return 0;
        strncpy(expr->sub_var1, sub_var1, sizeof(expr->sub_var1) - 1);
        strncpy(expr->sub_var2, sub_var2, sizeof(expr->sub_var2) - 1);
        expr->sub_op = sub_op;

        // Check for outer operator and variable
        char *outer = end_paren + 1;
        trim(outer);
        if (*outer) {
            if (sscanf(outer, "%c %s", &expr->op, expr->var2) != 2) return 0;
            if (expr->op != '*' && expr->op != '/') return 0;
        } else {
            // No outer operation, treat sub-expression as main
            expr->is_parenthesized = 0;
            strncpy(expr->var1, sub_var1, sizeof(expr->var1) - 1);
            strncpy(expr->var2, sub_var2, sizeof(expr->var2) - 1);
            expr->op = sub_op;
        }
    } else {
        // Simple expression: var1 op var2
        if (sscanf(expr_str, "%s %c %s", expr->var1, &expr->op, expr->var2) != 3) return 0;
        if (expr->op != '+' && expr->op != '-' && expr->op != '*' && expr->op != '/') return 0;
    }

    return 1;
}

// Function to generate assembly code
void generateAssembly(const Expression *expr) {
    printf("\n;; Assembly code (x86) for %s = ", expr->result);
    if (expr->is_parenthesized) {
        printf("(%s %c %s) %c %s\n", expr->sub_var1, expr->sub_op, expr->sub_var2, expr->op, expr->var2);
    } else {
        printf("%s %c %s\n", expr->var1, expr->op, expr->var2);
    }
    printf("section .text\n");
    printf("global _start\n");
    printf("_start:\n");

    if (expr->is_parenthesized) {
        // Handle sub-expression: (sub_var1 sub_op sub_var2)
        printf("    mov eax, [%s]    ; Load %s into eax\n", expr->sub_var1, expr->sub_var1);
        if (expr->sub_op == '+') {
            printf("    add eax, [%s]    ; Add %s to eax\n", expr->sub_var2, expr->sub_var2);
        } else if (expr->sub_op == '-') {
            printf("    sub eax, [%s]    ; Subtract %s from eax\n", expr->sub_var2, expr->sub_var2);
        }

        // Handle outer operation: * or /
        if (expr->op == '*') {
            printf("    imul eax, [%s]   ; Multiply eax by %s\n", expr->var2, expr->var2);
        } else if (expr->op == '/') {
            printf("    mov edx, 0       ; Clear edx for division\n");
            printf("    mov ebx, [%s]    ; Load %s into ebx\n", expr->var2, expr->var2);
            printf("    div ebx          ; Divide eax by ebx\n");
        }
    } else {
        // Handle simple expression
        printf("    mov eax, [%s]    ; Load %s into eax\n", expr->var1, expr->var1);
        if (expr->op == '+') {
            printf("    add eax, [%s]    ; Add %s to eax\n", expr->var2, expr->var2);
        } else if (expr->op == '-') {
            printf("    sub eax, [%s]    ; Subtract %s from eax\n", expr->var2, expr->var2);
        } else if (expr->op == '*') {
            printf("    imul eax, [%s]   ; Multiply eax by %s\n", expr->var2, expr->var2);
        } else if (expr->op == '/') {
            printf("    mov edx, 0       ; Clear edx for division\n");
            printf("    mov ebx, [%s]    ; Load %s into ebx\n", expr->var2, expr->var2);
            printf("    div ebx          ; Divide eax by ebx\n");
        }
    }

    printf("    mov [%s], eax    ; Store result in %s\n", expr->result, expr->result);
    printf("    ;; Exit program\n");
    printf("    mov eax, 1       ; System call number for exit\n");
    printf("    mov ebx, 0       ; Exit status code 0\n");
    printf("    int 0x80         ; Call kernel\n");
    printf("\nsection .data\n");
    if (expr->is_parenthesized) {
        printf("    %s dd 10        ; Example value for %s\n", expr->sub_var1, expr->sub_var1);
        printf("    %s dd 20        ; Example value for %s\n", expr->sub_var2, expr->sub_var2);
        printf("    %s dd 5         ; Example value for %s\n", expr->var2, expr->var2);
    } else {
        printf("    %s dd 10        ; Example value for %s\n", expr->var1, expr->var1);
        printf("    %s dd 20        ; Example value for %s\n", expr->var2, expr->var2);
    }
    printf("    %s dd 0         ; Result variable %s\n", expr->result, expr->result);
}

int main() {
    char input[100];
    Expression expr;

    printf("Enter a statement (e.g., a = b + c, x = y * z, p = (q + r) * s): ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0'; // Remove newline

    if (!parseStatement(input, &expr)) {
        printf("Invalid input! Expected: <var> = <var> <+|-,*,/> <var> or <var> = (<var> <+|-,*,/> <var>) <*,/> <var>\n");
        return 1;
    }

    generateAssembly(&expr);
    return 0;
}