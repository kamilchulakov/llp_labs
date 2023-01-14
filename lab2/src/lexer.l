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
field_reference         \"({word}\.)*{word}\"

%%

"db"                    {return (DB);}
"."                     {return (DOT);}
"createCollection"      {return (CREATE_COLLECTION);}
"getCollection"         {return (GET_COLLECTION);}
"dropDatabase"          {return (DROP_DATABASE);}

"count"                 {return (COUNT);}
"find"                  {return (FIND);}
"findOne"               {return (FIND_ONE);}
"insertOne"             {return (INSERT_ONE);}
"insertMany"            {return (INSERT_MANY);}
"remove"                {return (REMOVE);}
"renameCollection"      {return (RENAME_COLLECTION);}
"update"                {return (UPDATE);}
"updateOne"             {return (UPDATE_ONE);}
"limit"                 {return (LIMIT);}

"$or"                   {yylval.criteria_op = 0; return (OR);}
"$and"                  {yylval.criteria_op = 1; return (AND);}
"$eq"                   {yylval.cmp_op = 0; return (EQ);}
"$ne"                   {yylval.cmp_op = 1; return (NEQ);}
"$ge"                   {yylval.cmp_op = 2; return (GT);}
"$gte"                  {yylval.cmp_op = 3; return (GTE);}
"$lt"                   {yylval.cmp_op = 4; return (LT);}
"$lte"                  {yylval.cmp_op = 5; return (LTE);}
"$regex"                {yylval.cmp_op = 6; return (REGEX);}

"int32"                 {return (INT32);}
"bool"                  {return (BOOL);}
"double"                {return (DOUBLE);}

{int}                   {yylval.intval = atoi(yytext); return (INT32_VAL); }
{word}                  {sscanf(yytext, "%s", yylval.str); return (WORD);}
{quoted_word}           {sscanf(yytext, "%s", yylval.str); return (QUOTED_WORD);}
{field_reference}       {sscanf(yytext, "%s", yylval.str); return (QUOTED_WORD);}

":"                     {return (COLON);}
";"                     {return (SEMICOLON);}
"("                     {return (LPAREN);}
")"                     {return (RPAREN);}
","                     {return (COMMA);}
"{"                     {return (LBRACKET);}
"}"                     {return (RBRACKET);}
"["                     {return (LSBRACKET);}
"]"                     {return (RSBRACKET);}

{blank}                 { /* ignoring blank lines */ }

.                       {return (OTHER);}
%%