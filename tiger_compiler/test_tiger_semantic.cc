#define CATCH_CONFIG_MAIN
#include "ast.hh"
#include "semantic.hh"
#include "semantic_checks.hh"
#include "catch.hpp"
#include "tiger.tab.h"

#include <limits.h>
#include <cstdlib>
#include <ctime>

using namespace tiger;

int yyparse(ASTNode::ASTptr *out);
extern FILE *yyin;
extern int yylineno;

TEST_CASE("check simple int type", "[type-evaluation]") {
    FILE *myfile = fopen("test_semantic/oneplusone.tig", "r");
    yyin = myfile;
    std::cout << "TEST: 1 + 1" << std::endl;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    Scope *s = new Scope();
    const Type *type = output->type_verify(s);
    REQUIRE(type == Type::intType);

    int check_result = semantic_checks(output);
    REQUIRE(check_result == 0);

    delete output;
    fclose(myfile);
}

TEST_CASE("check simple string type", "[type-evaluation]") {
    FILE *myfile = fopen("test_semantic/string_test.tig", "r");
    yyin = myfile;
    std::cout << "TEST: Just checking strings" << std::endl;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    Scope *s = new Scope();
    const Type *type = output->type_verify(s);
    REQUIRE(type == Type::stringType);

    int check_result = semantic_checks(output);
    REQUIRE(check_result == 0);

    delete output;
    fclose(myfile);
}

TEST_CASE("check exprseq type", "[type-evaluation]") {
    FILE *myfile = fopen("test_semantic/sequence_type_test.tig", "r");
    yyin = myfile;
    std::cout << "TEST: Return type for sequence type" << std::endl;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    Scope *s = new Scope();
    const Type *type = output->type_verify(s);
    REQUIRE(type == Type::intType);

    int check_result = semantic_checks(output);
    REQUIRE(check_result == 0);

    delete output;
    fclose(myfile);
}

TEST_CASE("fail on basic type inconsistency (string + int)", "[semantic-check]") {
    cout << "== STRING PLUS INT ==" << endl;
    FILE *myfile = fopen("test_semantic/string_plus_int.tig", "r");
    yyin = myfile;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    Scope *s = new Scope();
    const Type *type = output->type_verify(s);
    REQUIRE(type == Type::errorType);

    int check_result = semantic_checks(output);
    REQUIRE(check_result != 0);

    delete output;
    fclose(myfile);
}

TEST_CASE("fail on basic inconsistency then <int> else <string>", "[semantic-check]") {
    cout << "== STRING PLUS INT ==" << endl;
    FILE *myfile = fopen("test_semantic/ifthenelse_type_mismatch.tig", "r");
    yyin = myfile;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    Scope *s = new Scope();
    const Type *type = output->type_verify(s);
    REQUIRE(type == Type::errorType);

    int check_result = semantic_checks(output);
    REQUIRE(check_result != 0);

    delete output;
    fclose(myfile);
}

TEST_CASE("fail on type inconsistency within expr seq", "[semantic-check]") {
    cout << "== ERROR INSIDE EXPRSEQ ==" << endl;
    FILE *myfile = fopen("test_semantic/error_inside_sequence.tig", "r");
    yyin = myfile;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    Scope *s = new Scope();
    const Type *type = output->type_verify(s);
    REQUIRE(type == Type::errorType);

    int check_result = semantic_checks(output);
    REQUIRE(check_result != 0);

    delete output;
    fclose(myfile);
}

TEST_CASE("fail on body of while loop error", "[semantic-check]") {
    cout << "== ERROR INSIDE WHILE ==" << endl;
    FILE *myfile = fopen("test_semantic/while_mistake.tig", "r");
    yyin = myfile;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    Scope *s = new Scope();
    const Type *type = output->type_verify(s);
    REQUIRE(type == Type::errorType);

    int check_result = semantic_checks(output);
    REQUIRE(check_result != 0);

    delete output;
    fclose(myfile);
}


