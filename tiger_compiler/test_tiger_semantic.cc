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

TEST_CASE("fail on bad record type", "[semantic-check]") {
    FILE *myfile = fopen("test_semantic/bad_record_type.tig", "r");
    yyin = myfile;
    yylineno = 1;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    delete output;
}

TEST_CASE("fail on expression type mismatch", "[semantic-check]") {
    FILE *myfile = fopen("test_semantic/expr_type_mismatch.tig", "r");
    yyin = myfile;
    yylineno = 1;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    delete output;
}

TEST_CASE("fail on incorrect nesting", "[semantic-check]") {
    FILE *myfile = fopen("test_semantic/incorrect_nesting.tig", "r");
    yyin = myfile;
    yylineno = 1;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    delete output;
}

TEST_CASE("fail on mutually recursive function type mismatch", "[semantic-check]") {
    FILE *myfile = fopen("test_semantic/mutual_recursive_mismatch.tig", "r");
    yyin = myfile;
    yylineno = 1;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    delete output;
}

TEST_CASE("fail on recursive argument type mismatch", "[semantic-check]") {
    FILE *myfile = fopen("test_semantic/recursive_arg_mismatch.tig", "r");
    yyin = myfile;
    yylineno = 1;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    delete output;
}

TEST_CASE("fail on recursive return type mismatch", "[semantic-check]") {
    FILE *myfile = fopen("test_semantic/recursive_func_mismatch.tig", "r");
    yyin = myfile;
    yylineno = 1;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    delete output;
}

TEST_CASE("fail on repeated definition of var", "[semantic-check]") {
    FILE *myfile = fopen("test_semantic/repeat_definition.tig", "r");
    yyin = myfile;
    yylineno = 1;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    delete output;
}

TEST_CASE("fail on use of undefined identifier", "[semantic-check]") {
    FILE *myfile = fopen("test_semantic/undefined_identifier.tig", "r");
    yyin = myfile;
    yylineno = 1;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    delete output;
}

TEST_CASE("fail on array index out of range", "[semantic-check]") {
    FILE *myfile = fopen("test_semantic/array_index_out_of_range.tig", "r");
    yyin = myfile;
    yylineno = 1;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    delete output;
}

TEST_CASE("fail on invalid type for array", "[semantic-check]") {
    FILE *myfile = fopen("test_semantic/invalid_type_for_array.tig", "r");
    yyin = myfile;
    yylineno = 1;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    delete output;
}

TEST_CASE("fail on repetition of function arguments", "[semantic-check]") {
    FILE *myfile = fopen("test_semantic/repitition_of_func_args.tig", "r");
    yyin = myfile;
    yylineno = 1;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    delete output;
}

TEST_CASE("fail on incorrect use of reserved words", "[semantic-check]") {
    FILE *myfile = fopen("test_semantic/reserved_words.tig", "r");
    yyin = myfile;
    yylineno = 1;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    delete output;
}

TEST_CASE("DON'T fail on using the same name in different scope", "[semantic-check]") {
    FILE *myfile = fopen("test_semantic/same_name_diff_scope.tg", "r");
    yyin = myfile;
    yylineno = 1;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    delete output;
}
