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
    REQUIRE(yylex() == '.');
    REQUIRE(yylex() == ',');
    REQUIRE(yylex() == ':');
    REQUIRE(yylex() == ';');
    REQUIRE(yylex() == '(');
    REQUIRE(yylex() == ')');
    REQUIRE(yylex() == '[');
    REQUIRE(yylex() == ']');
    REQUIRE(yylex() == '{');
    REQUIRE(yylex() == '}');
    REQUIRE(yylex() == '+');
    REQUIRE(yylex() == '-');
    REQUIRE(yylex() == '*');
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == '/');
    REQUIRE(yylex() == '=');
    REQUIRE(yylex() == POINTIES);
    REQUIRE(yylex() == '<');
    REQUIRE(yylex() == END_OF_LINE);
    REQUIRE(yylex() == LESS_EQUAL);
    REQUIRE(yylex() == '>');
    REQUIRE(yylex() == GREATER_EQUAL);
    REQUIRE(yylex() == '&');
    REQUIRE(yylex() == '|');
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
    REQUIRE(yylex() == '=');
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
    REQUIRE(yylex() == ':');
    REQUIRE(yylex() == IDENTIFIER);
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == ASSIGN);
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == IDENTIFIER);
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == '[');
    REQUIRE(yylex() == INTEGER_LITERAL);
    REQUIRE(yylex() == ']');
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
    REQUIRE(yylex() == '=');
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == '{');
    REQUIRE(yylex() == IDENTIFIER);
    REQUIRE(yylex() == ':');
    REQUIRE(yylex() == STRING);
    REQUIRE(yylex() == ',');
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == IDENTIFIER);
    REQUIRE(yylex() == ':');
    REQUIRE(yylex() == INTEGER);
    REQUIRE(yylex() == '}');
    REQUIRE(yylex() == END_OF_LINE);
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == VAR);
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == IDENTIFIER);
    REQUIRE(yylex() == ':');
    REQUIRE(yylex() == IDENTIFIER);
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == ASSIGN);
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == IDENTIFIER);
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == '{');
    REQUIRE(yylex() == IDENTIFIER);
    REQUIRE(yylex() == '=');
    REQUIRE(yylex() == STRING_LITERAL);
    REQUIRE(yylex() == ',');
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == IDENTIFIER);
    REQUIRE(yylex() == '=');
    REQUIRE(yylex() == INTEGER_LITERAL);
    REQUIRE(yylex() == '}');
    REQUIRE(yylex() == END_OF_LINE);
    REQUIRE(yylex() == IN);
    REQUIRE(yylex() == END_OF_LINE);
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == IDENTIFIER);
    REQUIRE(yylex() == '.');
    REQUIRE(yylex() == IDENTIFIER);
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == ASSIGN);
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == STRING_LITERAL);
    REQUIRE(yylex() == ';');
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
    REQUIRE(yylex() == '(');
    REQUIRE(yylex() == IDENTIFIER);
    REQUIRE(yylex() == ':');
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == INTEGER);
    REQUIRE(yylex() == ')');
    REQUIRE(yylex() == ':');
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == INTEGER);
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == '=');
    REQUIRE(yylex() == END_OF_LINE);
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == IF);
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == IDENTIFIER);
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == '=');
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
    REQUIRE(yylex() == '*');
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == IDENTIFIER);
    REQUIRE(yylex() == '(');
    REQUIRE(yylex() == IDENTIFIER);
    REQUIRE(yylex() == '-');
    REQUIRE(yylex() == INTEGER_LITERAL);
    REQUIRE(yylex() == ')');
    REQUIRE(yylex() == END_OF_LINE);
    REQUIRE(yylex() == END_OF_LINE);
    REQUIRE(yylex() == IN);
    REQUIRE(yylex() == END_OF_LINE);
    REQUIRE(yylex() == SPACE);
    REQUIRE(yylex() == IDENTIFIER);
    REQUIRE(yylex() == '(');
    REQUIRE(yylex() == INTEGER_LITERAL);
    REQUIRE(yylex() == ')');
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

TEST_CASE("testing unterminated comment", "[basic-lexing]") {
    FILE *myfile = fopen("test/unterminated_comment.tig", "r");
    yyin = myfile;
    REQUIRE(yylex() == IDENTIFIER);
    REQUIRE(yylex() == END_OF_LINE);
    REQUIRE(yylex() == END_OF_LINE);
    REQUIRE(yylex() == ERROR_UNTERM_COMMENT);

    REQUIRE(yylex() == 0);

    fclose(myfile);
}

TEST_CASE("testing unterminated literal", "[basic-lexing]") {
    FILE *myfile = fopen("test/unterminated_literal.tig", "r");
    yyin = myfile;
    REQUIRE(yylex() == ERROR_UNTERM_STRING);
    REQUIRE(yylex() == END_OF_LINE);
    REQUIRE(yylex() == ERROR_UNTERM_STRING);
    REQUIRE(yylex() == END_OF_LINE);

    REQUIRE(yylex() == 0);

    fclose(myfile);
}

