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

const Type *UntypedVarDeclaration::type_verify(Scope* scope, ASTNode::ASTptr left_, ASTNode::ASTptr right_, int location_) {
    string var_name = left_->toStr();
    const Type *expr_type = right_->type_verify(scope);

    if (scope->preexisting(var_name)) {
        cerr << "ERROR: line " << location_ << endl;
        cerr << "       cannot redeclare variable ‘" << var_name << "’ in the same scope" << endl;

        return Type::errorType;
    }

    scope->symbol_insert(var_name, expr_type);
    return Type::nilType;
}

const Type *TypedVarDeclaration::type_verify(Scope* scope, ASTNode::ASTptr left_, ASTNode::ASTptr middle_, ASTNode::ASTptr right_, int location_) {
    std::string var_name = left_->toStr();
    std::string type_name = middle_->toStr();

    if (scope->preexisting(var_name)) {
        cerr << "ERROR: line " << location_ << endl;
        cerr << "       cannot redeclare variable ‘" << var_name << "’ in the same scope" << endl;
        return Type::errorType;
    }

    const Type *value_type = right_->type_verify(scope);

    const Type *var_type = scope->type_search(type_name);
    if (var_type == Type::notFoundType) {
        cerr << "ERROR: line " << location_ << endl;
        cerr << "       variable ‘" << var_name << "’ declared as unknown type ‘" << type_name << "’" << endl;
        return Type::errorType;
    }

    if (value_type->equivalent(var_type)) {
        scope->symbol_insert(var_name, var_type);

        return Type::nilType;
    } else {
        cerr << "ERROR: line " << location_ << endl;
        cerr << "       variable ‘" << left_->toStr() << "’ declared as type ‘" <<
            var_type->toStr() << "’ but is being assigned type ‘" << value_type->toStr() << "’" << endl;
        return Type::errorType;
    }
}

const Type *TypeDeclaration::type_verify(Scope* scope, ASTNode::ASTptr left_, ASTNode::ASTptr right_, int location_) {
    string type_name = left_->toStr();
    const Type *new_type = right_->type_verify(scope);

    if (new_type->getKind() == tiger_type::RECORD) {
        /* the record type wants to know the name it's assigned to, but these
         * types are const, so we create a new one and delete the old one. */
        RecordType *new_rec = new RecordType(type_name, static_cast<const RecordType*>(new_type));
        delete static_cast<const RecordType*>(new_type);
        new_type = new_rec;
    }

    scope->type_insert(type_name, new_type);

    if (new_type == Type::errorType) {
        return Type::errorType;
    } else {
        return Type::nilType;
    }
}

const Type *LetBlock::type_verify(Scope* scope, ASTNode::ASTptr left_, ASTNode::ASTptr right_, int location_) {
    // Create a new scope for the duration of the declaration section and body
    // section.
    scope->push_scope();
    const Type *declaration_type = left_->type_verify(scope);
    const Type *body_type;
    if (declaration_type != Type::errorType) {
        body_type = right_->type_verify(scope);
    } else {
        body_type = Type::errorType;
    }
    scope->pop_scope();

    if (declaration_type != Type::errorType && body_type != Type::errorType) {
        return body_type;
    } else {
        return Type::errorType;
    }
}

const Type *Declaration::type_verify(Scope* scope, ASTNode::ASTptr child_, int location_) {
    return child_->type_verify(scope);
}

const Type *DeclList::type_verify(Scope* scope, std::vector<const DeclarationASTNode*> vec_, int location_) {
    for (auto decl : vec_) {
        const Type *t = decl->type_verify(scope);
        if (t == Type::errorType) {
            return Type::errorType;
        }
    }
    return Type::nilType;
}

