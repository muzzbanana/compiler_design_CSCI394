%code requires {
    using namespace tiger;
}

%{
#include <iostream>
#include <string>
#include "ast.hh"
#include "semantic_checks.hh"
//#define YY_DECL extern "C" int yylex()

using namespace std;

extern "C" int yylex(void);
extern "C" int yylineno;
extern int current_line;
void yyerror(tiger::ASTNode::ASTptr *out, const char *);
tiger::ASTNode::ASTptr name(const char *str);
%}

%union {
    double              d;
    const char*         str;

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
%locations

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
       *out = $1;
  }

expr: STR {
        $$ = new StrASTNode($1);
        free((char*)$1); // free the duplicated string
  } | NUMBER {
        $$ = new NumASTNode($1);
  } | NIL {
        $$ = new NilASTNode();
  } | lvalue {
        $$ = $1;
  } | '-' expr {
        $$ = new UnaryMinusASTNode("-", $2, @$.first_line);
  } | expr '+' expr {
        $$ = new PlusASTNode("+", $1, $3, @$.first_line);
  } | expr '-' expr {
        $$ = new MinusASTNode("-", $1, $3, @$.first_line);
  } | expr '*' expr {
        $$ = new MultASTNode("*", $1, $3, @$.first_line);
  } | expr '/' expr {
        $$ = new DivASTNode("/", $1, $3, @$.first_line);
  } | expr '=' expr {
        $$ = new EqualASTNode("=", $1, $3, @$.first_line);
  } | expr POINTIES expr {
        $$ = new NotEqualASTNode("<>", $1, $3, @$.first_line);
  } | expr '<' expr {
        $$ = new LessASTNode("<", $1, $3, @$.first_line);
  } | expr '>' expr {
        $$ = new GreaterASTNode(">", $1, $3, @$.first_line);
  } | expr LESS_EQUAL expr {
        $$ = new LessEqualASTNode("<=", $1, $3, @$.first_line);
  } | expr GREATER_EQUAL expr {
        $$ = new GreaterEqualASTNode(">=", $1, $3, @$.first_line);
  } | expr '&' expr {
        $$ = new LogicalAndASTNode("&", $1, $3, @$.first_line);
  } | expr '|' expr {
        $$ = new LogicalOrASTNode("|", $1, $3, @$.first_line);
  } | lvalue ASSIGN expr {
        $$ = new AssignASTNode(":=", $1, $3, @$.first_line);
  } | NAME '(' exprlist_opt ')' {
        $$ = new FuncCallASTNode("(", "(", "))", name($1), $3, @$.first_line, false);
  } | '(' exprseq_opt ')' {
        $$ = $2;
  } | NAME '{' fieldlist_opt '}' {
        $$ = new TypeInstASTNode("", "{", "}", name($1), $3, @$.first_line);
  } | NAME '[' expr ']' OF expr {     /* array */
        $$ = new ArrayASTNode("", " [", "] of ", name($1), $3, $6, @$.first_line, false);
  } | IF expr THEN expr {
        $$ = new ConditionalASTNode("if", "then", "else", $2, $4, new NilASTNode(), @$.first_line);
  } | IF expr THEN expr ELSE expr {
        $$ = new ConditionalASTNode("if", "then", "else", $2, $4, $6, @$.first_line);
  } | WHILE expr DO expr {
        $$ = new WhileLoopASTNode("while", "do", $2, $4, @$.first_line);
  } | FOR NAME ASSIGN expr TO expr DO expr {
        $$ = new ForLoopASTNode("for", ":=", "to", "do", name($2), $4, $6, $8, @$.first_line);
  } | BREAK {
        $$ = new BreakASTNode(@$.first_line);
  } | LET decllist IN exprseq_opt END {
        $$ = new LetASTNode("let", "in", "end", $2, $4, @$.first_line);
  }

exprlist_opt: /* nothing */ {
        $$ = new ExprSeqASTNode(", ", @$.first_line);
  } | exprlist {
        $$ = $1;
  }

exprseq_opt: /* nothing */ {
        $$ = new ExprSeqASTNode("; ", @$.first_line);
  } | exprseq {
        $$ = $1;
  }

fieldlist_opt: /* nothing */ {
        $$ = new FieldListASTNode(", ", @$.first_line);
  } | fieldlist {
        $$ = $1;
  }

