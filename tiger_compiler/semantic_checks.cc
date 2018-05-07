#include "semantic_checks.hh"

namespace tiger {

/* We can't use ASTptr here because it is inside ASTNode and that is not good because
 * we need to include scope.hh, which includes ast.hh, which includes this file. */
int semantic_checks(ASTNode::ASTptr ast) {
    Scope *scope = new Scope();

    scope->push_scope();

    /* Set up print and print_int here, so we can typecheck on them */
    RecordType *print_params = new RecordType();
    print_params->add_field("what", Type::stringType);
    FunctionType *print_type = new FunctionType("print", print_params, Type::nilType);
    scope->symbol_insert("print", print_type);

    RecordType *print_int_params = new RecordType();
    print_int_params->add_field("what", Type::intType);
    FunctionType *print_int_type = new FunctionType("print_int", print_int_params, Type::nilType);
    scope->symbol_insert("print_int", print_int_type);

    const Type *t = ast->type_verify(scope);

    if (t == Type::errorType) {
        return 1;
    } else {
        return 0;
    }
}

} //namespace
