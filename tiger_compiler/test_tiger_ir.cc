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

    const IRTree *ir = convert_ast(output);
    std::cout << output->toStr() << std::endl;
    std::cout << ir->toStr() << std::endl;
    std::cout << ir->vectorize()->toStr() << std::endl;

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

    const IRTree *ir = convert_ast(output);
    std::cout << output->toStr() << std::endl;
    std::cout << ir->toStr() << std::endl;
    std::cout << ir->vectorize()->toStr() << std::endl;

    delete output;
    fclose(myfile);
}

TEST_CASE("check binop plus", "[ir-conversion]") {
    FILE *myfile = fopen("test_ir/plus.tig", "r");
    yyin = myfile;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    // Scope *s = new Scope();
    // const Type *type = output->type_verify(s);
    // REQUIRE(type == Type::intType);

    const IRTree *ir = convert_ast(output);
    std::cout << output->toStr() << std::endl;
    std::cout << ir->toStr() << std::endl;
    std::cout << ir->vectorize()->toStr() << std::endl;

    delete output;
    fclose(myfile);
}

TEST_CASE("check simple if/then/else", "[ir-conversion]") {
    FILE *myfile = fopen("test_ir/ifthenelse.tig", "r");
    yyin = myfile;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    // Scope *s = new Scope();
    // const Type *type = output->type_verify(s);
    // REQUIRE(type == Type::intType);

    const IRTree *ir = convert_ast(output);
    std::cout << output->toStr() << std::endl;
    std::cout << ir->toStr() << std::endl;
    std::cout << ir->vectorize()->toStr() << std::endl;

    delete output;
    fclose(myfile);
}

TEST_CASE("check simple while", "[ir-conversion]") {
    FILE *myfile = fopen("test_ir/while.tig", "r");
    yyin = myfile;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    // Scope *s = new Scope();
    // const Type *type = output->type_verify(s);
    // REQUIRE(type == Type::intType);

    const IRTree *ir = convert_ast(output);
    std::cout << output->toStr() << std::endl;
    std::cout << ir->toStr() << std::endl;
    std::cout << ir->vectorize()->toStr() << std::endl;

    delete output;
    fclose(myfile);
}

TEST_CASE("check simple assign", "[ir-conversion]") {
    FILE *myfile = fopen("test_ir/assign.tig", "r");
    yyin = myfile;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    const IRTree *ir = convert_ast(output);
    std::cout << output->toStr() << std::endl;
    std::cout << ir->toStr() << std::endl;
    std::cout << ir->vectorize()->toStr() << std::endl;

    delete output;
    fclose(myfile);
}

TEST_CASE("check number as if", "[ir-conversion]") {
    FILE *myfile = fopen("test_ir/condition_number.tig", "r");
    yyin = myfile;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    const IRTree *ir = convert_ast(output);
    std::cout << output->toStr() << std::endl;
    std::cout << ir->toStr() << std::endl;
    std::cout << ir->vectorize()->toStr() << std::endl;

    delete output;
    fclose(myfile);
}

TEST_CASE("check simple for", "[ir-conversion]") {
    FILE *myfile = fopen("test_ir/for.tig", "r");
    yyin = myfile;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    const IRTree *ir = convert_ast(output);
    std::cout << output->toStr() << std::endl;
    std::cout << ir->toStr() << std::endl;
    std::cout << ir->vectorize()->toStr() << std::endl;

    delete output;
    fclose(myfile);
}

TEST_CASE("check var-name-listing", "[var-names]") {
    FILE *myfile = fopen("test_ir/varname.tig", "r");
    yyin = myfile;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    /* Should NOT print out r or q */
    std::stringstream ss;
    const std::vector<std::string> vars = output->get_var_names();
    for (auto a : vars) {
        ss << a << ", ";
    }
    std::cout << ss.str() << std::endl;

    REQUIRE(ss.str() == "i, j, k, c, ");

    delete output;
    fclose(myfile);
}

TEST_CASE("check variable declaration", "[ir-conversion]") {
    FILE *myfile = fopen("test_ir/var_decl.tig", "r");
    yyin = myfile;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    const IRTree *ir = convert_ast(output);
    std::cout << output->toStr() << std::endl;
    std::cout << ir->toStr() << std::endl;
    std::cout << ir->vectorize()->toStr() << std::endl;

    delete output;
    fclose(myfile);
}