TEST_CASE("fail on bad record type", "[semantic-check]") {
    cout << "== ERROR ON RECORD TYPE ==" << endl;
    FILE *myfile = fopen("test_semantic/bad_record_type.tig", "r");
    yyin = myfile;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    int check_result = semantic_checks(output);
    REQUIRE(check_result != 0);

    delete output;
    fclose(myfile);
}

TEST_CASE("check the use of dot for record types", "[semantic-check]") {
    cout << "== TESTING DOT ==" << endl;
    FILE *myfile = fopen("test_semantic/dot_for_records.tig", "r");
    yyin = myfile;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    int check_result = semantic_checks(output);
    REQUIRE(check_result != 0);

    delete output;
    fclose(myfile);
}


TEST_CASE("fail on expression type mismatch", "[semantic-check]") {
    cout << "== EXPR TYPE MISMATCH ==" << endl;
    FILE *myfile = fopen("test_semantic/expr_type_mismatch.tig", "r");
    yyin = myfile;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    int check_result = semantic_checks(output);
    REQUIRE(check_result != 0);

    delete output;
    fclose(myfile);
}

TEST_CASE("fail on function expression not matching declared return type", "[semantic-check]") {
    cout << "== FUNC WRONG TYPE DECL ==" << endl;
    FILE *myfile = fopen("test_semantic/func_wrong_type_decl.tig", "r");
    yyin = myfile;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    int check_result = semantic_checks(output);
    REQUIRE(check_result != 0);

    delete output;
    fclose(myfile);
}

TEST_CASE("fail on function returning fake return type", "[semantic-check]") {
    cout << "== NONEXISTENT RETURN ==" << endl;
    FILE *myfile = fopen("test_semantic/func_nonexistent_return.tig", "r");
    yyin = myfile;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    int check_result = semantic_checks(output);
    REQUIRE(check_result != 0);

    delete output;
    fclose(myfile);
}

TEST_CASE("fail on incorrect nesting", "[semantic-check]") {
    cout << "== INCORRECT NESTING ==" << endl;
    FILE *myfile = fopen("test_semantic/incorrect_nesting.tig", "r");
    yyin = myfile;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    int check_result = semantic_checks(output);
    REQUIRE(check_result != 0);

    delete output;
    fclose(myfile);
}

TEST_CASE("fail on mutually recursive function type mismatch", "[semantic-check]") {
    cout << "== MUTUAL RECURSIVE MISMATCH ==" << endl;
    FILE *myfile = fopen("test_semantic/mutual_recursive_mismatch.tig", "r");
    yyin = myfile;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    int check_result = semantic_checks(output);
    REQUIRE(check_result != 0);

    delete output;
    fclose(myfile);
}

TEST_CASE("fail on recursive argument type mismatch", "[semantic-check]") {
    cout << "== RECURSIVE ARGUMENT TYPE MISMATCH ==" << endl;
    FILE *myfile = fopen("test_semantic/recursive_arg_mismatch.tig", "r");
    yyin = myfile;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    int check_result = semantic_checks(output);
    REQUIRE(check_result != 0);

    delete output;
    fclose(myfile);
}

TEST_CASE("fail on recursive return type mismatch", "[semantic-check]") {
    cout << "== RECURSIVE RETURN TYPE MISMATCH ==" << endl;
    FILE *myfile = fopen("test_semantic/recursive_func_mismatch.tig", "r");
    yyin = myfile;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    int check_result = semantic_checks(output);
    REQUIRE(check_result != 0);

    delete output;
    fclose(myfile);
}

TEST_CASE("fail on repeated definition of var", "[semantic-check]") {
    cout << "== REPEATED DEFINITION OF VAR ==" << endl;
    FILE *myfile = fopen("test_semantic/repeat_definition.tig", "r");
    yyin = myfile;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    int check_result = semantic_checks(output);
    REQUIRE(check_result != 0);

    delete output;
    fclose(myfile);
}

TEST_CASE("fail on use of undefined identifier", "[semantic-check]") {
    cout << "== UNDEFINED IDENTIFIER ==" << endl;
    FILE *myfile = fopen("test_semantic/undefined_identifier.tig", "r");
    yyin = myfile;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    int check_result = semantic_checks(output);
    REQUIRE(check_result != 0);

    delete output;
    fclose(myfile);
}