const Type *ExprSeq::type_verify(Scope* scope, std::vector<const ASTNode*> vec_, int location_) {
    /* Find out whether we're checking if these match function parameters,
     * or if we're just evaluating them. */
    /* (We used the same AST node type for both.) */
    const Type *check_type = scope->search("_checked_params");

    if (check_type == Type::notFoundType) {
        /* It's just a sequence of expressions to evaluate
         * (semicolons) */
        const Type *return_type = Type::nilType;
        for (auto node : vec_) {
            return_type = node->type_verify(scope);
            if (return_type == Type::errorType) {
                return Type::errorType;
            }
        }
        return return_type;
    } else {
        /* It's a set of function parameters (commas) */
        if (check_type->getKind() != tiger_type::RECORD) {
            // this should only be passed in by the function call checker
            cerr << "how did you get here!" << endl;
            return Type::errorType;
        }

        const RecordType *params = static_cast<const RecordType*>(check_type);

        const Type *func_name = scope->search("_checked_funcname");

        if (func_name->getKind() != tiger_type::RECORD) {
            // again we should only end up here from the function call checker
            cerr << "how did you get here!!" << endl;
            return Type::errorType;
        }

        if (vec_.size() > params->fields_.size()) {
            cerr << "ERROR: line " << location_ << endl;
            cerr << "       too many parameters to function ‘" << func_name->toStr()
                << "’ (expected " << params->fields_.size() << ", got " << vec_.size() << ")" << endl;
            return Type::errorType;
        } else if (vec_.size() < params->fields_.size()) {
            cerr << "ERROR: line " << location_ << endl;
            cerr << "       not enough parameters to function ‘" << func_name->toStr()
                << "’ (expected " << params->fields_.size() << ", got " << vec_.size() << ")" << endl;
            return Type::errorType;
        }

        /* Now check that the types of the parameters match. */
        scope->push_scope();

        /* Hide _checked_params when we check types for the parameters */
        //scope->symbol_insert("_checked_params", Type::notFoundType);

        for (unsigned i = 0; i < vec_.size(); i++) {
            const Type *arg_type = vec_[i]->type_verify(scope);

            if (arg_type == Type::errorType) {
                return Type::errorType;
            }

            const Type *expected_type = params->fields_[i].second;

            if (arg_type != expected_type) {
                cerr << "ERROR: line " << location_ << endl;
                cerr << "       parameter ‘" << params->fields_[i].first << "’ to function ‘"
                    << func_name->toStr() << "’ declared as type ‘" << expected_type->toStr()
                    << "’ but received expression ‘" << vec_[i]->toStr() << "’, which is of type ‘"
                    << arg_type->toStr() << "’." << endl;
                return Type::errorType;
            }
        }

        scope->pop_scope();

        return Type::nilType;
    }
}

const Type *FieldMember::type_verify(Scope* scope, ASTNode::ASTptr left_, ASTNode::ASTptr right_, int location_) {
    return right_->type_verify(scope);
}

const Type *FieldList::type_verify(Scope* scope, std::vector<const FieldMemberASTNode*> vec_, int location_) {
    /* Similar to the declaration, we parse the toStr's to get
     * the field names and make sure there are no duplicates. */
    set<string> string_set;

    /* Get the currently-instantiating record type from the TypeInstantiation node
     * above us. */
    const Type *insttype = scope->type_search("_current_rectype");

    if (insttype->getKind() != tiger_type::RECORD) {
        cerr << "ERROR: line " << location_ << endl;
        cerr << "       cannot instantiate non-record type ‘" << insttype->toStr() << "’" << endl;
        return Type::errorType;
    }

    if (insttype == Type::notFoundType) {
        /* we should only be able to get here from a TypeInstantiation calling us
         * with this thing set in the scope */
        cerr << "how did you get here???" << endl;
        return Type::errorType;
    }

    const RecordType *rectype = static_cast<const RecordType*>(insttype);

    /* Keep track of which fields have yet to be declared, so we can report when
     * they are absent. */
    set<string> undeclared;
    for (unsigned int i = 0; i < rectype->fields_.size(); i++) {
        undeclared.insert(rectype->fields_[i].first);
    }

    for (unsigned int i = 0; i < vec_.size(); i++) {
        string s = vec_[i]->toStr();
        string t = s.substr(0,s.find("="));
        t.erase(remove(t.begin(), t.end(), ' '), t.end());
        t.erase(remove(t.begin(), t.end(), '('), t.end());

        const Type *field_type = rectype->field_type(t);

        if (field_type == Type::notFoundType) {
            cerr << "ERROR: line " << location_ << endl;
            cerr << "       type ‘" << rectype->toStr() << "’ has no field ‘" << t << "’" << endl;
            return Type::errorType;
        }

        if (string_set.count(t) > 0) {
            cerr << "ERROR: line " << location_ << endl;
            cerr << "       field ‘" << t << "’ initialized multiple times in instantiation of record type ‘"
                << rectype->toStr() << "’" << endl;
            return Type::errorType;
        }

        const Type *value_type = vec_[i]->type_verify(scope);
        if (value_type != field_type) {
            cerr << "ERROR: line " << location_ << endl;
            cerr << "       in expression ‘" << vec_[i]->toStr() << "’" << endl;
            cerr << "       field ‘" << t << "’ of record type ‘" << rectype->toStr() << "’ declared as type ‘"
                << field_type->toStr() << "’, but is being assigned a value of type ‘"
                << value_type->toStr() << "’" << endl;
            return Type::errorType;
        }

        undeclared.erase(t);

        string_set.insert(t);
    }

    if (undeclared.size() > 0) {
        cerr << "ERROR: line " << location_ << endl;
        if (undeclared.size() == 1) {
            cerr << "       no value given for field ‘" << *(undeclared.begin())
                << "’ of record type ‘" << rectype->toStr() << "’" << endl;
        } else if (undeclared.size() == 2) {
            cerr << "       no value given for field ‘" << *(undeclared.begin())
                << "’ or ‘" << *(next(undeclared.begin()))
                << "’ of record type ‘" << rectype->toStr() << "’" << endl;
        } else {
            cerr << "       no value given for field ";
            for (auto it = undeclared.begin(); it != undeclared.end(); it++) {
                cerr << "‘" << *it << "’";
                if (next(it) == undeclared.end()) {
                    /* at end, don't do anything */
                } else if (next(next(it)) == undeclared.end()) {
                    cerr << ", or ";
                } else {
                    cerr << ", ";
                }
            }
            cerr << " of record type ‘" << rectype->toStr() << "’" << endl;
        }
        return Type::errorType;
    }

    return rectype;
}

