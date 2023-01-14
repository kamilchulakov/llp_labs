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
%token CREATE_COLLECTION GET_COLLECTION DROP_DATABASE
%token WORD QUOTED_WORD
%token COUNT FIND INSERT_ONE INSERT_MANY REMOVE RENAME_COLLECTION UPDATE UPDATE_ONE
%token LPAREN RPAREN COMMA LBRACKET RBRACKET COLON SEMICOLON LSBRACKET RSBRACKET
%token EQ NEQ GT GTE LT LTE REGEX
%token OR AND
%token OTHER

%type <str> WORD
%type <str> QUOTED_WORD
%type <str_criteria> str_arg
%type <sch> sch_field
%type <sch> sch_fields
%type <sch> schema
%type <dbque> db_query
%type <dbque> db_func
%type <dbque> create_collection_call
%type <dbque> get_collection_call
%type <dbque> drop_database_call

%type <intval> INT32_VAL
%type <cmp_op> EQ NEQ GT GTE LT LTE REGEX
%type <criteria_op> OR AND
%type <criteria_op> criteria_operator
%type <cmp_op> cmp_operator
%type <val> value
%type <str> field
%type <criteria> query_criteria_arg
%type <criteria> query_criterias
%type <criteria> query_criteria
%type <criteria> field_query_criteria
%type <col_query> count_call
%type <col_query> col_func
%type <col_query> col_query


%union {
    char str[42];
    int intval;
    int cmp_op;
    int criteria_op;
    str_query_criteria* str_criteria;
    query_criteria* criteria;
    schema_field* sch;
    value* val;
    collection_query* col_query;
    db_query* dbque;
}

%%
input:
    | input db_query SEMICOLON {
        print_db_query($2);
    }
    | input col_query SEMICOLON {
        print_col_query($2);
    }
;

db_query:
    DB DOT db_func {
        $$ = $3;
    }
;

db_func:
   get_collection_call | create_collection_call | drop_database_call
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

get_collection_call:
    GET_COLLECTION str_arg {
        $$ = create_get_collection_query($2);
    }
;

drop_database_call:
    DROP_DATABASE LPAREN RPAREN {
        $$ = create_drop_database_query();
    }
;

str_arg:
    LPAREN QUOTED_WORD RPAREN {
        $$ = create_str_query_criteria($2);
    }
;

col_query:
    DB DOT WORD DOT col_func {
        $$ = $5;
        $$->collection = $3;
    }
;

col_func:
    count_call
;

count_call:
    COUNT query_criteria_arg {
        $$ = create_count_query($2);
    }
;

query_criteria_arg:
    LPAREN LBRACKET query_criterias RBRACKET RPAREN {
        $$ = $3;
    }
;

query_criterias:
    query_criteria
    | query_criteria COMMA query_criterias {
        $$ = $1;
        $1->nxt = $3;
    }
;

query_criteria:
    criteria_operator COLON LSBRACKET query_criterias RSBRACKET {
        $$ = create_criteria_operator($1, $4);
    }
    | field_query_criteria

field_query_criteria:
    field cmp_operator value {
        $$ = create_field_criteria($1, $2, $3);
    }
    | field COLON value {
        $$ = create_field_criteria($1, 0, $3);
    }

field:
    WORD

cmp_operator:
    EQ | NEQ | GT | GTE | LT | LTE | REGEX

criteria_operator:
    OR | AND

value:
    INT32_VAL {
        $$ = create_int32_value($1);
    }
    | QUOTED_WORD {
        $$ = create_str_value($1);
    }
%%

void yyerror(const char* s) {
    fprintf(stderr, "%s\n", s);
}