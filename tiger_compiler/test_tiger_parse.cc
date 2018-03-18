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

TEST_CASE("if then parsing", "[basic-parsing]") {
    FILE *myfile = fopen("test/ifthentest.tig", "r");
    yyin = myfile;
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
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    REQUIRE(output != NULL);
    std::cout << output->toStr() << std::endl;
    REQUIRE(output->toStr() == "(let (var b := 100.000000) (type blah = int) (var c : blah := 20.000000) in )");
    delete output;
}