const Type *TypeInstantiation::type_verify(Scope* scope, ASTNode::ASTptr left_, ASTNode::ASTptr right_, int location_) {
    const Type *instantiating_type = scope->type_search(left_->toStr());

    if (instantiating_type == Type::notFoundType) {
        cerr << "ERROR: line " << location_ << endl;
        cerr << "       cannot instantiate nonexistent record type ‘" << left_->toStr() << "’" << endl;
        return Type::errorType;
    }

    scope->push_scope();

    /* We do this to pass the attempted instantiating type to the child node,
     * so that it can check whether the fields are valid. */
    scope->type_insert("_current_rectype", instantiating_type);

    const Type *instantiated_type = right_->type_verify(scope);

    scope->pop_scope();

    if (instantiated_type == Type::errorType) {
        return Type::errorType;
    }

    return instantiated_type;
}

const Type *TypeValue::type_verify(Scope* scope, ASTNode::ASTptr child_, int location_) {
    return child_->type_verify(scope);
}

const Type *RecordField::type_verify(Scope* scope, ASTNode::ASTptr left_, ASTNode::ASTptr right_, int location_) {
    return scope->type_search(right_->toStr());
}

const Type *RecordTypeAST::type_verify(Scope* scope, std::vector<const RecordFieldASTNode*> vec_, int location_) {
    /* dealing with multiple declerations (function arguments)
       using sets allows us to check the count on a given variable
       name, if there are more duplicates, return errorType */
    set<string> string_set;

    RecordType *rec = new RecordType();

    for (unsigned i = 0; i < vec_.size(); i++){
        string s = vec_[i]->toStr();
        string t = s.substr(0,s.find(":"));
        t.erase(remove(t.begin(), t.end(), ' '), t.end());

        if (string_set.count(t) > 0) {
            cerr << "ERROR: line " << location_ << endl;
            cerr << "       name ‘" << t << "’ used multiple times in function or record declaration" << endl;
            delete rec;
            return Type::errorType;
        }

        string_set.insert(t);

        const Type *field_type = vec_[i]->type_verify(scope);

        if (field_type == Type::notFoundType) {
            cerr << "ERROR: line " << location_ << endl;
            cerr << "       record field or function parameter ‘" << t << "’ declared as nonexistent type ‘"
                << vec_[i]->toStr() << "’" << endl;
            delete rec;
            return Type::errorType;
        }

        rec->add_field(t, field_type);
    }

    return rec;
}

const Type *ArrayTypeAST::type_verify(Scope* scope, ASTNode::ASTptr child_, int location_) {
    string type_name = child_->toStr();
    const Type *arrayof = scope->type_search(type_name);

    if (arrayof == Type::notFoundType) {
        cerr << "ERROR: line " << location_ << endl;
        cerr << "       cannot create array of nonexistent type ‘" << type_name << "’" << endl;
        return Type::errorType;
    }

    return new ArrayType(arrayof);
}

