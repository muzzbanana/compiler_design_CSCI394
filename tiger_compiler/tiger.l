%{
#include <iostream>
#include "tokens.hh"
//#include "snazzle.tab.h"
using namespace std;
#define YY_DECL extern "C" int yylex()
std::string parse_string(const char *text);
int comment_depth = 0;
int current_line = 1;
int comment_start_line = -1;
/*
enum _Tokens {ARRAY = 257, IF, THEN, ELSE, WHILE, FOR, TO, DO, LET, IN, END,
              OF, BREAK, NIL, FUNCTION, VAR, TYPE, IMPORT, PRIMITIVE, CLASS,
              EXTENDS, METHOD, NEW, COMMA, COLON, SEMICOLON, OPEN_PAREN, CLOSE_PAREN,
              OPEN_BRACKET, CLOSE_BRACKET, OPEN_CURLY, CLOSE_CURLY, STOP, PLUS, MINUS,
              MULT, DIV, EQUAL, POINTIES, LESS, LESS_EQUAL, GREATER, GREATER_EQUAL, AND,
              OR, ASSIGN, END_OF_LINE, COMMENT, STRING, STRING_LITERAL, INTEGER,
              INTEGER_LITERAL, SPACE};
              */
%}
%option noyywrap
%x COMMENT
%%
[ \t]+         { return SPACE; }
array          { return ARRAY; }
int            { return INTEGER; }
string         { return STRING; }
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
:              { return COLON; }
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
:=             { return ASSIGN;}
\r\n|\n\r|\n|\r { current_line++; return END_OF_LINE; }
\"([^"]|\\.)*\" {
    /* a string literal is a quote, followed by 0 or more repetitions
       of things that are either (a) not a quote, or (b) \ followed by
       any character, and then a closing quote.
       Note: not all backslash-character sequences are valid, but we
       will deal with this somewhere else. */
    try {
        std::string result = parse_string(yytext);
    } catch (std::runtime_error e) {
        return ERROR_ESCAPE;
    }
    return STRING_LITERAL;
}
[0-9]+ {
    return INTEGER_LITERAL;
}
[A-Za-z][A-Za-z0-9_]*|_main {
    return IDENTIFIER;
}
\/\*          { BEGIN(COMMENT); comment_depth = 1; comment_start_line = current_line; }
\*\/          { return ERROR_COMMENT; }
<COMMENT>\/\* { comment_depth ++; }
<COMMENT>\*\/ {
    comment_depth --;
    if (comment_depth == 0) {
        BEGIN(INITIAL);
    }
}
<COMMENT><<EOF>> {
    fprintf(stderr, "error: unterminated comment starting at line %d\n", comment_start_line);
    /* switch back into initial mode here, so we still return
       the end-of-file token (0) after the ERROR_COMMENT. If
       we don't do this, it gets stuck on the EOF character and
       keeps returning ERROR_COMMENT over and over. */
    BEGIN(INITIAL);
    /* also, reset the line number to 1 at the end of the file,
       otherwise resetting yyin makes the line number go up
       really high */
    return ERROR_COMMENT;
}
<<EOF>> {
    current_line = 1;
    return 0;
}
<COMMENT>.     ;
<COMMENT>\n    ;
.              ;
%%
std::string parse_string(const char *text) {
    /* Parse a string literal into a std::string, replacing escape
       characters by their proper representations. Throws an exception
       if an invalid character is found. */
    bool escape_mode = false;
    std::string result = "";
    for (const char *it = text; *it; it++) {
        const char ch = *it;
        if (!escape_mode) {
            if (ch == '\\') {
                escape_mode = true;
            } else {
                // Just put the characters into the string
                result += ch;
            }
        } else {
            switch(ch) {
                case 'a':
                    result += '\a';
                    break;
                case 'b':
                    result += '\b';
                    break;
                case 'f':
                    result += '\f';
                    break;
                case 'n':
                    result += '\n';
                    break;
                case 'r':
                    result += '\r';
                    break;
                case 't':
                    result += '\t';
                    break;
                case 'v':
                    result += '\v';
                    break;
                case '"':
                    result += '"';
                    break;
                case '\\':
                    result += '\\';
                    break;
                default:
                    throw std::runtime_error("error: unknown escape character");
            }
            escape_mode = false;
        }
    }
    return result;
}
