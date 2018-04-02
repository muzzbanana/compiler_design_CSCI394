#include "ast.hh"

namespace tiger {

const Type *IfThenElse::type_verify(Scope* scope, ASTNode::ASTptr left_, ASTNode::ASTptr middle_, ASTNode::ASTptr right_, int location_) {
    const Type *cond_type = left_->type_verify(scope);
    const Type *then_type = middle_->type_verify(scope);
    const Type *else_type = right_->type_verify(scope);
    if (cond_type == Type::intType && then_type == else_type && then_type != Type::errorType) {
        return then_type;
    } else if (cond_type == Type::errorType) {
        /* there was an error in the condition -- just propagate upward */
        return Type::errorType;
    } else if (cond_type != Type::intType) {
        cerr << "ERROR: line " << location_ << endl;
        cerr << "       condition in 'if' statement must evaluate to integer" << endl;
        cerr << "       (evaluates to ‘" << cond_type->toStr() << "’ instead)" << endl;
        return Type::errorType;
    } else if (then_type == Type::errorType || else_type == Type::errorType) {
        /* error was somewhere below us, just pass it on */
        return Type::errorType;
    } else if (then_type != else_type) {
        cerr << "ERROR: line " << location_ << endl;
        cerr << "       true and false condition expressions in 'if' statement must have the same type" << endl;
        cerr << "       (‘" << middle_->toStr() << "’ is of type ‘" << then_type->toStr() << "’ and ‘"
             << right_->toStr() << "’ is of type ‘"<< else_type->toStr() << "’)" << endl;
        return Type::errorType;
    } else {
        return Type::errorType;
    }
}

const Type *WhileDo::type_verify(Scope* scope, ASTNode::ASTptr left_, ASTNode::ASTptr right_, int location_) {
    const Type *cond_type = left_->type_verify(scope);
    const Type *do_type = right_->type_verify(scope);

    if (cond_type == Type::intType && do_type != Type::errorType) {
        return Type::nilType;
    } else if (cond_type != Type::intType) {
        cerr << "ERROR: line " << location_ << endl;
        cerr << "       condition of 'while' loop must evaluate to integer" << endl;
        return Type::errorType;
    } else {
        return Type::errorType;
    }
}

const Type *ForTo::type_verify(Scope* scope, ASTNode::ASTptr one_, ASTNode::ASTptr two_,
        ASTNode::ASTptr three_, ASTNode::ASTptr four_, int location_) {
    const Type *first_type = two_->type_verify(scope);
    const Type *last_type = three_->type_verify(scope);
    const Type *body_type = four_->type_verify(scope);
    if (first_type == Type::intType && last_type == Type::intType
            && body_type != Type::errorType) {
        return Type::nilType;
    } else if (first_type != Type::intType || last_type != Type::intType) {
        cerr << "ERROR: line " << location_ << endl;
        cerr << "       'from' and 'to' expressions in 'for' loop must be integer expressions" << endl;
        return Type::errorType;
    } else {
        return Type::errorType;
    }
}

}//namespace
