#define CATCH_CONFIG_MAIN
#include "ast.hh"
#include "semantic.hh"
#include "semantic_checks.hh"
#include "catch.hpp"
#include "tiger.tab.h"
#include "irtree.hh"

#include <limits.h>
#include <cstdlib>
#include <ctime>
#include <typeinfo>

using namespace tiger;

int yyparse(ASTNode::ASTptr *out);
extern FILE *yyin;
extern int yylineno;

TEST_CASE("check simple int", "[ir-conversion]") {
    FILE *myfile = fopen("test_ir/num.tig", "r");
    yyin = myfile;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    Scope *s = new Scope();
    const Type *type = output->type_verify(s);
    REQUIRE(type == Type::intType);

    const IRTree *ir = output->convert_to_ir(NULL);
    std::cout << output->toStr() << std::endl;
    std::cout << ir->toStr() << std::endl;

    delete output;
    fclose(myfile);
}

TEST_CASE("check simple name", "[ir-conversion]") {
    FILE *myfile = fopen("test_ir/name.tig", "r");
    yyin = myfile;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    // Scope *s = new Scope();
    // const Type *type = output->type_verify(s);
    // REQUIRE(type == Type::intType);

    const IRTree *ir = output->convert_to_ir(NULL);
    std::cout << output->toStr() << std::endl;
    std::cout << ir->toStr() << std::endl;

    delete output;
    fclose(myfile);
}