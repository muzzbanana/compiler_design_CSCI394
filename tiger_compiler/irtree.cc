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

NewFrameTree::NewFrameTree(const int num_locals) : StmtTree(tt::NEWFRAME), num_locals_(num_locals) { }

EndFrameTree::EndFrameTree() : StmtTree(tt::ENDFRAME) { }

ArgReserveTree::ArgReserveTree(const int num_args) : StmtTree(tt::ARGRESERVE), num_args_(num_args) { }

ArgPutTree::ArgPutTree(const int index, const ExprTree *arg) : StmtTree(tt::ARGPUT), index_(index), arg_(arg) { }

ArgRemoveTree::ArgRemoveTree(const int amount) : StmtTree(tt::ARGREMOVE), amount_(amount) { }

StaticStringTree::StaticStringTree(const Label *label, const string value) : StmtTree(tt::STATICSTR), label_(label), value_(value) { }

SeqTree::SeqTree() : StmtTree(tt::SEQ), left_(NULL), right_(NULL) { }

SeqTree::SeqTree(const StmtTree *left, const StmtTree *right) : StmtTree(tt::SEQ), left_(left), right_(right) { }

/* fragment */

Fragment::Fragment(Temp *result_temp) : IRTree(tt::VECTORIZED), result_temp_(result_temp) { }

void Fragment::append(const StmtTree *stmt) {
    content_.push_back(stmt);
}

void Fragment::concat(const Fragment *vec) {
    if (vec == NULL) return;

    for (auto stmt : vec->content_) {
        append(stmt);
    }
}

FragMove::FragMove(const ExprTree *dest, const ExprTree *src) : StmtTree(tt::VECMOVE), dest_(dest), src_(src) { }

/* vectorization functions */

/* expr trees */

Fragment *StmtExprTree::vectorize() const {
    return stmt_->vectorize();
}

Fragment *BinOpTree::vectorize() const {
    Temp *result = new Temp();
    Fragment *vleft = left_->vectorize();
    Fragment *vright = right_->vectorize();
    StmtTree *move = new FragMove(new TempTree(result),
            new BinOpTree(op_, new TempTree(vleft->result_temp_), new TempTree(vright->result_temp_)));
    Fragment *v = new Fragment(result);
    v->concat(vleft);
    v->concat(vright);
    v->append(move);
    return v;
}

Fragment *CallTree::vectorize() const {
    Temp *result = new Temp();
    StmtTree *move = new FragMove(new TempTree(result), this);
    Fragment *v = new Fragment(result);

    v->append(new ArgReserveTree(args_.size()));

    int arg_index = 0;
    for (auto a : args_) {
        Fragment *varg = a->vectorize();
        v->concat(varg);
        v->append(new ArgPutTree(arg_index, new TempTree(varg->result_temp_)));
        arg_index ++;
    }

    v->append(move);
    v->append(new ArgRemoveTree(args_.size()));

    return v;
}

Fragment *ConstTree::vectorize() const {
    Temp *result = new Temp();
    StmtTree *move = new FragMove(new TempTree(result), this);
    Fragment *v = new Fragment(result);
    v->append(move);
    return v;
}

Fragment *ExprSeqTree::vectorize() const {
    Fragment *vstmt = stmt_->vectorize();
    Fragment *vexpr = NULL;
    if (expr_) {
        vexpr = expr_->vectorize();
    }

    Fragment *v;
    if (vexpr) {
        v = new Fragment(vexpr->result_temp_);
    } else {
        v = new Fragment(vstmt->result_temp_);
    }
    v->concat(vstmt);
    if (vexpr) {
        v->concat(vexpr);
    }

    return v;
}

Fragment *MemTree::vectorize() const {
    /* not implemented yet! */
    return NULL;
}

Fragment *NameTree::vectorize() const {
    Temp *result = new Temp();
    Fragment *v = new Fragment(result);
    v->append(new FragMove(new TempTree(result), this));
    return v;
}

