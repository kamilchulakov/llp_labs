#include <stdio.h>
#include "ast.h"
#include "parser.tab.h"

int main() {
    printf("INPUT:\n");
    return yyparse();
}