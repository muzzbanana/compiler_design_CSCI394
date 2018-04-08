#include "irtree.hh"

namespace tiger {

using tt = IRTree::TreeType;

ExprTree *ExprTree::notImpl = new NotImplExprTree();
StmtTree *StmtTree::notImpl = new NotImplStmtTree();

IRTree::IRTree(tt type) : type_(type) { }

ExprTree::ExprTree(tt type) : IRTree(type) { }

StmtTree::StmtTree(tt type) : IRTree(type) { }

/* expr trees */

BinOpTree::BinOpTree(Operator op, BinOpTree *left, BinOpTree *right)
    : ExprTree(tt::BINOP), op_(op), left_(left), right_(right) { }

CallTree::CallTree(NameTree *name, const ExprList args)
    : ExprTree(tt::CALL), name_(name), args_(args) { }

ConstTree::ConstTree(int value) : ExprTree(tt::CONST), value_(value) { }

ExprSeqTree::ExprSeqTree(StmtTree *stmt, ExprTree *expr)
    : ExprTree(tt::ESEQ), stmt_(stmt), expr_(expr) { }

MemTree::MemTree(ExprTree *expr) : ExprTree(tt::MEM), expr_(expr) { }

NameTree::NameTree(Label *label) : ExprTree(tt::NAME), label_(label) { }

TempTree::TempTree(Temp *temp) : ExprTree(tt::TEMP), temp_(temp) { }

/* statement trees */

ExprStmtTree::ExprStmtTree(ExprTree *expr) : StmtTree(tt::EXPR), expr_(expr) { }

CJumpTree::CJumpTree(CJumpTree::Comparison comp, ExprTree *left, ExprTree *right, Label *t, Label *f)
    : StmtTree(tt::CJUMP), comp_(comp), left_(left), right_(right), t_(t), f_(f) { }

UJumpTree::UJumpTree(Label *label) : StmtTree(tt::UJUMP), label_(label) { }

ReturnTree::ReturnTree(ExprTree *expr) : StmtTree(tt::RETURN), expr_(expr) { }

MoveTree::MoveTree(ExprTree *dest, ExprTree *src) : StmtTree(tt::MOVE), dest_(dest), src_(src) { }

SeqTree::SeqTree() : StmtTree(tt::SEQ), left_(NULL), right_(NULL) { }

SeqTree::SeqTree(StmtTree *left, StmtTree *right) : StmtTree(tt::SEQ), left_(left), right_(right) { }

/* tostr functions */


string BinOpTree::toStr() const {
    stringstream ss;
    ss << left_->toStr();
    switch (op_) {
        case Operator::PLUS:
            ss << " + ";
            break;
        case Operator::MINUS:
            ss << " - ";
            break;
        case Operator::MUL:
            ss << " * ";
            break;
        case Operator::DIV:
            ss << " / ";
            break;
    }
    ss << right_->toStr();
    return ss.str();
}

string CallTree::toStr() const {
    stringstream ss;
    ss << "CALL ";
    ss << name_->toStr();
    ss << " ( ";
    for (unsigned i = 0; i < args_.size(); i++) {
        if (i != 0) ss << ", ";
        ss << args_[i]->toStr();
    }
    ss << " ) ";
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
    ss << "ExprSeqTree: statement ";
    ss << stmt_->toStr();
    ss << ", expression ";
    ss << expr_->toStr();
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

string ExprStmtTree::toStr() const {
    stringstream ss;
    ss << "ExprStmtTree: ";
    ss << expr_->toStr();
    return ss.str();
}

string CJumpTree::toStr() const {
    stringstream ss;
    ss << left_->toStr();
    switch (comp_) {
        case Comparison::EQ:
            ss << " = ";
            break;
        case Comparison::NE:
            ss << " != ";
            break;
        case Comparison::LT:
            ss << " < ";
            break;
        case Comparison::GT:
            ss << " > ";
            break;
        case Comparison::LE:
            ss << " <= ";
            break;
        case Comparison::GE:
            ss << " >= ";
            break;
    }
    ss << right_->toStr();
    ss << " if true go to ";
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
    ss << "SEQ: ";
    ss << left_->toStr();
    ss << " ";
    ss << right_->toStr();
    return ss.str();
}

} //NAMESPACE