exprseq: expr {
        $$ = new ExprSeqASTNode("; ", @$.first_line);
        $$->add_node($1);
  } | exprseq ';' expr {
        $$ = $1;
        $$->add_node($3);
  } | error ';' {
        yyerrok;
  }

exprlist: expr {
        $$ = new ExprSeqASTNode(", ", @$.first_line);
        $$->add_node($1);
  } | exprlist ',' expr {
        $$ = $1;
        $$->add_node($3);
  }

fieldlist: NAME '=' expr {
        $$ = new FieldListASTNode(", ", @$.first_line);
        $$->add_node(new FieldMemberASTNode("=", name($1), $3, false));
  } | fieldlist ',' NAME '=' expr {
        $$ = $1;
        $$->add_node(new FieldMemberASTNode("=", name($3), $5, false));
  }

lvalue: NAME {
        $$ = name($1);
  } | lvalue_not_id {
        $$ = $1;
  }

lvalue_not_id: lvalue '.' NAME {
        $$ = new DotASTNode("(", ".", ")", $1, name($3), @$.first_line, false);
  } | NAME '[' expr ']' {
        $$ = new IndexASTNode("", "[", "]", name($1), $3, @$.first_line, false);
  } | lvalue_not_id '[' expr ']' {
        $$ = new IndexASTNode("", "[", "]", $1, $3, @$.first_line, false);
  }

decllist: declaration {
        $$ = new DeclListASTNode(" ", @$.first_line);
        $$->add_node($1);
  } | decllist declaration {
        $$ = $1;
        $$->add_node($2);
  } | decllist error {
        yyerrok;
  }

declaration: typedecl {
        $$ = new DeclarationASTNode("", $1, @$.first_line);
  } | vardecl {
        $$ = new DeclarationASTNode("", $1, @$.first_line);
  } | funcdecl {
        $$ = new DeclarationASTNode("", $1, @$.first_line);
  }

typedecl: TYPE NAME '=' type {
        $$ = new TypeDeclASTNode("type", "=", name($2), $4, @$.first_line);
  }

type: NAME {
        $$ = new TypeASTNode("", name($1), @$.first_line);
  } | '{' typefields_opt '}' {
        $$ = new TypeASTNode("{ ", " }", $2, @$.first_line);
  } | ARRAY OF NAME {
        $$ = new TypeASTNode("", new ArrayTypeASTNode("array of ", name($3), @3.first_line), @$.first_line);
  }

typefields_opt: /* nothing */ {
        $$ = new RecordTypeASTNode(", ", @$.first_line);
  } | typefields {
        $$ = $1;
  }

typefields: typefield {
        $$ = new RecordTypeASTNode(", ", @$.first_line);
        $$->add_node($1);
  } | typefields ',' typefield {
        $$ = $1;
        $$->add_node($3);
  }

typefield: NAME ':' NAME {
        $$ = new RecordFieldASTNode(":", name($1), name($3), @$.first_line, false);
  }

vardecl: VAR NAME ASSIGN expr {
        $$ = new UntypedVarDeclASTNode("var", ":=", name($2), $4, @$.first_line);
  } | VAR NAME ':' NAME ASSIGN expr {
        $$ = new TypedVarDeclASTNode("var", ":", ":=", name($2), name($4), $6, @$.first_line);
  }

funcdecl: FUNCTION NAME '(' typefields_opt ')' '=' expr {
        $$ = new UnTypedFuncDeclASTNode("(function ", "(", ") = ", ")", name($2), $4, $7, @$.first_line, false);
  } | FUNCTION NAME '(' typefields_opt ')' ':' NAME '=' expr {
        $$ = new TypedFuncDeclASTNode("(function ", "(", "):", "=", ")", name($2), $4, name($7), $9, @$.first_line, false);

  }

%%

void yyerror(tiger::ASTNode::ASTptr *out, const char *error) {
    cerr << "ERROR: line " << yylineno << " column " << yylloc.first_column << "-" << yylloc.last_column << endl;
    cerr << error << " " << yylloc.first_line << endl;
}

int send_line() {
  return yylineno;
}

/* Create a new NameASTNode from a duplicated const char* without causing a memory leak.
 * This is necessary because flex calls strdup() in order to make sure the pointer to the
 * string doesn't change, but this results in a memory leak if the newly duplicated string
 * is not freed properly. */
tiger::ASTNode::ASTptr name(const char *str) {
    tiger::ASTNode::ASTptr new_node = new NameASTNode(str);
    free((char*)str); // free the duplicated string after it's safely stored in a std::string
    return new_node;
}
