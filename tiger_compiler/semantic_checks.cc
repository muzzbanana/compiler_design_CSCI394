#include "semantic_checks.hh"

namespace tiger {

/* We can't use ASTptr here because it is inside ASTNode and that is not good because
 * we need to include scope.hh, which includes ast.hh, which includes this file. */
int semantic_checks(ASTNode::ASTptr ast) {
    Scope *scope = new Scope();
    const Type *t = ast->type_verify(scope);

    if (t == Type::errorType) {
        return 1;
    } else {
        return 0;
    }
}

} //namespace
