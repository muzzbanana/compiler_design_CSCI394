%{
#include <iostream>
#include <sstream>
#include "ast.hh"
#include "tiger.tab.h"

using namespace std;
using namespace tiger;

#define YY_DECL extern "C" int yylex()
int yycolumn = 1;
void set_yycolumn(int val) {
    yycolumn = val;
    yylloc.first_column = yycolumn;
    yylloc.last_column = yycolumn + yyleng - 1;
}

#define YY_USER_ACTION { \
    yylloc.first_line = yylloc.last_line = yylineno; \
    set_yycolumn(yycolumn); \
    yycolumn += yyleng; \
}

extern void yyerror(ASTNode::ASTptr *, const char *);
extern int yylineno;
std::string parse_string(const char *text);

int comment_depth = 0;
int current_line = 1;
int comment_start_line = -1;
%}

%option noyywrap nodefault yylineno nounput

%x COMMENT

%%
[ \t]+         ;
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

,   |
:   |
;   |
\(  |
\)  |
\[  |
\]  |
\{  |
\}  |
\.  |
\+  |
\-  |
\*  |
\/  |
\=  |
\<  |
\>  |
\&  |
\|             { return yytext[0]; }

\<>            { return POINTIES; }
\<=            { return LESS_EQUAL; }
\>=            { return GREATER_EQUAL; }
\:=            { return ASSIGN;}

\r\n|\n\r|\n|\r { current_line++; }
\"([^"\n\r]|\\.)*\" {
    /* a string literal is a quote, followed by 0 or more repetitions
       of things that are either (a) not a quote, or (b) \ followed by
       any character, and then a closing quote.
       Note: not all backslash-character sequences are valid, but we
       will deal with this somewhere else. */
    try {
        std::string result = parse_string(yytext);
        yylval.str = strdup(result.c_str());
    } catch (std::runtime_error e) {
        yyerror(NULL, "error use of illegal escape character in string at line");
    }
    return STR;
}
\"([^"\n\r]|\\.)* {
    /* if we found this, it can't have matched the previous rule --
       so it must be an error resulting from an unterminated string
       literal */
    yyerror(NULL, "error: unterminated string literal starting at line");
}
[0-9]+ {
    yylval.d = atof(yytext);
    return NUMBER;
}
[A-Za-z][A-Za-z0-9_]*|_main {
    yylval.str = strdup(yytext);
    return NAME;
}
\/\*          { BEGIN(COMMENT); comment_depth = 1; comment_start_line = current_line; }
\*\/          { yyerror(NULL, "error: invalid comment syntax at line"); }
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
    yyerror(NULL, "error: unterminated comment at line");
}
<<EOF>> {
    /* also, reset the line number to 1 at the end of the file,
       otherwise resetting yyin makes the line number go up
       really high */
    current_line = 1;
    comment_start_line = -1;
    return 0;
}
<COMMENT>.     ;
<COMMENT>\n    ;
.              { yyerror(NULL, "error: unknown character at line"); }

%%

std::string parse_string(const char *text) {
    /* Parse a string literal into a std::string, replacing escape
       characters by their proper representations. Throws an exception
       if an invalid character is found. */
    bool escape_mode = false;
    std::stringstream result;
    // use these +1's to skip opening and closing quotes
    for (const char *it = text + 1; *(it + 1); it++) {
        const char ch = *it;
        if (!escape_mode) {
            if (ch == '\\') {
                escape_mode = true;
            } else {
                // Just put the characters into the string
                result << ch;
            }
        } else {
            switch(ch) {
                case 'a':
                    result << '\a';
                    break;
                case 'b':
                    result << '\b';
                    break;
                case 'f':
                    result << '\f';
                    break;
                case 'n':
                    result << '\n';
                    break;
                case 'r':
                    result << '\r';
                    break;
                case 't':
                    result << '\t';
                    break;
                case 'v':
                    result << '\v';
                    break;
                case '"':
                    result << '"';
                    break;
                case '\\':
                    result << '\\';
                    break;
                default:
                    throw std::runtime_error("error: unknown escape character");
            }
            escape_mode = false;
        }
    }
    return result.str();
}
