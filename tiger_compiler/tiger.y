
%{
#include <iostream>
#include <string>
#include "ast.hh"
//#define YY_DECL extern "C" int yylex()

using namespace tiger;
using namespace std;

extern "C" int yylex(void);
extern "C" int yylineno;
void yyerror(ASTNode::ASTptr *out, const char *);
%}

%union {
    double              d;
    const char*         str; /* we can't use a std::string, ask Eitan */

    /* all these will need to be declared in ast.hh */
    /* depending on our implementation, we might need all these,
       certain types might be described using the same union
       member. we can edit that as we go. */
    tiger::ASTNode::ASTptr     ast;
    tiger::ASTNode::value_t    val;

    tiger::DeclListASTNode*    decllist_ast;
    tiger::DeclarationASTNode*  decl_ast;

    tiger::FieldListASTNode*   fieldlist_ast;

    tiger::ExprSeqASTNode*     exprseq_ast;

    tiger::TypeASTNode*        type_ast;

    tiger::RecordFieldASTNode* recfield_ast;
    tiger::RecordTypeASTNode*  rectype_ast;
}

%define parse.error verbose

%parse-param {tiger::ASTNode::ASTptr *out}

%token<d> NUMBER
%token<str> STR
%token<str> NAME

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
%type <exprseq_ast> exprseq_opt
%type <exprseq_ast> exprseq
%type <exprseq_ast> exprlist_opt
%type <exprseq_ast> exprlist
%type <ast> fieldlist_opt
%type <fieldlist_ast> fieldlist
%type <ast> lvalue
%type <ast> lvalue_not_id
%type <decl_ast> declaration
%type <decllist_ast> decllist
%type <ast> typedecl
%type <type_ast> type
%type <rectype_ast> typefields_opt
%type <rectype_ast> typefields
%type <recfield_ast> typefield
%type <ast> vardecl
%type <ast> funcdecl

%left '.'


%%

program: expr {
       *out = $$;
  }

expr: STR {
        $$ = new StrASTNode($1);
  } | NUMBER {
        $$ = new NumASTNode($1);
  } | NIL {
        $$ = new NilASTNode();
  } | lvalue {
        $$ = $1;
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
        $$ = new AssignASTNode(":=", $1, $3);
  } | NAME '(' exprlist_opt ')' {
  } | '(' exprseq_opt ')' {
  } | NAME '{' fieldlist_opt '}' {
        $$ = new TypeInstASTNode("", "{", "}", new NameASTNode($1), $3);
  } | NAME '[' expr ']' OF expr {     /* array */
        $$ = new ArrayASTNode("", " [", "] of ", new NameASTNode($1), $3, $6, false);
  } | IF expr THEN expr {
        $$ = new ConditionalASTNode("if", "then", "else", $2, $4, new NilASTNode());
  } | IF expr THEN expr ELSE expr {
        $$ = new ConditionalASTNode("if", "then", "else", $2, $4, $6);
  } | WHILE expr DO expr {
        $$ = new WhileLoopASTNode("while", "do", $2, $4);
  } | FOR NAME ASSIGN expr TO expr DO expr {
        $$ = new ForLoopASTNode("for", ":=", "to", "do", new NameASTNode($2), $4, $6, $8);
  } | BREAK {
  } | LET decllist IN exprseq_opt END {
        $$ = new LetASTNode("let", "in", "end", $2, $4);
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
        $$ = new ExprSeqASTNode(", ");
  } | exprlist {
        $$ = $1;
  }

exprseq_opt: /* nothing */ {
        $$ = new ExprSeqASTNode("; ");
  } | exprseq {
        $$ = $1;
  }

fieldlist_opt: /* nothing */ {
        $$ = new FieldListASTNode(", ");
  } | fieldlist {
        $$ = $1;
  }

exprseq: expr {
        std::cout << "in exprseq" << std::endl;
        $$ = new ExprSeqASTNode("; ");
        $$->add_node($1);
  } | exprseq ';' expr {
        $$ = $1;
        $$->add_node($3);
  }

exprlist: expr {
        std::cout << "in exprlist" << std::endl;
        $$ = new ExprSeqASTNode(", ");
        $$->add_node($1);
  } | exprlist ',' expr {
        $$ = $1;
        $$->add_node($3);
  }

fieldlist: NAME '=' expr {
        $$ = new FieldListASTNode(", ");
        $$->add_node(new FieldMemberASTNode("=", new NameASTNode($1), $3, false));
  } | fieldlist ',' NAME '=' expr {
        $$ = $1;
        $$->add_node(new FieldMemberASTNode("=", new NameASTNode($3), $5, false));
  }

lvalue: NAME {
      $$ = new NameASTNode($1);
  } | lvalue_not_id {
      $$ = $1;
  }

lvalue_not_id: lvalue '.' NAME {
      $$ = new DotASTNode(".", $1, new NameASTNode($3), false);
  } | NAME '[' expr ']' {
      $$ = new IndexASTNode("", "[", "]", new NameASTNode($1), $3, false);
  } | lvalue_not_id '[' expr ']' {
      $$ = new IndexASTNode("", "[", "]", $1, $3, false);
  }

decllist: declaration {
        $$ = new DeclListASTNode(" ");
        $$->add_node($1);
  } | decllist declaration {
        $$ = $1;
        $$->add_node($2);
  }

declaration: typedecl {
        $$ = new DeclarationASTNode("", $1);
  } | vardecl {
        $$ = new DeclarationASTNode("", $1);
  } | funcdecl {
        std::cout << "the decleration is " << $1->toStr() << std::endl;
        $$ = new DeclarationASTNode("", $1);
  }

typedecl: TYPE NAME '=' type {
        $$ = new TypeDeclASTNode("type", "=", new NameASTNode($2), $4);
  }

type: NAME {
        $$ = new TypeASTNode("", new NameASTNode($1));
  } | '{' typefields_opt '}' {
        $$ = new TypeASTNode("", $2);
  } | ARRAY OF NAME {
        $$ = new TypeASTNode("", new ArrayTypeASTNode("array of", new NameASTNode($3)));
  }

typefields_opt: /* nothing */ {
        $$ = new RecordTypeASTNode(", ", "{", "}");
  } | typefields {
        $$ = $1;
  }

typefields: typefield {
        $$ = new RecordTypeASTNode(", ", "{", "}");
        $$->add_node($1);
  } | typefields ',' typefield {
        $$ = $1;
        $$->add_node($3);
  }

typefield: NAME ':' NAME {
       $$ = new RecordFieldASTNode(":", new NameASTNode($1), new NameASTNode($3), false);
  }

vardecl: VAR NAME ASSIGN expr {
        $$ = new UntypedVarDeclASTNode("var", ":=", new NameASTNode($2), $4);
  } | VAR NAME ':' NAME ASSIGN expr {
        $$ = new TypedVarDeclASTNode("var", ":", ":=", new NameASTNode($2), new NameASTNode($4), $6);
  }

funcdecl: FUNCTION NAME '(' typefields_opt ')' '=' expr {
        $$ = new UnTypedFuncDeclASTNode("function", "(", ") =", new NameASTNode($2), $4, $7);
  } | FUNCTION NAME '(' typefields_opt ')' ':' NAME '=' expr {
        $$ = new TypedFuncDeclASTNode("function ", "(", "):", "=", new NameASTNode($2), $4, new NameASTNode($7), $9);
  }

%%

void yyerror(tiger::ASTNode::ASTptr *out, const char *error) {
    cout << error << " " << yylineno << endl;
}
