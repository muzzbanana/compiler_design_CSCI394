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

void print_instrs(InstructionList il) {
    for (auto a : il) {
        std::cout << a->toStr() << std::endl;
    }
}

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

    const ProgramTree *ir = convert_ast(output);
    std::cout << output->toStr() << std::endl;
    std::cout << ir->toStr() << std::endl;

    const ProgramFragment *fr = ir->vectorize();
    std::cout << "\n== ASM ==\n" << std::endl;
    print_instrs(fr->munch());
    std::cout << "\n";

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

    const ProgramTree *ir = convert_ast(output);
    std::cout << output->toStr() << std::endl;
    std::cout << ir->toStr() << std::endl;

    const ProgramFragment *fr = ir->vectorize();
    std::cout << "\n== ASM ==\n" << std::endl;
    print_instrs(fr->munch());
    std::cout << "\n";

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

    const ProgramTree *ir = convert_ast(output);
    std::cout << output->toStr() << std::endl;
    std::cout << ir->toStr() << std::endl;

    const ProgramFragment *fr = ir->vectorize();
    std::cout << "\n== ASM ==\n" << std::endl;
    print_instrs(fr->munch());
    std::cout << "\n";

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

    const ProgramTree *ir = convert_ast(output);
    std::cout << output->toStr() << std::endl;
    std::cout << ir->toStr() << std::endl;

    const ProgramFragment *fr = ir->vectorize();
    std::cout << "\n== ASM ==\n" << std::endl;
    print_instrs(fr->munch());
    std::cout << "\n";

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

    const ProgramTree *ir = convert_ast(output);
    std::cout << output->toStr() << std::endl;
    std::cout << ir->toStr() << std::endl;

    const ProgramFragment *fr = ir->vectorize();
    std::cout << "\n== ASM ==\n" << std::endl;
    print_instrs(fr->munch());
    std::cout << "\n";

    delete output;
    fclose(myfile);
}

TEST_CASE("check simple assign", "[ir-conversion]") {
    FILE *myfile = fopen("test_ir/assign.tig", "r");
    yyin = myfile;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    const ProgramTree *ir = convert_ast(output);
    std::cout << output->toStr() << std::endl;
    std::cout << ir->toStr() << std::endl;

    const ProgramFragment *fr = ir->vectorize();
    std::cout << "\n== ASM ==\n" << std::endl;
    print_instrs(fr->munch());
    std::cout << "\n";

    delete output;
    fclose(myfile);
}

TEST_CASE("check number as if", "[ir-conversion]") {
    FILE *myfile = fopen("test_ir/condition_number.tig", "r");
    yyin = myfile;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    const ProgramTree *ir = convert_ast(output);
    std::cout << output->toStr() << std::endl;
    std::cout << ir->toStr() << std::endl;

    const ProgramFragment *fr = ir->vectorize();
    std::cout << "\n== ASM ==\n" << std::endl;
    print_instrs(fr->munch());
    std::cout << "\n";

    delete output;
    fclose(myfile);
}

TEST_CASE("check simple for", "[ir-conversion]") {
    FILE *myfile = fopen("test_ir/for.tig", "r");
    yyin = myfile;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    const ProgramTree *ir = convert_ast(output);
    std::cout << output->toStr() << std::endl;
    std::cout << ir->toStr() << std::endl;

    const ProgramFragment *fr = ir->vectorize();
    std::cout << "\n== ASM ==\n" << std::endl;
    print_instrs(fr->munch());
    std::cout << "\n";

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

    const ProgramTree *ir = convert_ast(output);
    std::cout << output->toStr() << std::endl;
    std::cout << ir->toStr() << std::endl;

    const ProgramFragment *fr = ir->vectorize();
    std::cout << "\n== ASM ==\n" << std::endl;
    print_instrs(fr->munch());
    std::cout << "\n";

    delete output;
    fclose(myfile);
}

