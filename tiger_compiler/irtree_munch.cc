#include "irtree.hh"
#include <iostream>

namespace tiger {

void FragMove::munch(InstructionList instrs) const {
    string command;
    vector<string> args;
    if (src_->getType() == tt::BINOP) {
        const BinOpTree *bo = dynamic_cast<const BinOpTree*>(src_);
        /* TODO mips doesn't have instructions for eq/lt/etc */
        if (bo->op_ == IRTree::Operator::EQ) {
            command = "EQ";
        } else if (bo->op_ == IRTree::Operator::NE) {
            command = "NEQ";
        } else if (bo->op_ == IRTree::Operator::LT) {
            command = "LT";
        } else if (bo->op_ == IRTree::Operator::GT) {
            command = "GT";
        } else if (bo->op_ == IRTree::Operator::GE) {
            command = "GE";
        } else if (bo->op_ == IRTree::Operator::GE) {
            command = "LE";
        } else if (bo->op_ == IRTree::Operator::PLUS) {
            command = "add";
        } else if (bo->op_ == IRTree::Operator::MINUS) {
            command = "sub";
        } else if (bo->op_ == IRTree::Operator::MUL) {
            command = "mult";
        } else if (bo->op_ == IRTree::Operator::DIV) {
            command = "div";
        } else {
            command = "??";
        }
        /* TODO get temp locations and use them here!! */
        args.push_back(dest_->toStr());
        args.push_back(bo->left_->toStr());
        args.push_back(bo->right_->toStr());
    } else if (src_->getType() == tt::VAR) {
        command = "lw";
        args.push_back(dest_->toStr());
        args.push_back(src_->toStr());
    } else if (dest_->getType() == tt::VAR) {
        command = "sw";
        args.push_back(dest_->toStr());
        args.push_back(src_->toStr());
    } else if (src_->getType() == tt::CALL) {
        command = "jal";
        args.push_back(dynamic_cast<const CallTree*>(src_)->name_->toStr());
    } else {
        command = "move";
        args.push_back(dest_->toStr());
        args.push_back(src_->toStr());
    }
    instrs.push_back(new ASMMove(command, args, toStr()));
}

} //NAMESPACE