const Type *DotAccess::type_verify(Scope* scope, ASTNode::ASTptr left_, ASTNode::ASTptr right_, int location_) {
    const Type *accessed_type = left_->type_verify(scope);
    if (accessed_type->getKind() != tiger_type::RECORD) {
        cerr << "ERROR: line " << location_ << endl;
        cerr << "       attempt to access field ‘" << right_->toStr() << "’ of expression ‘"
            << left_->toStr() << "’, which is of non-record type ‘" << accessed_type->toStr() << "’" << endl;
        return Type::errorType;
    }

    const RecordType *rectype = static_cast<const RecordType*>(accessed_type);

    if (rectype->field_type(right_->toStr()) == Type::notFoundType) {
        cerr << "ERROR: line " << location_ << endl;
        cerr << "       in expression ‘" << left_->toStr() << "." << right_->toStr() << "’" << endl;
        cerr << "       ‘" << left_->toStr() << "’ is of type ‘" << rectype->toStr() <<
            "’, which has no field ‘" << right_->toStr() << "’" << endl;
        return Type::errorType;
    }

    return rectype->field_type(right_->toStr());
}

const Type *IndexAccess::type_verify(Scope* scope, ASTNode::ASTptr left_, ASTNode::ASTptr right_, int location_) {
    const Type *left_type = left_->type_verify(scope);
    if (left_type->getKind() != tiger_type::ARRAY) {
        cerr << "ERROR: line " << location_ << endl;
        cerr << "       attempt to index into ‘" << left_->toStr() << "’, which is not an array" << endl;
        cerr << "       (it is of type ‘" << left_type->toStr() << "’.)" << endl;
        return Type::errorType;
    }

    const Type *index_type = right_->type_verify(scope);
    if (index_type != Type::intType) {
        cerr << "ERROR: line " << location_ << endl;
        cerr << "       index of the array must be an integer expression" << endl;
        cerr << "       (got ‘" << right_->toStr() << "’, which is of type ‘" << index_type->toStr() << "’)" << endl;

        return Type::errorType;
    } else {
        // We know it's an ArrayType, because if it wasn't, we would have thrown
        // it away up there.
        return static_cast<const ArrayType*>(left_type)->type_of_;
    }
}

const Type *ArrayValue::type_verify(Scope* scope, ASTNode::ASTptr left_, ASTNode::ASTptr middle_, ASTNode::ASTptr right_, int location_) {
    string type_name = left_->toStr();

    const Type *length_type = middle_->type_verify(scope);

    if (length_type != Type::intType) {
        cerr << "ERROR: line " << location_ << endl;
        cerr << "       length of the array must be an integer expression" << endl;
        cerr << "       (got ‘" << middle_->toStr() << "’, which is of type ‘" << length_type->toStr() << "’." << endl;
        return Type::errorType;
    }

    const Type *arr_type = scope->type_search(type_name);

    const Type *value_type = right_->type_verify(scope);

    if (!arr_type->equivalent(value_type)) {
        cerr << "ERROR: line " << location_ << endl;
        cerr << "       declaring an array of values of type ‘" << type_name << "’, but default value given is ‘" <<
            right_->toStr() << "’, which is of type ‘" << value_type->toStr() << "’" << endl;
        return Type::errorType;
    }

    return new ArrayType(value_type);
}

const Type *UnTypedFuncDecl::type_verify(Scope* scope, ASTNode::ASTptr left_, ASTNode::ASTptr middle_, ASTNode::ASTptr right_, int location_) {
    string func_name = left_->toStr();

    if (scope->preexisting(func_name)) {
        cerr << "ERROR: line " << location_ << endl;
        cerr << "       cannot redeclare function ‘" << func_name << "’ in the same scope" << endl;
        return Type::errorType;
    }

    const Type *argtype = middle_->type_verify(scope);
    if (argtype == Type::errorType){
        return Type::errorType;
    }

    if (argtype->getKind() != tiger_type::RECORD) {
        /* this should have been a syntax error */
        cerr << "how did you get here" << endl;
        return Type::errorType;
    }

    const RecordType *arguments = static_cast<const RecordType*>(argtype);

    // add the parameters into the scope, so we can check them in function body

    scope->push_scope();

    /* We will just disallow calling functions recursively with no declared return type. */

    scope->symbol_insert(func_name, Type::incompleteRecursiveType);

    for (auto a : arguments->fields_) {
        scope->symbol_insert(a.first, a.second);
    }

    const Type *return_type = right_->type_verify(scope);

    scope->pop_scope();

    FunctionType *functype = new FunctionType(func_name, arguments, return_type);

    scope->symbol_insert(func_name, functype);

    return Type::nilType;
}

