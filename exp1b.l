%{
#include <stdio.h>
#include <string.h>

int word_count = 0, line_count = 0, space_count = 0, tab_count = 0, target_word_freq = 0;
char target_word[50];

void count_word(const char* word) {
    if (strcmp(word, target_word) == 0) {
        target_word_freq++;
    }
}
%}

%%

\n              { line_count++; }                      // Increment line count on new line
[ \t]+          { space_count += yyleng;               // Count spaces and tabs
                  tab_count += (yytext[0] == '\t' ? yyleng : 0); }
[a-zA-Z0-9]+    { word_count++; count_word(yytext); }  // Count words and check target word
.|\n            { /* Ignore other characters */ }

%%

int yywrap(){}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Usage: %s <filename> <target_word>\n", argv[0]);
        return 1;
    }

    FILE* file = fopen(argv[1], "r");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    strcpy(target_word, argv[2]); // Set the target word
    yyin = file;
    yylex(); // Process the file

    printf("Number of lines: %d\n", line_count);
    printf("Number of words: %d\n", word_count);
    printf("Number of spaces: %d\n", space_count - tab_count); // Exclude tab spaces
    printf("Number of tab spaces: %d\n", tab_count);
    printf("Frequency of '%s': %d\n", target_word, target_word_freq);

    fclose(file);
    return 0;
}
