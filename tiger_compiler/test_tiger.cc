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
    REQUIRE(yylex() == SPACE);
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
    REQUIRE(yylex() == 0);


    fclose(myfile);
}

TEST_CASE("testing test 1", "[basic-lexing]") {
    FILE *myfile = fopen("test/test1.tig", "r");
    yyin = myfile;
    REQUIRE(yylex() == END_OF_LINE);
    REQUIRE(yylex() == LET);
    REQUIRE(yylex() == END_OF_LINE);
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == TYPE);
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == IDENTIFIER);
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == EQUAL);
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == ARRAY);
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == OF);
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == INTEGER);
    REQUIRE(yylex() == END_OF_LINE);
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == VAR);
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == IDENTIFIER);
    REQUIRE(yylex() == COLON);
    REQUIRE(yylex() == IDENTIFIER);
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == ASSIGN);
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == IDENTIFIER);
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == OPEN_BRACKET);
    REQUIRE(yylex() == INTEGER_LITERAL);
    REQUIRE(yylex() == CLOSE_BRACKET);
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == OF);
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == INTEGER_LITERAL);
    REQUIRE(yylex() == END_OF_LINE);
    REQUIRE(yylex() == IN);
    REQUIRE(yylex() == END_OF_LINE);
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == IDENTIFIER);
    REQUIRE(yylex() == END_OF_LINE);
    REQUIRE(yylex() == END);
    REQUIRE(yylex() == END_OF_LINE);
    REQUIRE(yylex() == 0);

    REQUIRE(yylex() == 0);

    fclose(myfile);
}

TEST_CASE("literals recognized properly", "[basic-lexing]") {
    FILE *myfile = fopen("test/literaltest.tig", "r");
    yyin = myfile;

    REQUIRE(yylex() == END_OF_LINE);
    REQUIRE(yylex() == END_OF_LINE);
    REQUIRE(yylex() == STRING_LITERAL);
    REQUIRE(yylex() == END_OF_LINE);
    REQUIRE(yylex() == STRING_LITERAL);
    REQUIRE(yylex() == END_OF_LINE);
    REQUIRE(yylex() == INTEGER_LITERAL);
    REQUIRE(yylex() == END_OF_LINE);
    REQUIRE(yylex() == INTEGER_LITERAL);
    REQUIRE(yylex() == END_OF_LINE);
    REQUIRE(yylex() == STRING_LITERAL);
    REQUIRE(yylex() == END_OF_LINE);

    REQUIRE(yylex() == 0);

    fclose(myfile);
}

TEST_CASE("testing test 2", "[basic-lexing]") {
    FILE *myfile = fopen("test/test2.tig", "r");
    yyin = myfile;
    REQUIRE(yylex() == END_OF_LINE);
    REQUIRE(yylex() == LET);
    REQUIRE(yylex() == END_OF_LINE);
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == TYPE);
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == IDENTIFIER);
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == EQUAL);
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == OPEN_CURLY);
    REQUIRE(yylex() == IDENTIFIER);
    REQUIRE(yylex() == COLON);
    REQUIRE(yylex() == STRING);
    REQUIRE(yylex() == COMMA);
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == IDENTIFIER);
    REQUIRE(yylex() == COLON);
    REQUIRE(yylex() == INTEGER);
    REQUIRE(yylex() == CLOSE_CURLY);
    REQUIRE(yylex() == END_OF_LINE);
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == VAR);
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == IDENTIFIER);
    REQUIRE(yylex() == COLON);
    REQUIRE(yylex() == IDENTIFIER);
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == ASSIGN);
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == IDENTIFIER);
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == OPEN_CURLY);
    REQUIRE(yylex() == IDENTIFIER);
    REQUIRE(yylex() == EQUAL);
    REQUIRE(yylex() == STRING_LITERAL);
    REQUIRE(yylex() == COMMA);
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == IDENTIFIER);
    REQUIRE(yylex() == EQUAL);
    REQUIRE(yylex() == INTEGER_LITERAL);
    REQUIRE(yylex() == CLOSE_CURLY);
    REQUIRE(yylex() == END_OF_LINE);
    REQUIRE(yylex() == IN);
    REQUIRE(yylex() == END_OF_LINE);
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == IDENTIFIER);
    REQUIRE(yylex() == STOP);
    REQUIRE(yylex() == IDENTIFIER);
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == ASSIGN);
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == STRING_LITERAL);
    REQUIRE(yylex() == SEMICOLON);
    REQUIRE(yylex() == END_OF_LINE);
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == IDENTIFIER);
    REQUIRE(yylex() == END_OF_LINE);
    REQUIRE(yylex() == END);
    REQUIRE(yylex() == END_OF_LINE);

    REQUIRE(yylex() == 0);

    fclose(myfile);
}

TEST_CASE("testing test 3", "[basic-lexing]") {
    FILE *myfile = fopen("test/test3.tig", "r");
    yyin = myfile;

    REQUIRE(yylex() == END_OF_LINE);
    REQUIRE(yylex() == LET);
    REQUIRE(yylex() == END_OF_LINE);
    REQUIRE(yylex() == END_OF_LINE);
    REQUIRE(yylex() == END_OF_LINE);
    REQUIRE(yylex() == FUNCTION);
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == IDENTIFIER);
    REQUIRE(yylex() == OPEN_PAREN);
    REQUIRE(yylex() == IDENTIFIER);
    REQUIRE(yylex() == COLON);
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == INTEGER);
    REQUIRE(yylex() == CLOSE_PAREN);
    REQUIRE(yylex() == COLON);
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == INTEGER);
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == EQUAL);
    REQUIRE(yylex() == END_OF_LINE);
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == IF);
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == IDENTIFIER);
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == EQUAL);
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == INTEGER_LITERAL);
    REQUIRE(yylex() == END_OF_LINE);
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == THEN);
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == INTEGER_LITERAL);
    REQUIRE(yylex() == END_OF_LINE);
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == ELSE);
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == IDENTIFIER);
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == MULT);
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == IDENTIFIER);
    REQUIRE(yylex() == OPEN_PAREN);
    REQUIRE(yylex() == IDENTIFIER);
    REQUIRE(yylex() == MINUS);
    REQUIRE(yylex() == INTEGER_LITERAL);
    REQUIRE(yylex() == CLOSE_PAREN);
    REQUIRE(yylex() == END_OF_LINE);
    REQUIRE(yylex() == END_OF_LINE);
    REQUIRE(yylex() == IN);
    REQUIRE(yylex() == END_OF_LINE);
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == IDENTIFIER);
    REQUIRE(yylex() == OPEN_PAREN);
    REQUIRE(yylex() == INTEGER_LITERAL);
    REQUIRE(yylex() == CLOSE_PAREN);
    REQUIRE(yylex() == END_OF_LINE);
    REQUIRE(yylex() == END);
    REQUIRE(yylex() == END_OF_LINE);

    REQUIRE(yylex() == 0);

    fclose(myfile);
}

TEST_CASE("testing nested comments", "[basic-lexing]") {
    FILE *myfile = fopen("test/nested_comment.tig", "r");
    yyin = myfile;
    REQUIRE(yylex() == END_OF_LINE);

REQUIRE(yylex() == 0);

    fclose(myfile);
}
