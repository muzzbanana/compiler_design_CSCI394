%{
    #include <iostream>
    #include <string>
%}

%union {
    double d;
    string s;
}

%token<d> NUMBER
%token<s> STR
%token<s> NAME

%token ARRAY IF THEN ELSE WHILE FOR TO DO LET IN END OF BREAK NIL FUNCTION VAR TYPE IMPORT PRIMITIVE CLASS EXTENDS METHOD NEW ASSIGN POINTIES LESS_EQUAL GREATER_EQUAL

%%

program: expr {
  }

expr: STR {
  } | NUMBER {
  } | NIL {
  } | lvalue {
  } | '-' expr {
  } | expr binop expr {
  } | lvalue ASSIGN expr {
  } | NAME '(' exprlist_opt ')' {
  } | '(' exprseq_opt ')' {
  } | NAME '{' fieldlist_opt '}' {
  } | NAME '[' expr ']' OF expr {
  } | IF expr THEN expr {
  } | IF expr THEN expr ELSE expr {
  } | WHILE expr DO expr {
  } | FOR NAME ASSIGN expr TO expr DO expr {
  } | BREAK {
  } | LET decllist IN exprseq_opt END {
  }

binop: '+' {
  } | '-' {
  } | '*' {
  } | '/' {
  } | '=' {
  } | POINTIES {
  } | '<' {
  } | '>' {
  } | LESS_EQUAL {
  } | GREATER_EQUAL {
  } | '&' {
  } | '|' {
  }

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
  } | lvalue '.' NAME {
  } | lvalue '[' expr ']' {
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
