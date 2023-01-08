%{
    #include <stdio.h>
    #include "ast.h"
    extern int yylex(void);
    #define YYERROR_VERBOSE 1
    void yyerror(const char* s);
%}

%define parse.error verbose

%token DB DOT
%token DOUBLE STRING INT32 BOOL
%token INT32_VAL
%token CREATE_COLLECTION GET_COLLECTION GET_NAME DROP_DATABASE
%token WORD QUOTED_WORD
%token COUNT FIND INSERT_ONE INSERT_MANY REMOVE RENAME_COLLECTION UPDATE UPDATE_ONE
%token LPAREN RPAREN COMMA LBRACKET RBRACKET COLON SEMICOLON
%token EQ NEQ GT GTE LT LTE REGEX
%token OTHER

%type <str> WORD
%type <str> QUOTED_WORD
%type <sch> sch_field
%type <sch> sch_fields
%type <sch> schema
%type <dbque> db_query
%type <dbque> create_collection_call

%union {
    char str[42];
    schema_field* sch;
    db_query* dbque;
}

%%
input:
    | input db_query SEMICOLON {
        print_db_query($2);
    }
;

db_query:
    DB DOT create_collection_call {
        $$ = $3;
    }
;

schema:
    LBRACKET sch_fields RBRACKET {
        $$ = $2;
    }
;

sch_fields:
    sch_field | sch_field COMMA sch_fields {
        $$ = $1;
        $1->nxt = $3;
    }
;

sch_field:
    WORD COLON QUOTED_WORD {
        $$ = create_schema_field($1, $3);
    }
;

create_collection_call:
    CREATE_COLLECTION LPAREN QUOTED_WORD COMMA schema RPAREN {
        $$ = create_create_collection_query($3, $5);
    }
;
%%

void yyerror(const char* s) {
    fprintf(stderr, "%s\n", s);
}