TEST_CASE("check function declaration", "[ir-conversion]") {
    FILE *myfile = fopen("test_ir/funcdecl.tig", "r");
    yyin = myfile;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    const ProgramTree *ir = convert_ast(output);
    std::cout << output->toStr() << std::endl;
    std::cout << ir->toStr() << std::endl;

    const ProgramFragment *fr = ir->vectorize();
    std::cout << "\n== ASM ==\n" << std::endl;
    print_instrs(fr->munch());
    std::cout << "\n";

    delete output;
    fclose(myfile);
}

TEST_CASE("check recursive function from test_semantic", "[ir-conversion]") {
    FILE *myfile = fopen("test_ir/semantic_recursive.tig", "r");
    yyin = myfile;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    const int result = semantic_checks(output);
    REQUIRE(result == 0);
    const ProgramTree *ir = convert_ast(output);
    std::cout << output->toStr() << std::endl;
    std::cout << ir->toStr() << std::endl;

    const ProgramFragment *fr = ir->vectorize();
    std::cout << "\n== ASM ==\n" << std::endl;
    print_instrs(fr->munch());
    std::cout << "\n";

    delete output;
    fclose(myfile);
}

TEST_CASE("check array declaration", "[ir-conversion]") {
    FILE *myfile = fopen("test_ir/array_decl.tig", "r");
    yyin = myfile;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    const ProgramTree *ir = convert_ast(output);
    std::cout << output->toStr() << std::endl;
    std::cout << ir->toStr() << std::endl;

    const ProgramFragment *fr = ir->vectorize();
    std::cout << "\n== ASM ==\n" << std::endl;
    print_instrs(fr->munch());
    std::cout << "\n";

    delete output;
    fclose(myfile);
}

TEST_CASE("check nil", "[ir-conversion]") {
    FILE *myfile = fopen("test_ir/niltest.tig", "r");
    yyin = myfile;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    const ProgramTree *ir = convert_ast(output);

    REQUIRE(ir == NULL);

    delete output;
    fclose(myfile);
}

TEST_CASE("check regular funccall", "[ir-conversion]") {
    FILE *myfile = fopen("test_ir/regfunc.tig", "r");
    yyin = myfile;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    const ProgramTree *ir = convert_ast(output);
    std::cout << output->toStr() << std::endl;
    std::cout << ir->toStr() << std::endl;

    const ProgramFragment *fr = ir->vectorize();
    std::cout << "\n== ASM ==\n" << std::endl;
    print_instrs(fr->munch());
    std::cout << "\n";

    delete output;
    fclose(myfile);
}

TEST_CASE("check funccall w/ multiple args", "[ir-conversion]") {
    FILE *myfile = fopen("test_ir/multiargfunc.tig", "r");
    yyin = myfile;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    const ProgramTree *ir = convert_ast(output);
    std::cout << output->toStr() << std::endl;
    std::cout << ir->toStr() << std::endl;

    const ProgramFragment *fr = ir->vectorize();
    std::cout << "\n== ASM ==\n" << std::endl;
    print_instrs(fr->munch());
    std::cout << "\n";

    delete output;
    fclose(myfile);
}

TEST_CASE("check recursive funccall", "[ir-conversion]") {
    FILE *myfile = fopen("test_ir/factorial.tig", "r");
    yyin = myfile;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    const ProgramTree *ir = convert_ast(output);
    std::cout << output->toStr() << std::endl;
    std::cout << ir->toStr() << std::endl;

    const ProgramFragment *fr = ir->vectorize();
    std::cout << "\n== ASM ==\n" << std::endl;
    print_instrs(fr->munch());
    std::cout << "\n";

    delete output;
    fclose(myfile);
}

TEST_CASE("check division", "[ir-conversion]") {
    FILE *myfile = fopen("test_ir/division.tig", "r");
    yyin = myfile;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    const ProgramTree *ir = convert_ast(output);
    std::cout << output->toStr() << std::endl;
    std::cout << ir->toStr() << std::endl;

    const ProgramFragment *fr = ir->vectorize();
    std::cout << "\n== ASM ==\n" << std::endl;
    print_instrs(fr->munch());
    std::cout << "\n";

    delete output;
    fclose(myfile);
}

