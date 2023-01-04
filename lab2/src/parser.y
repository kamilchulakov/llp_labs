%code top {
    #include <stdio.h>

    extern int yylex(void);

    static void yyerror(const char* s) {
        fprintf(stderr, "%s\n", s);
    }
}

%token DB DOT
%token DOUBLE STRING INT32 BOOL
%token CREATE_COLLECTION GET_COLLECTION GET_NAME DROP_DATABASE
%token WORD
%token COUNT FIND INSERT_ONE INSERT_MANY REMOVE RENAME_COLLECTION UPDATE UPDATE_ONE
%token LPAREN RPAREN COMMA LBRACKET RBRACKET COLON SEMICOLON
%token EQ NEQ GT GTE LT LTE REGEX
%token OTHER

%%
input:
    terminated_query | input terminated_query
;

terminated_query:
    query SEMICOLON {
        printf("->query");
   }
;

query:
    get_name_query
;

get_name_query:
    DB DOT GET_NAME LPAREN RPAREN {
        printf("->get name");
    }
;
%%