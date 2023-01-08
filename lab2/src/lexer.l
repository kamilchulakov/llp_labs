%option noyywrap

%{
    #include <stdio.h>
    #include <string.h>
    #include "ast.h"
    #include "parser.tab.h"
%}

int                     ([-+]?[0-9])+
blank                   [ \t\r\n\f\v]+
word                    [a-zA-Z_][a-zA-Z0-9_]*
quoted_word             \"{word}\"

%%

"db"                    {return (DB);}
"."                     {return (DOT);}
"createCollection"      {return (CREATE_COLLECTION);}
"getCollection"         {return (GET_COLLECTION);}
"dropDatabase"          {return (DROP_DATABASE);}
"getName"               {return (GET_NAME);}

"count"                 {return (COUNT);}
"find"                  {return (FIND);}
"insertOne"             {return (INSERT_ONE);}
"insertMany"            {return (INSERT_MANY);}
"remove"                {return (REMOVE);}
"renameCollection"      {return (RENAME_COLLECTION);}
"update"                {return (UPDATE);}
"updateOne"             {return (UPDATE_ONE);}

"$eq"                   {return (EQ);}
"$ne"                   {return (NEQ);}
"$ge"                   {return (GT);}
"$gte"                  {return (GTE);}
"$lt"                   {return (LT);}
"$lte"                  {return (LTE);}
"$regex"                {return (REGEX);}

"int32"                 {return (INT32);}
"bool"                  {return (BOOL);}
"double"                {return (DOUBLE);}

{word}                  {sscanf(yytext, "%s", yylval.str); return (WORD);}
{quoted_word}           {sscanf(yytext, "%s", yylval.str); return (QUOTED_WORD);}

":"                     {return (COLON);}
";"                     {return (SEMICOLON);}
"("                     {return (LPAREN);}
")"                     {return (RPAREN);}
","                     {return (COMMA);}
"{"                     {return (LBRACKET);}
"}"                     {return (RBRACKET);}

{blank}                 { /* ignoring blank lines */ }

.                       {return (OTHER);}
%%