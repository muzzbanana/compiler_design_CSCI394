%{
    #include <iostream>
    #include <string>
%}

%union {
    double              d;
    string              s;
    double              binop;

    /* all these will need to be declared in ast.hh */
    /* depending on our implementation, we might need all these,
       certain types might be described using the same union
       member. we can edit that as we go. */
    Program_t*          program;
    Expr_t*             expr;
    ExprseqOpt_t*       exprseq_opt;
    ExprlistOpt_t*      exprlist_opt;
    FieldlistOpt_t*     fieldlist_opt;
    Exprseq*            exprseq;
    Fieldlist_t*        fieldlist;
    Lvalue_t*           lvalue;
    Declaration_t*      declaration;
    Decllist_t*         decllist;
    Typedecl_t*         typedecl;
    Type_t*             type;
    TypefieldsOpt_t*    typefields_opt;
    Typefield_t*        typefield;
    Vardecl_t*          vardecl;
    Funcdecl_t*         fundecl;
}

%token<d> NUMBER
%token<s> STR
%token<s> NAME

%token ARRAY IF THEN ELSE WHILE FOR TO DO LET IN END OF BREAK NIL FUNCTION VAR TYPE IMPORT PRIMITIVE CLASS EXTENDS METHOD NEW ASSIGN POINTIES LESS_EQUAL GREATER_EQUAL

%nonassoc ASSIGN
%left '|'
%left '&'

%nonassoc '='
%left '+' '-'
%left '*' '/'

%type <program> program
%type <expr> expr
%type <binop> binop
%type <exprseq_opt> exprseq_opt
%type <exprlist_opt> exprlist_opt
%type <fieldlist_opt> fieldlist_opt
%type <exprseq> exprseq
%type <fieldlist> fieldlist
%type <lvalue> lvalue
%type <declaration> declaration
%type <decllist> decllist
%type <typedecl> typedecl
%type <type> type
%type <typefields_opt> typefields_opt
%type <typefield>
%type <vardecl> vardecl
%type <fundecl> funcdecl>


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
  } | NAME '[' expr ']' OF expr {     /* array */
  } | IF expr THEN expr {
  } | IF expr THEN expr ELSE expr {
  } | WHILE expr DO expr {
  } | FOR NAME ASSIGN expr TO expr DO expr {
  } | BREAK {
  } | LET decllist IN exprseq_opt END {
  }

binop: 
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
    | '|'             { $$ = '|' }

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