const Type *TypedFuncDecl::type_verify(Scope* scope, ASTNode::ASTptr name_node, ASTNode::ASTptr params_node, ASTNode::ASTptr rettype_node, ASTNode::ASTptr funcbody_node, int location_) {
    // one: func name
    // two: func params
    // three: func return type
    // four: func body
    string func_name = name_node->toStr();

    const Type *declared_func_type = scope->type_search(rettype_node->toStr());

    if (declared_func_type == Type::notFoundType) {
        cerr << "ERROR: line " << location_ << endl;
        cerr << "       function ‘" << func_name << "’ declared as returning nonexistent type ‘" <<
            rettype_node->toStr() << "’" << endl;
        return Type::errorType;
    }

    if (scope->preexisting(func_name)) {
        cerr << "ERROR: line " << location_ << endl;
        cerr << "       cannot redeclare function ‘" << func_name << "’ in the same scope" << endl;

        return Type::errorType;
    }

    const Type *argtype = params_node->type_verify(scope);
    if (argtype == Type::errorType){
        return Type::errorType;
    }

    if (argtype->getKind() != tiger_type::RECORD) {
        /* this should have been a syntax error */
        cerr << "how did you get here" << endl;
        return Type::errorType;
    }

    const RecordType *arguments = static_cast<const RecordType*>(argtype);

    // add the parameters into the scope, so we can check them in function body
    // also add the function type itself before we check the body, so that we can
    // account for recursive function calls

    FunctionType *functype = new FunctionType(func_name, arguments, declared_func_type);

    scope->symbol_insert(func_name, functype);

    scope->push_scope();

    for (auto a : arguments->fields_) {
        scope->symbol_insert(a.first, a.second);
    }

    const Type *return_type = funcbody_node->type_verify(scope);

    if (return_type == declared_func_type) {
        // we're good!
    } else if (return_type != Type::errorType && declared_func_type != Type::errorType) {
        cerr << "ERROR: line " << location_ << endl;
        cerr << "       function ‘" << func_name << "’ declared as returning ‘" << declared_func_type->toStr()
            << "’, but evaluates to ‘" << return_type->toStr() << "’" << endl;
        return Type::errorType;
    } else {
        return Type::errorType;
    }

    scope->pop_scope();

    return Type::nilType;
}

const Type *type_verify(Scope* scope, ASTNode::ASTptr left_, ASTNode::ASTptr right_, int location_) {
    string func_name = left_->toStr();

    const Type *var_type = scope->search(func_name);

    if (var_type == Type::notFoundType) {
        cerr << "ERROR: line " << location_ << endl;
        cerr << "       unknown function ‘" << func_name << "’" << endl;
        return Type::errorType;
    } else if (var_type == Type::incompleteRecursiveType) {
        cerr << "ERROR: line " << location_ << endl;
        cerr << "       cannot recursively call function ‘" << func_name
            << "’, which has no declared return type" << endl;
        return Type::errorType;
    } else if (var_type->getKind() != tiger_type::FUNCTION) {
        cerr << "ERROR: line " << location_ << endl;
        cerr << "       in expression ‘" << left_->toStr() << "(" << right_->toStr() << ")’" << endl;
        cerr << "       variable ‘" << left_->toStr() << "’ is not a function" << endl;
        return Type::errorType;
    }

    const FunctionType *func_type = static_cast<const FunctionType*>(var_type);

    const RecordType *arg_type = func_type->args_;

    const RecordType *dummy_func_name = new RecordType(func_name);

    scope->push_scope();

    /* Pass the checked parameters to the expression-sequence node. */
    scope->symbol_insert("_checked_params", arg_type);
    /* Pass the name of the current function to the expr-seq node, for better
     * error messages. */
    scope->symbol_insert("_checked_funcname", dummy_func_name);

    const Type *param_check_result = right_->type_verify(scope);

    if (param_check_result == Type::errorType) {
        delete dummy_func_name;
        scope->pop_scope();
        return Type::errorType;
    }

    scope->pop_scope();

    delete dummy_func_name;

    const Type *return_type = func_type->rettype_;

    return return_type;
}

}//namespace
