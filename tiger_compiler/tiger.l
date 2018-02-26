%{
#include <iostream>
#include "tokens.hh"
//#include "snazzle.tab.h"
using namespace std;
#define YY_DECL extern "C" int yylex()
int line_num = 1;
/*
enum _Tokens {ARRAY = 257, IF, THEN, ELSE, WHILE, FOR, TO, DO, LET, IN, END,
              OF, BREAK, NIL, FUNCTION, VAR, TYPE, IMPORT, PRIMITIVE, CLASS,
              EXTENDS, METHOD, NEW, COMMA, COLON, SEMICOLON, OPEN_PAREN, CLOSE_PAREN,
              OPEN_BRACKET, CLOSE_BRACKET, OPEN_CURLY, CLOSE_CURLY, STOP, PLUS, MINUS,
              MULT, DIV, EQUAL, POINTIES, LESS, LESS_EQUAL, GREATER, GREATER_EQUAL, AND,
              OR, ASSIGN, END_OF_LINE, COMMENT, STRING_LITERAL, INTEGER_LITERAL, SPACE};
              */
%}
%option noyywrap
%%
[ \t]          { return SPACE; }
array          { return ARRAY; }
if             { return IF; }
then           { return THEN; }
else           { return ELSE; }
while          { return WHILE; }
for            { return FOR; }
to             { return TO; }
do             { return DO; }
let            { return LET; }
in             { return IN; }
end            { return END; }
of             { return OF; }
break          { return BREAK; }
nil            { return NIL; }
function       { return FUNCTION; }
var            { return VAR; }
type           { return TYPE; }
import         { return IMPORT; }
primitive      { return PRIMITIVE; }
class          { return CLASS; }
extends        { return EXTENDS; }
method         { return METHOD; }
new            { return NEW; }
,              { return COMMA; }
;              { return SEMICOLON; }
\(             { return OPEN_PAREN; }
\)             { return CLOSE_PAREN; }
\[             { return OPEN_BRACKET; }
\]             { return CLOSE_BRACKET; }
\{             { return OPEN_CURLY; }
\}             { return CLOSE_CURLY; }
\.             { return STOP; }
\+             { return PLUS; }
-              { return MINUS; }
\*             { return MULT; }
\/             { return DIV; }
=              { return EQUAL; }
\<>            { return POINTIES; }
\<             { return LESS; }
\<=            { return LESS_EQUAL; }
\>             { return GREATER; }
\>=            { return GREATER_EQUAL; }
&              { return AND; }
\|             { return OR; }
:=             { return ASSIGN; }
\r\n|\n\r|\n|\r { return END_OF_LINE; }
.              ;
%%
