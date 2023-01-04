%option noyywrap

%top {
    #include <stdio.h>
    #include <string.h>
    #include "parser.h"
}

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

"count"                 {printf("COUNT"); return (COUNT);}
"find"                  {printf("FIND"); return (FIND);}
"insertOne"             {printf("INSERT_ONE"); return (INSERT_ONE);}
"insertMany"            {printf("INSERT_MANY"); return (INSERT_MANY);}
"remove"                {printf("REMOVE"); return (REMOVE);}
"renameCollection"      {printf("RENAME_COLLECTION"); return (RENAME_COLLECTION);}
"update"                {printf("UPDATE"); return (UPDATE);}
"updateOne"             {printf("UPDATE_ONE"); return (UPDATE_ONE);}

"$eq"                   {printf("EQ"); return (EQ);}
"$ne"                   {printf("NEQ"); return (NEQ);}
"$ge"                   {printf("GT"); return (GT);}
"$gte"                  {printf("GTE"); return (GTE);}
"$lt"                   {printf("LT"); return (LT);}
"$lte"                  {printf("LTE"); return (LTE);}
"$regex"                {printf("REGEX"); return (REGEX);}

{word}                  {printf("WORD"); return (WORD);}

":"                     {printf("COLON"); return (COLON);}
"("                     {printf("LPAREN"); return (LPAREN);}
")"                     {printf("RPAREN"); return (RPAREN);}
","                     {printf("COMMA"); return (COMMA);}
"{"                     {printf("LBRACKET"); return (LBRACKET);}
"}"                     {printf("RBRACKET"); return (RBRACKET);}
.                       {printf("OTHER: %s", yytext); return (OTHER);}

%%