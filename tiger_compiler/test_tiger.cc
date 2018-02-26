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
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == ARRAY);
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == THEN);
    REQUIRE(yylex() == END_OF_LINE);
    REQUIRE(yylex() == SPACE);          // tests tabs
    REQUIRE(yylex() == ELSE);
    REQUIRE(yylex() == END_OF_LINE);
    REQUIRE(yylex() == FOR);
    REQUIRE(yylex() == END_OF_LINE);
    REQUIRE(yylex() == TO);
    REQUIRE(yylex() == END_OF_LINE);
    REQUIRE(yylex() == DO);
    REQUIRE(yylex() == END_OF_LINE);
    REQUIRE(yylex() == LET);
    REQUIRE(yylex() == END_OF_LINE);
    REQUIRE(yylex() == IN);
    REQUIRE(yylex() == END_OF_LINE);
    REQUIRE(yylex() == END);
    REQUIRE(yylex() == END_OF_LINE);
    REQUIRE(yylex() == OF);
    REQUIRE(yylex() == END_OF_LINE);
    REQUIRE(yylex() == BREAK);
    REQUIRE(yylex() == END_OF_LINE);
    REQUIRE(yylex() == NIL);
    REQUIRE(yylex() == END_OF_LINE);
    REQUIRE(yylex() == FUNCTION);
    REQUIRE(yylex() == END_OF_LINE);
    REQUIRE(yylex() == VAR);
    REQUIRE(yylex() == END_OF_LINE);
    REQUIRE(yylex() == TYPE);
    REQUIRE(yylex() == END_OF_LINE);
    REQUIRE(yylex() == IMPORT);
    REQUIRE(yylex() == END_OF_LINE);
    REQUIRE(yylex() == PRIMITIVE);
    REQUIRE(yylex() == END_OF_LINE);
    REQUIRE(yylex() == CLASS);
    REQUIRE(yylex() == END_OF_LINE);
    REQUIRE(yylex() == EXTENDS);
    REQUIRE(yylex() == END_OF_LINE);
    REQUIRE(yylex() == METHOD);
    REQUIRE(yylex() == END_OF_LINE);
    REQUIRE(yylex() == NEW);
    REQUIRE(yylex() == END_OF_LINE);
    REQUIRE(yylex() == STOP);
    REQUIRE(yylex() == COMMA);
    REQUIRE(yylex() == COLON);
    REQUIRE(yylex() == SEMICOLON);
    REQUIRE(yylex() == OPEN_PAREN);
    REQUIRE(yylex() == CLOSE_PAREN);
    REQUIRE(yylex() == OPEN_BRACKET);
    REQUIRE(yylex() == CLOSE_BRACKET);
    REQUIRE(yylex() == OPEN_CURLY);
    REQUIRE(yylex() == CLOSE_CURLY);
    REQUIRE(yylex() == PLUS);
    REQUIRE(yylex() == MINUS);
    REQUIRE(yylex() == MULT);
    REQUIRE(yylex() == DIV);
    REQUIRE(yylex() == EQUAL);
    REQUIRE(yylex() == POINTIES);
    REQUIRE(yylex() == LESS);
    REQUIRE(yylex() == END_OF_LINE);
    REQUIRE(yylex() == LESS_EQUAL);
    REQUIRE(yylex() == GREATER);
    REQUIRE(yylex() == GREATER_EQUAL);
    REQUIRE(yylex() == AND);
    REQUIRE(yylex() == OR);
    REQUIRE(yylex() == ASSIGN);
    REQUIRE(yylex() == END_OF_LINE);


    fclose(myfile);
}