TEST_CASE("fail on array index out of range", "[semantic-check]") {
    cout << "== ARRAY INDEX OUT OF RANGE ==" << endl;
    FILE *myfile = fopen("test_semantic/array_index_out_of_range.tig", "r");
    yyin = myfile;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    int check_result = semantic_checks(output);
    REQUIRE(check_result != 0);

    delete output;
    fclose(myfile);
}

TEST_CASE("fail on invalid type for array", "[semantic-check]") {
    cout << "== INVALID TYPE FOR ARRAY ==" << endl;
    FILE *myfile = fopen("test_semantic/invalid_type_for_array.tig", "r");
    yyin = myfile;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    int check_result = semantic_checks(output);
    REQUIRE(check_result != 0);

    delete output;
    fclose(myfile);
}

TEST_CASE("fail on indexing array with non-integer", "[semantic-check]") {
    cout << "== NON-INTEGER ARRAY INDEX ==" << endl;
    FILE *myfile = fopen("test_semantic/non_int_array_index.tig", "r");
    yyin = myfile;
    yylineno = 1;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    int check_result = semantic_checks(output);
    REQUIRE(check_result != 0);

    delete output;
    fclose(myfile);
}

TEST_CASE("fail on array default not matching array type", "[semantic-check]") {
    cout << "== ARRAY DEFAULT MISMATCH ==" << endl;
    FILE *myfile = fopen("test_semantic/wrong_array_default.tig", "r");
    yyin = myfile;
    yylineno = 1;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    int check_result = semantic_checks(output);
    REQUIRE(check_result != 0);

    delete output;
    fclose(myfile);
}

TEST_CASE("fail on repetition of function arguments", "[semantic-check]") {
    cout << "== REPETITION OF FUNCTION ARGUMENTS ==" << endl;
    FILE *myfile = fopen("test_semantic/repitition_of_func_args.tig", "r");
    yyin = myfile;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);
    Scope *s = new Scope();
    const Type *type = output->type_verify(s);
    REQUIRE(type == Type::errorType);

    int check_result = semantic_checks(output);

    REQUIRE(check_result != 0);

    delete output;
    fclose(myfile);
}

TEST_CASE("DON'T fail on using the same name in different scope", "[semantic-check]") {
    cout << "== SAME NAME, DIFFERENT SCOPE ==" << endl;
    cout << "(should succeed)" << endl;
    FILE *myfile = fopen("test_semantic/same_name_diff_scope.tig", "r");
    yyin = myfile;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    int check_result = semantic_checks(output);
    REQUIRE(check_result != 0);

    delete output;
    fclose(myfile);
}

TEST_CASE("fail on declaring a variable with the wrong type", "[semantic-check]") {
    cout << "== DECLARING VARIABLE WRONG TYPE ==" << endl;
    FILE *myfile = fopen("test_semantic/var_declare_wrong_type.tig", "r");
    yyin = myfile;
    yylineno = 1;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    int check_result = semantic_checks(output);
    REQUIRE(check_result != 0);

    delete output;
    fclose(myfile);
}

TEST_CASE("fail on assigning a variable the wrong type", "[semantic-check]") {
    cout << "== ASSIGNING VARIABLE WRONG TYPE ==" << endl;
    FILE *myfile = fopen("test_semantic/var_assign_wrong_type.tig", "r");
    yyin = myfile;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    int check_result = semantic_checks(output);
    REQUIRE(check_result != 0);

    delete output;
    fclose(myfile);
}



// this one is just a syntax error, not a semantic error.
/* TEST_CASE("fail on incorrect use of reserved words", "[semantic-check]") {
    FILE *myfile = fopen("test_semantic/reserved_words.tig", "r");
    yyin = myfile;
    yylineno = 1;
    ASTNode::ASTptr output = NULL;
    yyparse(&output);

    REQUIRE(output == NULL);

    delete output;
    fclose(myfile);
} */