Fragment *TempTree::vectorize() const {
    /* not implemented yet! */
    return NULL;
}

Fragment *VarTree::vectorize() const {
    Temp *result = new Temp();
    Fragment *v = new Fragment(result);
    v->append(new FragMove(new TempTree(result), this));
    return v;
}

/* statement trees */

Fragment *ExprStmtTree::vectorize() const {
    return expr_->vectorize();
}

Fragment *CJumpTree::vectorize() const {
    Temp *result = new Temp();
    Fragment *compval1 = left_->vectorize();
    Fragment *compval2 = right_->vectorize();

    Fragment *v = new Fragment(result);
    v->concat(compval1);
    v->concat(compval2);
    v->append(new CJumpTree(comp_,
                new TempTree(compval1->result_temp_),
                new TempTree(compval2->result_temp_),
                t_, f_));
    return v;
}

Fragment *UJumpTree::vectorize() const {
    /* UJumps don't return a result, so our result_temp_ is null */
    Fragment *v = new Fragment(NULL);
    v->append(this);
    return v;
}

Fragment *ReturnTree::vectorize() const {
    Fragment *vexpr = expr_->vectorize();
    ReturnTree *rt = NULL;
    Fragment *v = NULL;

    if (vexpr->result_temp_) {
        rt = new ReturnTree(new TempTree(vexpr->result_temp_));
        v = new Fragment(vexpr->result_temp_);
    } else {
        rt = new ReturnTree(NULL);
        v = new Fragment(NULL);
    }

    v->concat(vexpr);
    v->append(rt);
    return v;
}

Fragment *LabelTree::vectorize() const {
    /* labels have no result */
    Fragment *v = new Fragment(NULL);
    v->append(this);
    return v;
}

Fragment *MoveTree::vectorize() const {
    if (dest_->getType() == tt::TEMP) {
        /* We're moving something into a temp. */
        Fragment *v = new Fragment(dynamic_cast<const TempTree*>(dest_)->temp_);
        Fragment *vsrc = src_->vectorize();
        v->concat(vsrc);
        v->append(new FragMove(new TempTree(v->result_temp_), new TempTree(vsrc->result_temp_)));
        return v;
    } else if (dest_->getType() == tt::VAR) {
        /* We're moving something to a variable. This produces no result, so our result temp is NULL */
        Fragment *v = new Fragment(NULL);
        Fragment *vsrc = src_->vectorize();
        v->concat(vsrc);
        v->append(new FragMove(dest_, new TempTree(vsrc->result_temp_)));
        return v;
    } else {
        /* Otherwise we're moving an arbitrary expression somewhere. */
        Fragment *vdest = dest_->vectorize();
        Fragment *vsrc = src_->vectorize();
        Fragment *v = new Fragment(vdest->result_temp_);
        v->concat(vsrc);
        v->concat(vdest);
        /* ? not sure what the dest should be here */
        v->append(new FragMove(new TempTree(vsrc->result_temp_), dest_));
        return v;
    }
}

Fragment *NewFrameTree::vectorize() const {
    Fragment *v = new Fragment(NULL);
    v->append(this);
    return v;
}

Fragment *EndFrameTree::vectorize() const {
    Fragment *v = new Fragment(NULL);
    v->append(this);
    return v;
}

Fragment *ArgReserveTree::vectorize() const {
    Fragment *v = new Fragment(NULL);
    v->append(this);
    return v;
}

Fragment *ArgPutTree::vectorize() const {
    Fragment *v = new Fragment(NULL);
    v->append(this);
    return v;
}

Fragment *ArgRemoveTree::vectorize() const {
    Fragment *v = new Fragment(NULL);
    v->append(this);
    return v;
}

Fragment *StaticStringTree::vectorize() const {
    Fragment *v = new Fragment(NULL);
    v->append(this);
    return v;
}

