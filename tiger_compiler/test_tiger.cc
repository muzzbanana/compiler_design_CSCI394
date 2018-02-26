#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "tokens.hh"
extern "C" int yylex();
extern FILE *yyin;

#include <limits.h>
#include <cstdlib>
#include <ctime>

//using namespace ???;

TEST_CASE("basic lexical specification", "[basic-lexing]") {
    FILE *myfile = fopen("test/tokentest.tig", "r");
    yyin = myfile;
    REQUIRE(yylex() == WHILE);
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == IF);
    fclose(myfile);
}
