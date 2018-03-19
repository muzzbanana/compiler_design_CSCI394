#define CATCH_CONFIG_MAIN
#include "ast.hh"
#include "catch.hpp"
#include "tiger.tab.h"

#include <limits.h>
#include <cstdlib>
#include <ctime>

using namespace tiger;

int yyparse(ASTNode::ASTptr *out);
extern FILE *yyin;
extern int yylineno;

TEST_CASE("if then parsing", "[basic-parsing]") {
    FILE *myfile = fopen("test/ifthentest.tig", "r");
    yyin = myfile;
    yylineno = 1;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    REQUIRE(output != NULL);
    std::cout << output->toStr() << std::endl;
    REQUIRE(output->eval() == 39);
    delete output;
}

TEST_CASE("nested if-else", "[basic-parsing]") {
    FILE *myfile = fopen("test/nestedifelse.tig", "r");
    yyin = myfile;
    yylineno = 1;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    REQUIRE(output != NULL);
    std::cout << output->toStr() << std::endl;
    REQUIRE(output->eval() == 7);
    delete output;
}

TEST_CASE("while parsing", "[basic-parsing]") {
    FILE *myfile = fopen("test/whiletest.tig", "r");
    yyin = myfile;
    yylineno = 1;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    REQUIRE(output != NULL);
    std::cout << output->toStr() << std::endl;
    REQUIRE(output->toStr() == "(while 1.000000 do 10.000000)");
    delete output;
}

TEST_CASE("for parsing", "[basic-parsing]") {
    FILE *myfile = fopen("test/fortest.tig", "r");
    yyin = myfile;
    yylineno = 1;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    REQUIRE(output != NULL);
    std::cout << output->toStr() << std::endl;
    REQUIRE(output->toStr() == "(for i := 0.000000 to 10.000000 do 10.000000)");
    delete output;
}

TEST_CASE("var decl parsing", "[basic-parsing]") {
    FILE *myfile = fopen("test/vardecltest.tig", "r");
    yyin = myfile;
    yylineno = 1;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    REQUIRE(output != NULL);
    std::cout << output->toStr() << std::endl;
    REQUIRE(output->toStr() == "(let (var b := 100.000000) (type blah = int) (var c : blah := 20.000000) in  end)");
    delete output;
}

TEST_CASE("field list parsing", "[basic-parsing]") {
    FILE *myfile = fopen("test/fieldtest.tig", "r");
    yyin = myfile;
    yylineno = 1;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    REQUIRE(output != NULL);
    std::cout << output->toStr() << std::endl;
    REQUIRE(output->toStr() == "(let (type rectype = { name:string, id:int }) (var rec1 := (rectype { name=\"Name\", id=0.000000 })) in ((rec1.name) := \"asd\") end)");
    delete output;
}

TEST_CASE("array expr parsing", "[basic-parsing]") {
    FILE *myfile = fopen("test/arrayvaluetest.tig", "r");
    yyin = myfile;
    yylineno = 1;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    REQUIRE(output != NULL);
    std::cout << output->toStr() << std::endl;
    REQUIRE(output->toStr() == "(let (var a := int [5.000000] of 10.000000) in a end)");
    delete output;
}


TEST_CASE("untyped function parsing", "[basic-parsing]") {
    FILE *myfile = fopen("test/untypedfunc.tig", "r");
    yyin = myfile;
    yylineno = 1;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    REQUIRE(output != NULL);
    std::cout << output->toStr() << std::endl;
    REQUIRE(output->toStr() == "(let (function g(a:int) = a) in (g(2.000000)) end)");
    delete output;
}

TEST_CASE("break parsing", "[basic-parsing]") {
    FILE *myfile = fopen("test/breaktest.tig", "r");
    yyin = myfile;
    yylineno = 1;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    REQUIRE(output != NULL);
    std::cout << output->toStr() << std::endl;
    REQUIRE(output->toStr() == "(while 1.000000 do break)");
    delete output;
}

TEST_CASE("dot associativity", "[basic-parsing]") {
    FILE *myfile = fopen("test/multidottest.tig", "r");
    yyin = myfile;
    yylineno = 1;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    REQUIRE(output != NULL);
    std::cout << output->toStr() << std::endl;
    // in particular we're noting that it becomes ((rec1.rec).name) rather than (rec1.(rec.name))... the rest of it is just kind of silly
    REQUIRE(output->toStr() == std::string("(let (type t1 = { name:string, id:int }) (type t2 = { rec:t1 }) ")+
            "(var rec1 := (t2 { rec=(t1 { name=\"Bruno\", id=17.000000 }) })) in (((rec1.rec).name) := \"Benson\") end)");
    delete output;
}

TEST_CASE("expr_seq test", "[basic-parsing]") {
    FILE *myfile = fopen("test/exprseq.tig", "r");
    yyin = myfile;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    REQUIRE(output != NULL);
    std::cout << output->toStr() << std::endl;
    REQUIRE(output->toStr() == std::string("(let (type arrtype1 = array of int) in ((arr1[0] := 1;)")+
            "arr1[9] := 3; arr2[3].name := \"kati\"; arr2[1].age := 23; arr3[34] := \"sfd\") end)");
    delete output;
}