Fragment *SeqTree::vectorize() const {
    Fragment *v1, *v2, *v;

    v1 = left_->vectorize();

    if (right_) {
        v2 = right_->vectorize();
    } else {
        v2 = NULL;
    }

    if (v2 && v2->result_temp_) {
        v = new Fragment(v2->result_temp_);
    } else {
        v = new Fragment(v1->result_temp_);
    }

    v->concat(v1);

    if (v2) {
        v->concat(v2);
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
    ss << " (";
    for (unsigned i = 0; i < args_.size(); i++) {
        if (i != 0) ss << ", ";
        ss << args_[i]->toStr();
    }
    ss << ")";
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
    ss << "(";
    ss << stmt_->toStr();
    ss << " ; ";
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
    ss << ".";
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
    ss << name_;
    ss << " [fp+";
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
    if (expr_) {
        ss << expr_->toStr();
    }
    ss << "\n";
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

string NewFrameTree::toStr() const {
    stringstream ss;
    ss << "NEW FRAME (";
    ss << num_locals_;
    ss << " locals)";
    return ss.str();
}

string EndFrameTree::toStr() const {
    return "END FRAME";
}

string ArgReserveTree::toStr() const {
    stringstream ss;
    ss << "PASS ";
    ss << num_args_;
    ss << " ARGUMENTS";
    return ss.str();
}

string ArgPutTree::toStr() const {
    stringstream ss;
    ss << "SET ARG ";
    ss << index_;
    ss << " TO ";
    ss << arg_->toStr();
    return ss.str();
}

string ArgRemoveTree::toStr() const {
    stringstream ss;
    ss << "REMOVE ";
    ss << amount_;
    ss << " ARGUMENTS";
    return ss.str();
}

string StaticStringTree::toStr() const {
    stringstream ss;
    ss << ".";
    ss << label_->toStr();
    ss << ":\t";
    ss << value_;
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

string Fragment::toStr() const {
    stringstream ss;
    for (auto a : content_) {
        ss << a->toStr();
        ss << "\n";
    }
    return ss.str();
}

string FragMove::toStr() const {
    stringstream ss;
    if (src_->getType() == tt::BINOP) {
        const BinOpTree *bo = dynamic_cast<const BinOpTree*>(src_);
        if (bo->op_ == IRTree::Operator::EQ) {
            ss << "EQ";
        } else if (bo->op_ == IRTree::Operator::NE) {
            ss << "NEQ";
        } else if (bo->op_ == IRTree::Operator::LT) {
            ss << "LT";
        } else if (bo->op_ == IRTree::Operator::GT) {
            ss << "GT";
        } else if (bo->op_ == IRTree::Operator::GE) {
            ss << "GE";
        } else if (bo->op_ == IRTree::Operator::GE) {
            ss << "LE";
        } else if (bo->op_ == IRTree::Operator::PLUS) {
            ss << "ADD";
        } else if (bo->op_ == IRTree::Operator::MINUS) {
            ss << "SUB";
        } else if (bo->op_ == IRTree::Operator::MUL) {
            ss << "MUL";
        } else if (bo->op_ == IRTree::Operator::DIV) {
            ss << "DIV";
        } else {
            ss << "??";
        }
        ss << "\t";
        ss << bo->left_->toStr();
        ss << ", ";
        ss << bo->right_->toStr();
    } else if (src_->getType() == tt::VAR) {
        ss << "LOAD\t";
        ss << src_->toStr();
    } else if (dest_->getType() == tt::VAR) {
        ss << "STORE\t";
        ss << src_->toStr();
    } else if (src_->getType() == tt::CALL) {
        ss << "CALL\t";
        ss << dynamic_cast<const CallTree*>(src_)->name_->toStr();
    } else {
        ss << "MOVE\t";
        ss << src_->toStr();
    }
    ss << " into ";
    ss << dest_->toStr();
    return ss.str();
}

} //NAMESPACE
