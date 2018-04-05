#include "irtree.hh"

namespace tiger {

using tt = IRTree::TreeType;

IRTree::IRTree(tt type) : type_(type) { }

ExprTree::ExprTree(tt type) : IRTree(type) { }

StmtTree::StmtTree(tt type) : IRTree(type) { }

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

}//namespace
