%{
#include <iostream>
#include <string>
#include "ast.hh"
//#define YY_DECL extern "C" int yylex()

extern "C" int yylex(void);
void yyerror(const char *);

using namespace tiger;
using namespace std;
%}

%union {
    double              d;
    char*               str; /* we can't use a std::string, ask Eitan */

    /* all these will need to be declared in ast.hh */
    /* depending on our implementation, we might need all these,
       certain types might be described using the same union
       member. we can edit that as we go. */
    tiger::ASTNode::ASTptr     ast;
    tiger::ASTNode::value_t    val;
}

%token<d> NUMBER
%token<s> STR
%token<s> NAME

%token ARRAY IF THEN ELSE WHILE FOR TO DO LET IN END OF BREAK NIL FUNCTION VAR TYPE IMPORT PRIMITIVE CLASS EXTENDS METHOD NEW ASSIGN POINTIES LESS_EQUAL GREATER_EQUAL

%precedence DO
%precedence OF
%precedence THEN
%precedence ELSE

%right ASSIGN
%left '|'
%left '&'

%nonassoc '=' '<' LESS_EQUAL '>' GREATER_EQUAL POINTIES

%nonassoc '['
%left '+' '-'
%left '*' '/'

%type <ast> program
%type <ast> expr
%type <ast> exprseq_opt
%type <ast> exprlist_opt
%type <ast> fieldlist_opt
%type <ast> exprseq
%type <ast> fieldlist
%type <ast> lvalue
%type <ast> declaration
%type <ast> decllist
%type <ast> typedecl
%type <ast> type
%type <ast> typefields_opt
%type <ast> typefield
%type <ast> vardecl
%type <ast> funcdecl


%%

program: expr {
  }

expr: STR {
  } | NUMBER {
  } | NIL {
  } | lvalue {

  } | '-' expr {
        $$ = new UnaryMinusASTNode("-", $2);
  } | expr '+' expr {
        $$ = new PlusASTNode("+", $1, $3);
  } | expr '-' expr {
        $$ = new MinusASTNode("-", $1, $3);
  } | expr '*' expr {
        $$ = new MultASTNode("*", $1, $3);
  } | expr '/' expr {
        $$ = new DivASTNode("/", $1, $3);
  } | expr '=' expr {
        $$ = new EqualASTNode("=", $1, $3);
  } | expr POINTIES expr {
        $$ = new NotEqualASTNode("<>", $1, $3);
  } | expr '<' expr {
        $$ = new LessASTNode("<", $1, $3);
  } | expr '>' expr {
        $$ = new GreaterASTNode(">", $1, $3);
  } | expr LESS_EQUAL expr {
        $$ = new LessEqualASTNode("<=", $1, $3);
  } | expr GREATER_EQUAL expr {
        $$ = new GreaterEqualASTNode(">=", $1, $3);
  } | expr '&' expr {
        $$ = new LogicalAndASTNode("&", $1, $3);
  } | expr '|' expr {
        $$ = new LogicalOrASTNode("|", $1, $3);
  } | lvalue ASSIGN expr {
  } | NAME '(' exprlist_opt ')' {
  } | '(' exprseq_opt ')' {
  } | NAME '{' fieldlist_opt '}' {
  } | NAME '[' expr ']' OF expr {     /* array */
  } | IF expr THEN expr {
  } | IF expr THEN expr ELSE expr {
  } | WHILE expr DO expr {
  } | FOR NAME ASSIGN expr TO expr DO expr {
  } | BREAK {
  } | LET decllist IN exprseq_opt END {
  }

/*binop:
      '+'             { $$ = '+' }
    | '-'             { $$ = '-' }
    | '*'             { $$ = '*' }
    | '/'             { $$ = '/' }
    | '='             { $$ = '=' }
    | POINTIES        { $$ = POINTIES }
    | '<'             { $$ = '<' }
    | '>'             { $$ = '>' }
    | LESS_EQUAL      { $$ = LESS_EQUAL }
    | GREATER_EQUAL   { $$ = GREATER_EQUAL }
    | '&'             { $$ = '&' }
    | '|'             { $$ = '|' } */

exprlist_opt: /* nothing */ {
  } | exprlist {
  }

exprseq_opt: /* nothing */ {
  } | exprseq {
  }

fieldlist_opt: /* nothing */ {
  } | fieldlist {
  }

exprseq: expr {
  } | exprseq ';' expr {
  }

exprlist: expr {
  } | exprlist ',' expr {
  }

fieldlist: NAME '=' expr {
  } | fieldlist ',' NAME '=' expr {
  }

lvalue: NAME {
  } | lvalue_not_id {
  }

lvalue_not_id: lvalue '.' NAME {
  } | NAME '[' expr ']' {
  } | lvalue_not_id '[' expr ']' {
  }

decllist: declaration {
  } | decllist declaration {
  }

declaration: typedecl {
  } | vardecl {
  } | funcdecl {
  }

typedecl: TYPE NAME '=' type {
  }

type: NAME {
  } | '{' typefields_opt '}' {
  } ARRAY OF NAME {
  }

typefields_opt: /* nothing */ {
  } | typefields {
  }

typefields: typefield {
  } | typefields ',' typefield {
  }

typefield: NAME ':' NAME {
  }

vardecl: VAR NAME ASSIGN expr {
  } | VAR NAME ':' NAME ASSIGN expr {
  }

funcdecl: FUNCTION NAME '(' typefields_opt ')' '=' expr {
  } | FUNCTION NAME '(' typefields_opt ')' ':' NAME '=' expr {
  }

%%

void yyerror(const char *error) {
    cout << error << endl;
}
