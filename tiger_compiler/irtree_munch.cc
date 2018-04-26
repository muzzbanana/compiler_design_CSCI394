#include "irtree.hh"
#include <iostream>

namespace tiger {

using tt = IRTree::TreeType;

void FragMove::munch(InstructionList instrs) {
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

void StmtExprTree::munch(InstructionList instrs) {
/* TODO fill me in! */ }

void BinOpTree::munch(InstructionList instrs) {
/* TODO fill me in! */ }

void CallTree::munch(InstructionList instrs) {
/* TODO fill me in! */ }

void ConstTree::munch(InstructionList instrs) {
/* TODO fill me in! */ }

void ExprSeqTree::munch(InstructionList instrs) {
/* TODO fill me in! */ }

void MemTree::munch(InstructionList instrs) {
/* TODO fill me in! */ }

void NameTree::munch(InstructionList instrs) {
/* TODO fill me in! */ }

void TempTree::munch(InstructionList instrs) {
/* TODO fill me in! */ }

void VarTree::munch(InstructionList instrs) {
/* TODO fill me in! */ }

void ConditionalExprTree::munch(InstructionList instrs) {
    /* TODO fill me in ! */
}

/* statement trees */

void ExprStmtTree::munch(InstructionList instrs) {
/* TODO fill me in! */ }

void CJumpTree::munch(InstructionList instrs) {
    /* TODO fill me in! */
}

void UJumpTree::munch(InstructionList instrs) {
/* TODO fill me in! */ }

void ReturnTree::munch(InstructionList instrs) {
/* TODO fill me in! */ }

void LabelTree::munch(InstructionList instrs) {
/* TODO fill me in! */ }

void MoveTree::munch(InstructionList instrs) {
/* TODO fill me in! */ }

void NewFrameTree::munch(InstructionList instrs) {
/* TODO fill me in! */ }

void EndFrameTree::munch(InstructionList instrs) {
/* TODO fill me in! */ }

void ArgReserveTree::munch(InstructionList instrs) {
/* TODO fill me in! */ }

void ArgPutTree::munch(InstructionList instrs) {
/* TODO fill me in! */ }

void ArgRemoveTree::munch(InstructionList instrs) {
/* TODO fill me in! */ }

void StaticStringTree::munch(InstructionList instrs) {
/* TODO fill me in! */ }

void SeqTree::munch(InstructionList instrs) {
/* TODO fill me in! */ }

void Fragment::munch(InstructionList instrs) {
    /* TODO fill me in */
}

void NotImplExprTree::munch(InstructionList instrs) {
    /* don't call this */
}

void NotImplStmtTree::munch(InstructionList instrs) {
    /* don't call this */
}

} //NAMESPACE
