#include "irtree.hh"
#include <iostream>

namespace tiger {

using tt = IRTree::TreeType;

const ExprTree *ExprTree::notImpl = new NotImplExprTree();
const StmtTree *StmtTree::notImpl = new NotImplStmtTree();

IRTree::IRTree(tt type) : type_(type) { }

ExprTree::ExprTree(tt type) : IRTree(type) { is_expr_ = true; }

StmtTree::StmtTree(tt type) : IRTree(type) { is_expr_ = false; }

/* expr trees */

StmtExprTree::StmtExprTree(const StmtTree *stmt) : ExprTree(tt::STMT), stmt_(stmt) { }

BinOpTree::BinOpTree(IRTree::Operator op, const ExprTree *left, const ExprTree *right)
    : ExprTree(tt::BINOP), op_(op), left_(left), right_(right) { }

CallTree::CallTree(NameTree *name, const ExprList args)
    : ExprTree(tt::CALL), name_(name), args_(args) { }

ConstTree::ConstTree(int value) : ExprTree(tt::CONST), value_(value) { }

ExprSeqTree::ExprSeqTree(const StmtTree *stmt, const ExprTree *expr)
    : ExprTree(tt::ESEQ), stmt_(stmt), expr_(expr) { }

MemTree::MemTree(const ExprTree *expr) : ExprTree(tt::MEM), expr_(expr) { }

NameTree::NameTree(Label *label) : ExprTree(tt::NAME), label_(label) { }

TempTree::TempTree(Temp *temp) : ExprTree(tt::TEMP), temp_(temp) { }

VarTree::VarTree(std::string name, int offset) : ExprTree(tt::VAR), name_(name), offset_(offset) { }

/* statement trees */

ExprStmtTree::ExprStmtTree(const ExprTree *expr) : StmtTree(tt::EXPR), expr_(expr) { }

CJumpTree::CJumpTree(IRTree::Operator comp, const ExprTree *left, const ExprTree *right, Label *t, Label *f)
    : StmtTree(tt::CJUMP), comp_(comp), left_(left), right_(right), t_(t), f_(f) { }

UJumpTree::UJumpTree(Label *label) : StmtTree(tt::UJUMP), label_(label) { }

ReturnTree::ReturnTree(const ExprTree *expr) : StmtTree(tt::RETURN), expr_(expr) { }

LabelTree::LabelTree(Label *l) : StmtTree(tt::LABEL), l_(l) { }

MoveTree::MoveTree(const ExprTree *dest, const ExprTree *src) : StmtTree(tt::MOVE), dest_(dest), src_(src) { }

SeqTree::SeqTree() : StmtTree(tt::SEQ), left_(NULL), right_(NULL) { }

SeqTree::SeqTree(const StmtTree *left, const StmtTree *right) : StmtTree(tt::SEQ), left_(left), right_(right) { }

/* vectorized tree */

VectorizedTree::VectorizedTree(Temp *result_temp) : IRTree(tt::VECTORIZED), result_temp_(result_temp) { }

void VectorizedTree::append(const StmtTree *stmt) {
    content_.push_back(stmt);
}

void VectorizedTree::concatenate(const VectorizedTree *vec) {
    if (vec == NULL) return;

    for (auto stmt : vec->content_) {
        append(stmt);
    }
}

/* vectorization functions */

/* expr trees */

VectorizedTree *StmtExprTree::vectorize() const {
    return stmt_->vectorize();
}

VectorizedTree *BinOpTree::vectorize() const {
    Temp *result = new Temp();
    VectorizedTree *vleft = left_->vectorize();
    VectorizedTree *vright = right_->vectorize();
    StmtTree *move = new MoveTree(new TempTree(result),
            new BinOpTree(op_, new TempTree(vleft->result_temp_), new TempTree(vright->result_temp_)));
    VectorizedTree *v = new VectorizedTree(result);
    v->concatenate(vleft);
    v->concatenate(vright);
    v->append(move);
    return v;
}

VectorizedTree *CallTree::vectorize() const {
    /* not implemented yet! */
    return NULL;
}

VectorizedTree *ConstTree::vectorize() const {
    Temp *result = new Temp();
    StmtTree *move = new MoveTree(new TempTree(result), this);
    VectorizedTree *v = new VectorizedTree(result);
    v->append(move);
    return v;
}

VectorizedTree *ExprSeqTree::vectorize() const {
    VectorizedTree *vstmt = stmt_->vectorize();
    VectorizedTree *vexpr = NULL;
    if (expr_) {
        vexpr = expr_->vectorize();
    }

    VectorizedTree *v;
    if (vexpr) {
        v = new VectorizedTree(vexpr->result_temp_);
    } else {
        v = new VectorizedTree(vstmt->result_temp_);
    }
    v->concatenate(vstmt);
    if (vexpr) {
        v->concatenate(vexpr);
    }

    return v;
}

VectorizedTree *MemTree::vectorize() const {
    /* not implemented yet! */
    return NULL;
}

VectorizedTree *NameTree::vectorize() const {
    /* not implemented yet! */
    return NULL;
}

VectorizedTree *TempTree::vectorize() const {
    /* not implemented yet! */
    return NULL;
}

VectorizedTree *VarTree::vectorize() const {
    Temp *result = new Temp();
    VectorizedTree *v = new VectorizedTree(result);
    v->append(new MoveTree(new TempTree(result), this));
    return v;
}

/* statement trees */

VectorizedTree *ExprStmtTree::vectorize() const {
    return expr_->vectorize();
}

VectorizedTree *CJumpTree::vectorize() const {
    Temp *result = new Temp();
    VectorizedTree *compval1 = left_->vectorize();
    VectorizedTree *compval2 = right_->vectorize();

    VectorizedTree *v = new VectorizedTree(result);
    v->concatenate(compval1);
    v->concatenate(compval2);
    v->append(new CJumpTree(comp_,
                new TempTree(compval1->result_temp_),
                new TempTree(compval2->result_temp_),
                t_, f_));
    return v;
}

VectorizedTree *UJumpTree::vectorize() const {
    /* UJumps don't return a result, so our result_temp_ is null */
    VectorizedTree *v = new VectorizedTree(NULL);
    v->append(this);
    return v;
}

VectorizedTree *ReturnTree::vectorize() const {
    /* not implemented yet! */
    return NULL;
}

VectorizedTree *LabelTree::vectorize() const {
    /* labels have no result */
    VectorizedTree *v = new VectorizedTree(NULL);
    v->append(this);
    return v;
}

VectorizedTree *MoveTree::vectorize() const {
    if (dest_->getType() == tt::TEMP) {
        /* We're moving something into a temp. */
        VectorizedTree *v = new VectorizedTree(dynamic_cast<const TempTree*>(dest_)->temp_);
        VectorizedTree *vsrc = src_->vectorize();
        v->concatenate(vsrc);
        v->append(new MoveTree(new TempTree(v->result_temp_), new TempTree(vsrc->result_temp_)));
        return v;
    } else if (dest_->getType() == tt::VAR) {
        /* We're moving something to a variable. This produces no result, so our result temp is NULL */
        VectorizedTree *v = new VectorizedTree(NULL);
        VectorizedTree *vsrc = src_->vectorize();
        v->concatenate(vsrc);
        v->append(new MoveTree(dest_, new TempTree(vsrc->result_temp_)));
        return v;
    } else {
        /* Otherwise we're moving an arbitrary expression somewhere. */
        VectorizedTree *vdest = dest_->vectorize();
        VectorizedTree *vsrc = src_->vectorize();
        VectorizedTree *v = new VectorizedTree(vdest->result_temp_);
        v->concatenate(vsrc);
        v->concatenate(vdest);
        /* ? not sure what the dest should be here */
        v->append(new MoveTree(new TempTree(vsrc->result_temp_), dest_));
        return v;
    }
}

VectorizedTree *SeqTree::vectorize() const {
    VectorizedTree *v1, *v2, *v;

    v1 = left_->vectorize();

    if (right_) {
        v2 = right_->vectorize();
    } else {
        v2 = NULL;
    }

    if (v2) {
        v = new VectorizedTree(v2->result_temp_);
    } else {
        v = new VectorizedTree(v1->result_temp_);
    }

    v->concatenate(v1);

    if (v2) {
        v->concatenate(v2);
    }

    return v;
}

/* tostr functions */

string StmtExprTree::toStr() const {
    stringstream ss;
    ss << "[";
    ss << stmt_->toStr();
    ss << "]";
    return ss.str();
}

string BinOpTree::toStr() const {
    stringstream ss;
    ss << "(";
    ss << left_->toStr();
    switch (op_) {
        case IRTree::Operator::PLUS:
            ss << " + ";
            break;
        case IRTree::Operator::MINUS:
            ss << " - ";
            break;
        case IRTree::Operator::MUL:
            ss << " * ";
            break;
        case IRTree::Operator::DIV:
            ss << " / ";
            break;
        case IRTree::Operator::EQ:
            ss << " = ";
            break;
        case IRTree::Operator::NE:
            ss << " != ";
            break;
        case IRTree::Operator::LT:
            ss << " < ";
            break;
        case IRTree::Operator::GT:
            ss << " > ";
            break;
        case IRTree::Operator::LE:
            ss << " <= ";
            break;
        case IRTree::Operator::GE:
            ss << " >= ";
            break;
    }
    ss << right_->toStr();
    ss << ")";
    return ss.str();
}

string CallTree::toStr() const {
    stringstream ss;
    ss << "CALL ";
    ss << name_->toStr();
    for (unsigned i = 0; i < args_.size(); i++) {
        if (i != 0) ss << ", ";
        ss << args_[i]->toStr();
    }
    return ss.str();
}

string ConstTree::toStr() const {
    stringstream ss;
    ss << "CONST ";
    ss << to_string(value_);
    return ss.str();
}

string ExprSeqTree::toStr() const {
    stringstream ss;
    ss << "\n";
    ss << stmt_->toStr();
    ss << "(";
    if (expr_ != NULL){
        ss << expr_->toStr();
    }
    ss << ")";
    return ss.str();
}

string MemTree::toStr() const {
    stringstream ss;
    ss << "MEM: ";
    ss << expr_->toStr();
    return ss.str();
}

string NameTree::toStr() const {
    stringstream ss;
    ss << "name ";
    ss << label_->toStr();
    return ss.str();
}

string TempTree::toStr() const {
    stringstream ss;
    ss << temp_->toStr();
    return ss.str();
}

string VarTree::toStr() const {
    stringstream ss;
    ss << "[";
    ss << name_;
    ss << " fp+";
    ss << offset_;
    ss << "]";
    return ss.str();
}

string ExprStmtTree::toStr() const {
    stringstream ss;
    ss << expr_->toStr();
    return ss.str();
}

string CJumpTree::toStr() const {
    stringstream ss;
    ss << "if ";
    ss << left_->toStr();
    switch (comp_) {
        case IRTree::Operator::EQ:
            ss << " = ";
            break;
        case IRTree::Operator::NE:
            ss << " != ";
            break;
        case IRTree::Operator::LT:
            ss << " < ";
            break;
        case IRTree::Operator::GT:
            ss << " > ";
            break;
        case IRTree::Operator::LE:
            ss << " <= ";
            break;
        case IRTree::Operator::GE:
            ss << " >= ";
            break;
        case IRTree::Operator::PLUS:
            ss << " + ";
            break;
        case IRTree::Operator::MINUS:
            ss << " - ";
            break;
        case IRTree::Operator::MUL:
            ss << " * ";
            break;
        case IRTree::Operator::DIV:
            ss << " / ";
            break;
    }
    ss << right_->toStr();
    ss << " go to ";
    ss << t_->toStr();
    ss << " else go to ";
    ss << f_->toStr();

    return ss.str();
}

string UJumpTree::toStr() const {
    stringstream ss;
    ss << "JUMP to ";
    ss << label_->toStr();
    return ss.str();
}

string ReturnTree::toStr() const {
    stringstream ss;
    ss << "RETURN ";
    ss << expr_->toStr();
    return ss.str();
}

string LabelTree::toStr() const {
    stringstream ss;
    ss << ".";
    ss << l_->toStr();
    ss << ": ";
    return ss.str();
}

string MoveTree::toStr() const {
    stringstream ss;
    ss << "MOVE ";
    ss << src_->toStr();
    ss << " to ";
    ss << dest_->toStr();
    return ss.str();
}

string SeqTree::toStr() const {
    stringstream ss;
    if (left_) {
        ss << left_->toStr();
    }
    if (left_ || right_) {
        ss << "\n";
    }
    if (right_) {
        ss << right_->toStr();
    }
    return ss.str();
}

string VectorizedTree::toStr() const {
    stringstream ss;
    for (auto a : content_) {
        ss << a->toStr();
        ss << "\n";
    }
    return ss.str();
}

} //NAMESPACE



