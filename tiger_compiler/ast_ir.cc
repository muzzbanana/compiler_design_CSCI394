#include "ast.hh"

namespace tiger {

/* Construct a new frame for global variables, and then call convert_to_ir on an AST pointer. */
/* Also places function definitions after the rest of the code. */
const ProgramTree *convert_ast(ASTNode::ASTptr ast) {
    vector<string> local_vars = ast->get_var_names();
    Frame *frame = new Frame();
    IRInfo *info = new IRInfo();

    info->frame_ = frame;

    vector<string> empty_args;
    info->frame_->pushFrame(empty_args, local_vars);

    const IRTree *main_ir = ast->convert_to_ir(info);

    const StmtTree *main_stmt = NULL;
    if (main_ir->isExpr()) {
        main_stmt = new ExprStmtTree(dynamic_cast<const ExprTree*>(main_ir));
    } else {
        main_stmt = dynamic_cast<const StmtTree*>(main_ir);
    }

    if (local_vars.size() > 0) {
        /* If we have any local variables, we need to reserve space for them! */
        main_stmt = new SeqTree(new NewFrameTree(local_vars.size()),
                    new SeqTree(main_stmt,
                    new SeqTree(new EndFrameTree, NULL)));
    }

    const SeqTree *func_decls = NULL;
    for (auto a : info->functions_) {
        func_decls = new SeqTree(a, func_decls);
    }

    const SeqTree *text = new SeqTree(new ReturnTree(new StmtExprTree(main_stmt)), func_decls);

    const SeqTree *data = NULL;
    for (auto a : info->static_strings_) {
        data = new SeqTree(new StaticStringTree(a.first, a.second), data);
    }

    ProgramTree *program = new ProgramTree();
    program->data_segment = data;
    program->text_segment = text;

    return program;
}

const StmtTree *Assignment::convert_to_ir(IRInfo *info, ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
    const IRTree *left = left_->convert_to_ir(info);
    const IRTree *right = right_->convert_to_ir(info);
    const ExprTree *leftExpr;
    const ExprTree *rightExpr;
    if (left->isExpr()) {
        leftExpr = dynamic_cast<const ExprTree*>(left);
    } else {
        leftExpr = new StmtExprTree(dynamic_cast<const StmtTree*>(left));
    }
    if (right->isExpr()) {
        rightExpr = dynamic_cast<const ExprTree*>(right);
    } else {
        rightExpr = new StmtExprTree(dynamic_cast<const StmtTree*>(right));
    }

    return new MoveTree(leftExpr, rightExpr);
}

const StmtTree *IfThenElse::convert_to_ir(IRInfo *info, ASTNode::ASTptr left_, ASTNode::ASTptr middle_, ASTNode::ASTptr right_) {
    /* We evaluate the conditional expression and then just create
     * a CJumpnode that checks whether it's equal to 0. */
    Label *trueLabel = new Label("true");
    Label *falseLabel = new Label("false");
    Label *afterLabel = new Label("after");

    /* Convert to something like:
     *      JNE [<left>], 0, t, f
     *  .t: <middle>
     *      JMP after
     *  .f: <right>
     *  .after: */
    /* When we vectorize everything, the JNE [<left>], 0, t, f
     * will convert to something like:
     *      MOV <left>, tmp1
     *      MOV 0, tmp2
     *      JNE tmp1, tmp2, t, f */
    const IRTree *condTree = left_->convert_to_ir(info);

    /* Need to pass an expression to CJumpTree -- we know it has to be an expression
     * because of our position in the AST (can't put a statement inside the condition
     * of an if...) but need to convert it */
    assert(condTree->isExpr());
    const ExprTree *conditional = dynamic_cast<const ExprTree*>(condTree);

    /* Then the true and false things need to be examined -- if they're exprs,
     * they need to be wrapped in an ExprStmtTree; otherwise they can just be passed
     * along as stmts. */
    const IRTree *trueTree = middle_->convert_to_ir(info);
    const StmtTree *trueStmt;
    if (trueTree->isExpr()) {
        trueStmt = new ExprStmtTree(dynamic_cast<const ExprTree*>(trueTree));
    } else {
        trueStmt = dynamic_cast<const StmtTree*>(trueTree);
    }

    const IRTree *falseTree = right_->convert_to_ir(info);
    const StmtTree *falseStmt;
    if (falseTree->isExpr()) {
        falseStmt = new ExprStmtTree(dynamic_cast<const ExprTree*>(falseTree));
    } else {
        falseStmt = dynamic_cast<const StmtTree*>(falseTree);
    }
    return new SeqTree(new CJumpTree(IRTree::Operator::NE,
                conditional, new ConstTree(0), trueLabel, falseLabel),
           new SeqTree(new LabelTree(trueLabel),
           new SeqTree(trueStmt,
           new SeqTree(new UJumpTree(afterLabel),
           new SeqTree(new LabelTree(falseLabel),
           new SeqTree(falseStmt,
           new SeqTree(new LabelTree(afterLabel),
                   NULL)))))));
}

const StmtTree *WhileDo::convert_to_ir(IRInfo *info, ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
    Label *testLabel = new Label("test");
    Label *bodyLabel = new Label("body");
    Label *doneLabel = new Label("done");
    const IRTree *condTree = left_->convert_to_ir(info);
    const ExprTree *conditional = dynamic_cast<const ExprTree*>(condTree);
    const IRTree *bodyTree = right_->convert_to_ir(info);
    const StmtTree *bodyStmt;
    if (bodyTree->isExpr()) {
        bodyStmt = new ExprStmtTree(dynamic_cast<const ExprTree*>(bodyTree));
    } else {
        bodyStmt = dynamic_cast<const StmtTree*>(bodyTree);
    }

    return new SeqTree(new LabelTree(testLabel),
           new SeqTree(new CJumpTree(IRTree::Operator::NE,
                conditional, new ConstTree(0), bodyLabel, doneLabel),
           new SeqTree(new LabelTree(bodyLabel),
           new SeqTree(bodyStmt,
           new SeqTree(new UJumpTree(testLabel),
           new SeqTree(new LabelTree(doneLabel), NULL))))));
}

const StmtTree *ForTo::convert_to_ir(IRInfo *info, ASTNode::ASTptr one_, ASTNode::ASTptr two_,
        ASTNode::ASTptr three_, ASTNode::ASTptr four_) {
    Label *testLabel = new Label("test");
    Label *bodyLabel = new Label("body");
    Label *doneLabel = new Label("done");

    const IRTree *variable = one_->convert_to_ir(info);
    assert(variable->isExpr());
    const IRTree *start_int = two_->convert_to_ir(info);
    assert(start_int->isExpr());

    const ExprTree *limitExpr;
    const ExprTree *startExpr;
    const ExprTree *countExpr;

    countExpr = dynamic_cast<const ExprTree*>(variable);
    startExpr = dynamic_cast<const ExprTree*>(start_int);

    const StmtTree *initialize = new MoveTree(countExpr, startExpr);

    const IRTree *limit = three_->convert_to_ir(info);
    assert(limit->isExpr());
    limitExpr = dynamic_cast<const ExprTree*>(limit);

    const ExprTree *conditional = new BinOpTree(IRTree::Operator::LT, countExpr, limitExpr);

    const IRTree *bodyTree = four_->convert_to_ir(info);
    const StmtTree *bodyStmt;
    if (bodyTree->isExpr()) {
        bodyStmt = new ExprStmtTree(dynamic_cast<const ExprTree*>(bodyTree));
    } else {
        bodyStmt = dynamic_cast<const StmtTree*>(bodyTree);
    }

    const ExprTree *plus = new BinOpTree(IRTree::Operator::PLUS, countExpr, new ConstTree(1));

    return new SeqTree(initialize,
            new SeqTree(new LabelTree(testLabel),
            new SeqTree(new CJumpTree(IRTree::Operator::NE,
                        conditional, new ConstTree(0), bodyLabel, doneLabel),
            new SeqTree(new LabelTree(bodyLabel),
            new SeqTree(bodyStmt,
            new SeqTree(new MoveTree(countExpr, plus),
            new SeqTree(new UJumpTree(testLabel),
            new SeqTree(new LabelTree(doneLabel), NULL))))))));
}

const StmtTree *UntypedVarDeclaration::convert_to_ir(IRInfo *info, ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
    const IRTree *lhs = left_->convert_to_ir(info);
    const IRTree *rhs = right_->convert_to_ir(info);

    assert(lhs != NULL);
    assert(rhs != NULL);
    assert(lhs->isExpr());
    assert(rhs->isExpr());

    const ExprTree *left = dynamic_cast<const ExprTree*>(lhs);
    const ExprTree *right = dynamic_cast<const ExprTree*>(rhs);

    return new MoveTree(left, right);
}

const StmtTree *TypedVarDeclaration::convert_to_ir(IRInfo *info, ASTNode::ASTptr left_, ASTNode::ASTptr middle_, ASTNode::ASTptr right_) {
    /* This is exactly the same as UntypedVarDeclaration,
     * because we don't care about the type at this point
     * (assuming it already got checked by semantic_checks) */
    const IRTree *lhs = left_->convert_to_ir(info);
    const IRTree *rhs = right_->convert_to_ir(info);

    assert(lhs != NULL);
    assert(rhs != NULL);
    assert(lhs->isExpr());
    assert(rhs->isExpr());

    const ExprTree *left = dynamic_cast<const ExprTree*>(lhs);
    const ExprTree *right = dynamic_cast<const ExprTree*>(rhs);

    return new MoveTree(left, right);
}

const StmtTree *UnTypedFuncDecl::convert_to_ir(IRInfo *info, ASTNode::ASTptr left_, ASTNode::ASTptr middle_, ASTNode::ASTptr right_) {
    /* Get the argument names out of the RecordTypeASTNode representing arguments. */
    /* (This is kind of hacky, but no less hacky than the string-parsing solution
     * we came up with last time :P ) */
    const RecordTypeASTNode *arglist = dynamic_cast<const RecordTypeASTNode *>(middle_);
    vector<string> argnames;
    for (ASTNode::ASTptr arg : arglist->vec_) {
        argnames.push_back(dynamic_cast<const RecordFieldASTNode *>(arg)->left_->toStr());
    }

    vector<string> localvars = right_->get_var_names();

    /* Push the new frame. */
    info->frame_->pushFrame(argnames, localvars);

    /* Figure out the IR for the inside of the function. */
    const IRTree *inner_func = right_->convert_to_ir(info);

    /* Convert it to a StmtTree*. */
    const StmtTree *inner_stmt = NULL;
    if (inner_func->isExpr()) {
        inner_stmt = new ExprStmtTree(dynamic_cast<const ExprTree*>(inner_func));
    } else {
        inner_stmt = dynamic_cast<const StmtTree *>(inner_stmt);
    }

    /* Pop the frame. */
    info->frame_->popFrame();

    Label *new_func_label = new Label(left_->toStr());
    info->func_labels_[left_->toStr()] = new_func_label;

    /* Note that we store the functions in a vector rather than interpolating
     * them directly into the generated fragments. This is so that we can put
     * the functions at the end of the output rather than having them scattered
     * throughout the code (which makes the code path through assembly kind
     * of unpredictable.) */
    info->functions_.push_back(
        new SeqTree(new LabelTree(new_func_label),
            new SeqTree(new ReturnTree(
                new StmtExprTree(
                    new SeqTree(new NewFrameTree(localvars.size()),
                    new SeqTree(inner_stmt,
                    new SeqTree(new EndFrameTree, NULL)))
                )
            ), NULL))
        );

    return NULL;
}

const ExprTree *LetBlock::convert_to_ir(IRInfo *info, ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
    const IRTree *let_block = left_->convert_to_ir(info);
    const IRTree *in_block = right_->convert_to_ir(info);

    const StmtTree *letStmt;
    if (let_block->isExpr()) {
        letStmt = new ExprStmtTree(dynamic_cast<const ExprTree*>(let_block));
    } else {
        letStmt = dynamic_cast<const StmtTree*>(let_block);
    }

    const ExprTree *inExpr;

    if (in_block->isExpr()) {
        inExpr = dynamic_cast<const ExprTree*>(in_block);
    } else {
        inExpr = new StmtExprTree(dynamic_cast<const StmtTree*>(in_block));
    }

    return new ExprSeqTree(letStmt,inExpr);
}

const StmtTree *Declaration::convert_to_ir(IRInfo *info, ASTNode::ASTptr child_) {
    const IRTree *stmt = child_->convert_to_ir(info);
    const StmtTree *statement;
    if (stmt == NULL) {
        /* If it returned null, it's something that doesn't want to put its
         * code here (so either a type declaration, which doesn't turn into
         * any generated code, or a function declaration, which will get moved
         * to the end. */
        return NULL;
    }

    if (stmt->isExpr()) {
        statement = new ExprStmtTree(dynamic_cast<const ExprTree*>(stmt));
    } else {
        statement = dynamic_cast<const StmtTree*>(stmt);
    }
    return statement;
}

const StmtTree *DeclList::convert_to_ir(IRInfo *info, std::vector<const DeclarationASTNode*> vec_) {
    auto node = vec_.front();
    const IRTree *stmt = node->convert_to_ir(info);
    const StmtTree *firstStmt;

    if (stmt == NULL) {
        firstStmt = NULL;
    } else if (stmt->isExpr()) {
        firstStmt = new ExprStmtTree(dynamic_cast<const ExprTree*>(stmt));
    } else {
        firstStmt = dynamic_cast<const StmtTree*>(stmt);
    }

    if (vec_.size() == 1){
        // const StmtTree *nullStmt = dynamic_cast<const StmtTree*>(NULL);
        if (firstStmt == NULL) {
            return NULL;
        } else {
            return new SeqTree(firstStmt,NULL);
        }
    }
    vec_.erase(vec_.begin());

    if (firstStmt == NULL) {
        return this->convert_to_ir(info, vec_);
    } else {
        return new SeqTree(firstStmt, this->convert_to_ir(info, vec_));
    }
}

const ExprTree *ExprSeq::convert_to_ir(IRInfo *info, std::vector<const ASTNode*> vec_) {
    auto node = vec_.front();
    const IRTree *statement = node->convert_to_ir(info);
    const StmtTree *seqStmt;

    if (statement->isExpr()) {
        seqStmt = new ExprStmtTree(dynamic_cast<const ExprTree*>(statement));
    } else {
        seqStmt = dynamic_cast<const StmtTree*>(statement);
    }

    if (vec_.size() == 1){
        return new ExprSeqTree(seqStmt,NULL);
    }
    vec_.erase(vec_.begin());

    return new ExprSeqTree(seqStmt, this->convert_to_ir(info, vec_));
}

const ExprTree *ArrayValue::convert_to_ir(IRInfo *info, ASTNode::ASTptr left_, ASTNode::ASTptr middle_, ASTNode::ASTptr right_) {
    return ExprTree::notImpl;
    // /*
    //     ignore this, i just wanted to print things and needed to return something so it does not
    //     crop up an error
    // */
    // const IRTree *array_name = left_->convert_to_ir(info);
    // const IRTree *length = middle_->convert_to_ir(info);
    // const IRTree *value = right_->convert_to_ir(info);
    // std::cout << array_name->toStr() << " " << length->toStr() << " " << value->toStr() << std::endl;
    // const StmtTree *leftExpr;
    // const StmtTree *middleExpr;
    // const StmtTree *rightExpr;

    // if (array_name->isExpr()) {
    //     leftExpr = new ExprStmtTree(dynamic_cast<const ExprTree*>(array_name));
    // } else {
    //     leftExpr = dynamic_cast<const StmtTree*>(array_name);
    // }
    // if (length->isExpr()) {
    //     middleExpr = new ExprStmtTree(dynamic_cast<const ExprTree*>(length));
    // } else {
    //     middleExpr = dynamic_cast<const StmtTree*>(length);
    // }
    // if (value->isExpr()) {
    //     rightExpr = new ExprStmtTree(dynamic_cast<const ExprTree*>(value));
    // } else {
    //     rightExpr = dynamic_cast<const StmtTree*>(value);
    // }
    // std::cout << leftExpr->toStr() << " " << middleExpr->toStr() << " " << rightExpr->toStr() << std::endl;

    // return new ExprSeqTree(leftExpr, (new ExprSeqTree(middleExpr, new ExprSeqTree(rightExpr, NULL))));
}

const ExprTree *DotAccess::convert_to_ir(IRInfo *info, ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
    auto right = right_->toStr();
    auto leftt = left_->precomputed_type_;
    assert(leftt != NULL);
    assert(leftt->getKind() == tiger_type::RECORD);
    const RecordType *rt = dynamic_cast<const RecordType*>(leftt);
    int index = 0;
    for (auto iter = rt->fields_.begin(); iter != rt->fields_.end(); ++iter){
        if (iter->first == right) {
            break;
        }
        index ++;
    }
    const IRTree *rec = left_->convert_to_ir(info);
    assert(rec->isExpr());
    return new BinOpTree(IRTree::Operator::PLUS, dynamic_cast<const ExprTree*>(rec),  new ConstTree(index*4));
}

const ExprTree *IndexAccess::convert_to_ir(IRInfo *info, ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
    /* this might be, like, array[x * 2 + 1] or something arbitrarily complex */
    auto index = right_->convert_to_ir(info);
    assert(index->isExpr());
    const ExprTree *indexExpr = dynamic_cast<const ExprTree*>(index);

    const IRTree *arr = left_->convert_to_ir(info);
    assert(arr->isExpr());
    const ExprTree *arrExpr = dynamic_cast<const ExprTree*>(arr);

    return new BinOpTree(IRTree::Operator::PLUS, arrExpr,  new BinOpTree(IRTree::Operator::MUL, indexExpr, new ConstTree(4)));
}

const ExprTree *FuncCall::convert_to_ir(IRInfo *info, ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
    /* Find the appropriate label for the function */
    string func_name = left_->toStr();
    Label *func_label = info->func_labels_[func_name];

    const ExprSeqASTNode *argsseq = dynamic_cast<const ExprSeqASTNode *>(right_);

    vector<const ExprTree*> args_expr_list;

    for (auto a : argsseq->vec_) {
        const IRTree *arg = a->convert_to_ir(info);
        assert(arg->isExpr());
        const ExprTree *argexpr = dynamic_cast<const ExprTree*>(arg);
        args_expr_list.push_back(argexpr);
    }

    return new CallTree(new NameTree(func_label), args_expr_list);
}

} //namespace
