%{
#include <stdio.h>
#include <string.h>

enum yytokentype {
    DB = 258,
    DOT = 259,
    CREATE_COLLECTION = 260,
    GET_COLLECTION = 261,
    DROP_DATABASE = 262,
    GET_NAME = 263,
    WORD = 264,
    COLON = 265,
    OTHER = 266
};
%}                                                                                                                                                                                                                          `````````````````````````````````````````````````

%option noyywrap

alpha                   [A-Za-z_0-9 \t\r]
int                     ([-+]?[0-9])+
blank                   [ \t\r]
word                    [a-zA-Z_][a-zA-Z0-9_]*
quoted_string           \"{word}*\"

%%
"db"                    {printf("DB"); return (DB);}
"."                     {printf("DOT"); return (DOT);}
"createCollection"      {printf("CREATE_COLLECTION"); return (CREATE_COLLECTION);}
"getCollection"         {printf("GET_COLLECTION"); return (GET_COLLECTION);}
"dropDatabase"          {printf("DROP_DATABASE"); return (DROP_DATABASE);}
"getName"               {printf("GET_NAME"); return (GET_NAME);}

{word}                  {printf("WORD"); return (WORD);}

":"                     {printf("COLON"); return (COLON);}
"."                     {printf("DOT"); return (DOT);}
.                       {printf("OTHER"); return (OTHER);}
%%