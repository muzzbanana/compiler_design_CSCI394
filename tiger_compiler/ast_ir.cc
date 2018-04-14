#include "ast.hh"

namespace tiger {

const StmtTree *Assignment::convert_to_ir(Frame *frame, ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
    const IRTree *left = left_->convert_to_ir(frame);
    const IRTree *right = right_->convert_to_ir(frame);
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

const StmtTree *IfThenElse::convert_to_ir(Frame *frame, ASTNode::ASTptr left_, ASTNode::ASTptr middle_, ASTNode::ASTptr right_) {
    /* We evaluate the conditional expression and then just create
     * a CJumpnode that checks whether it's equal to 0. */
    Label *trueLabel = new Label("true");
    Label *falseLabel = new Label("false");
    Label *afterLabel = new Label("after");

    /* Convert to something like:
     *      MOVE tmp1, <left>
     *      JNE  tmp1, 0, t, f
     * .t:  <middle>
     *      JMP  after
     * .f:  <right>
     * .after:
     * */
    const IRTree *condTree = left_->convert_to_ir(frame);

    /* Need to pass an expression to CJumpTree -- we know it has to be an expression
     * because of our position in the AST (can't put a statement inside the condition
     * of an if...) but need to convert it */
    assert(condTree->isExpr());
    const ExprTree *conditional = dynamic_cast<const ExprTree*>(condTree);

    /* Then the true and false things need to be examined -- if they're exprs,
     * they need to be wrapped in an ExprStmtTree; otherwise they can just be passed
     * along as stmts. */
    const IRTree *trueTree = middle_->convert_to_ir(frame);
    const StmtTree *trueStmt;
    if (trueTree->isExpr()) {
        trueStmt = new ExprStmtTree(dynamic_cast<const ExprTree*>(trueTree));
    } else {
        trueStmt = dynamic_cast<const StmtTree*>(trueTree);
    }

    const IRTree *falseTree = right_->convert_to_ir(frame);
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

const StmtTree *WhileDo::convert_to_ir(Frame *frame, ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
    Label *testLabel = new Label("test");
    Label *bodyLabel = new Label("body");
    Label *doneLabel = new Label("done");
    const IRTree *condTree = left_->convert_to_ir(frame);
    const ExprTree *conditional = dynamic_cast<const ExprTree*>(condTree);
    const IRTree *bodyTree = right_->convert_to_ir(frame);
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

const StmtTree *ForTo::convert_to_ir(Frame *frame, ASTNode::ASTptr one_, ASTNode::ASTptr two_,
        ASTNode::ASTptr three_, ASTNode::ASTptr four_) {
    Label *testLabel = new Label("test");
    Label *bodyLabel = new Label("body");
    Label *doneLabel = new Label("done");

    const IRTree *variable = one_->convert_to_ir(frame);
    assert(variable->isExpr());
    const IRTree *start_int = two_->convert_to_ir(frame);
    assert(start_int->isExpr());

    const ExprTree *limitExpr;
    const ExprTree *startExpr;
    const ExprTree *countExpr;

    countExpr = dynamic_cast<const ExprTree*>(variable);
    startExpr = dynamic_cast<const ExprTree*>(start_int);

    const StmtTree *initialize = new MoveTree(countExpr, startExpr);

    const IRTree *limit = three_->convert_to_ir(frame);
    assert(limit->isExpr());
    limitExpr = dynamic_cast<const ExprTree*>(limit);

    const ExprTree *conditional = new BinOpTree(IRTree::Operator::LT, countExpr, limitExpr);

    const IRTree *bodyTree = four_->convert_to_ir(frame);
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

const StmtTree *UntypedVarDeclaration::convert_to_ir(Frame *frame, ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
    const IRTree *lhs = left_->convert_to_ir(frame);
    const IRTree *rhs = right_->convert_to_ir(frame);

    const ExprTree *left;
    const ExprTree *right;

    if (lhs->isExpr()) {
        left = dynamic_cast<const ExprTree*>(lhs);
    } else {
        left = new StmtExprTree(dynamic_cast<const StmtTree*>(lhs));
    }

    if (rhs->isExpr()) {
        right = dynamic_cast<const ExprTree*>(rhs);
    } else {
        right = new StmtExprTree(dynamic_cast<const StmtTree*>(rhs));
    }
    return new MoveTree(left, right);
}

const ExprTree *LetBlock::convert_to_ir(Frame *frame, ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
    const IRTree *let_block = left_->convert_to_ir(frame);
    const IRTree *in_block = right_->convert_to_ir(frame);

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

const StmtTree *Declaration::convert_to_ir(Frame *frame, ASTNode::ASTptr child_) {
    const IRTree *stmt = child_->convert_to_ir(frame);
    const StmtTree *statement;
    if (stmt->isExpr()) {
        statement = new ExprStmtTree(dynamic_cast<const ExprTree*>(stmt));
    } else {
        statement = dynamic_cast<const StmtTree*>(stmt);
    }
    return statement;
}

const StmtTree *DeclList::convert_to_ir(Frame *frame, std::vector<const DeclarationASTNode*> vec_) {
    auto node = vec_.front();
    const IRTree *stmt = node->convert_to_ir(frame);
    const StmtTree *firstStmt;
    if (stmt->isExpr()) {
        firstStmt = new ExprStmtTree(dynamic_cast<const ExprTree*>(stmt));
    } else {
        firstStmt = dynamic_cast<const StmtTree*>(stmt);
    }
    if (vec_.size() == 1){
        // const StmtTree *nullStmt = dynamic_cast<const StmtTree*>(NULL);
        return new SeqTree(firstStmt,NULL);
    }
    vec_.erase(vec_.begin());

    return new SeqTree(firstStmt, this->convert_to_ir(frame, vec_));
}

const ExprTree *ExprSeq::convert_to_ir(Frame *frame, std::vector<const ASTNode*> vec_) {
    auto node = vec_.front();  
    const IRTree *statement = node->convert_to_ir(frame);
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

    return new ExprSeqTree(seqStmt, this->convert_to_ir(frame, vec_));
}

} //namespace
