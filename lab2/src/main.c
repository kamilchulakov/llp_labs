#include <stdio.h>
#include "ast.h"
#include "parser.tab.h"

int main() {
    printf("it works...\n");
    return yyparse();
}