#define CATCH_CONFIG_MAIN
#include "ast.hh"
#include "catch.hpp"
#include "tokens.hh"

#include <limits.h>
#include <cstdlib>
#include <ctime>

using namespace tiger;

extern "C" int yyparse(ASTNode::ASTptr *out);
extern FILE *yyin;

TEST_CASE("basic parsing", "[basic-parsing]") {
    FILE *myfile = fopen("test/ifthentest.tig", "r");
    yyin = myfile;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);
}
