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
%token COUNT FIND FIND_ONE INSERT_ONE INSERT_MANY REMOVE REMOVE_ONE RENAME_COLLECTION UPDATE UPDATE_ONE
%token LPAREN RPAREN COMMA LBRACKET RBRACKET COLON SEMICOLON LSBRACKET RSBRACKET
%token EQ NEQ GT GTE LT LTE REGEX
%token OR AND
%token LIMIT
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
%type <intval> limit
%type <cmp_op> EQ NEQ GT GTE LT LTE REGEX
%type <criteria_op> OR AND
%type <criteria_op> criteria_operator
%type <cmp_op> cmp_operator
%type <val> value
%type <str> field
%type <element> field_value
%type <element> field_values
%type <doc> document
%type <doc> documents

%type <criteria> query_criterias_in_brackets
%type <criteria> query_criteria_in_brackets
%type <criteria> query_criteria_arg
%type <criteria> query_criterias
%type <criteria> query_criteria
%type <criteria> field_query_criteria

%type <col_query> count_call
%type <col_query> find_call
%type <col_query> remove_call
%type <col_query> insert_call
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
    document* doc;
    field_value* element;
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
    count_call | find_call | remove_call | insert_call
;

count_call:
    COUNT query_criteria_arg {
        $$ = create_count_query($2);
    }
;

find_call:
    FIND query_criteria_arg {
        $$ = create_find_query($2, -1);
    }
    | FIND_ONE query_criteria_arg {
        $$ = create_find_query($2, 1);
    }
    | FIND query_criteria_arg limit {
        $$ = create_find_query($2, $3);
    }
;

remove_call:
    REMOVE query_criteria_arg {
        $$ = create_remove_query($2, -1);
    }
    | REMOVE_ONE query_criteria_arg {
        $$ = create_remove_query($2, 1);
    }
    | REMOVE query_criteria_arg limit {
        $$ = create_remove_query($2, $3);
    }
;

limit:
    DOT LIMIT LPAREN INT32_VAL RPAREN {
        $$ = $4;
    }
;

insert_call:
    INSERT_ONE LPAREN document RPAREN {
        $$ = create_insert_query($3);
    }
    | INSERT_MANY LPAREN documents RPAREN {
        $$ = create_insert_query($3);
    }
;

documents:
    document | document COMMA documents {
        $$ = $1;
        $1->nxt = $3;
    }
;

document:
    LBRACKET field_values RBRACKET {
        $$ = create_document($2);
    }
;

field_values:
    field_value | field_value COMMA field_values {
        $$ = $1;
        $1->nxt = $3;
    }
;

field_value:
    field COLON value {
        $$ = create_field_value($1, $3);
    }
;

query_criteria_arg:
    LPAREN query_criteria_in_brackets RPAREN {
        $$ = $2;
    }
;

query_criterias_in_brackets:
    query_criteria_in_brackets | query_criteria_in_brackets COMMA query_criterias_in_brackets {
        $$ = $1;
        $1->nxt = $3;
    }
;

query_criteria_in_brackets:
    LBRACKET query_criterias RBRACKET {
        $$ = $2;
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
    criteria_operator COLON LSBRACKET query_criterias_in_brackets RSBRACKET {
        $$ = create_criteria_operator($1, $4);
    }
    | field_query_criteria
;

field_query_criteria:
    field COLON LBRACKET cmp_operator COLON value RBRACKET {
        $$ = create_field_criteria($1, $4, $6);
    }
    | field COLON value {
        $$ = create_field_criteria($1, 0, $3);
    }
;

field:
    WORD | QUOTED_WORD
;

cmp_operator:
    EQ | NEQ | GT | GTE | LT | LTE | REGEX
;

criteria_operator:
    OR | AND
;

value:
    INT32_VAL {
        $$ = create_int32_value($1);
    }
    | QUOTED_WORD {
        $$ = create_str_value($1);
    }
;
%%

void yyerror(const char* s) {
    fprintf(stderr, "%s\n", s);
}