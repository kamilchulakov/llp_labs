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
%token WORD QUOTED_WORD
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
    db_query
;

db_query:
    DB DOT db_func {
        printf("->db query");
    }
;

db_func:
   get_name | get_collection | drop_database | create_collection
;

no_args:
    LPAREN RPAREN {
        printf("no args");
    }
;

query_criteria_arg:
    LPAREN query_criteria RPAREN {
        printf("->query criteria");
    }
;

query_criteria:
    QUOTED_WORD {
        printf("\"word\"");
    }
;

schema:
    LBRACKET schema_fields RBRACKET
;

schema_fields:
    schema_field | schema_field COMMA schema_fields
;

schema_field:
    WORD COLON type {
        printf("->schema field");
    }
;

type:
    STRING | INT32 | DOUBLE | BOOL
;

get_name:
    GET_NAME no_args {
        printf("->get name");
    }
;

drop_database:
    DROP_DATABASE no_args {
        printf("->drop database");
    }
;

get_collection:
    GET_COLLECTION query_criteria_arg {
        printf("->get collection");
    }
;

create_collection:
    CREATE_COLLECTION LPAREN QUOTED_WORD COMMA schema RPAREN {
        printf("->create collection");
    }
;
%%