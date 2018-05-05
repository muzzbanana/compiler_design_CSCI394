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
        if (bo->op_ == IRTree::Operator::PLUS) {
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
    } else if (src_->getType() == tt::CONST) {
        command = "li";
        args.push_back(to_string(dynamic_cast<const ConstTree*>(dest_)->value_));
        args.push_back(src_->toStr());
    } else {
        command = "move";
        args.push_back(dest_->toStr());
        args.push_back(src_->toStr());
    }
    instrs.push_back(new ASMMove(command, args, toStr()));
}

void StmtExprTree::munch(InstructionList instrs) {
    /* shouldn't be called! */
    cerr << "error, called StmtExprTree::munch!" << endl;
}

void BinOpTree::munch(InstructionList instrs) {
    /* shouldn't be called, since binoptree should only be found inside movetree */
    cerr << "error, called BinOpTree::munch!" << endl;
}

void CallTree::munch(InstructionList instrs) {
    /* handled in fragmove */
}

void ConstTree::munch(InstructionList instrs) {
    //instrs.push_back(value_->toStr()); //this is probably wrong!
    // see added const section to fragmove
}

void ExprSeqTree::munch(InstructionList instrs) {
    /* not here at this point */
}

void MemTree::munch(InstructionList instrs) {
    /* i don't think this one is ever used */
}

void NameTree::munch(InstructionList instrs) {
    /* TODO fill me in! */
    /* not sure if we need this one ? i don't think so */
}

void TempTree::munch(InstructionList instrs) {
    /* TODO fill me in! */
    /* this is going to be tricky! */

}

void VarTree::munch(InstructionList instrs) {
    /* TODO fill me in! */
    /* this needs to turn into a 'whatever($fp)' i think */
}

void ConditionalExprTree::munch(InstructionList instrs) {
    //should be v similar to CJumpTree. What are left and right?
    // oh sorry this one doesn't need filled in.
    // conditionalexprtree gets turned into a cjumptree when we call vectorize()
}

/* statement trees */

void ExprStmtTree::munch(InstructionList instrs) {
    /* not here at this point */
}

void CJumpTree::munch(InstructionList instrs) {
    string command;
    vector<string> args;
    switch(comp_) {
        case IRTree::Operator::EQ:
            command = "beq";
            break;
        case IRTree::Operator::NE:
            command = "bne";
            break;
        case IRTree::Operator::LT:
            command = "blt";
            break;
        case IRTree::Operator::LE:
            command = "ble";
            break;
        case IRTree::Operator::GT:
            command = "bgt";
            break;
        case IRTree::Operator::GE:
            command = "bge";
            break;
        default:
            command = "???";
    }
    /* TODO get temp locations and use them here!! */
    args.push_back(left_->toStr());
    args.push_back(right_->toStr());
    args.push_back(t_->toStr());
    /* this probably shouldn't be an ASMMove but also idk if it matters */
    instrs.push_back(new ASMMove(command, args, toStr()));

    /* We need to unconditionally branch to false label now if
     * we didn't jump to true label. */
    instrs.push_back(new ASMJump("j", f_));
}

void UJumpTree::munch(InstructionList instrs) {
    /* TODO fill me in! */
}

void ReturnTree::munch(InstructionList instrs) {
    /* TODO use temp stack locations or w/e */
    /* TODO save and restore return value on stack */
    vector<string> args;
    args.push_back(expr_->toStr());
    args.push_back("$v0");
    instrs.push_back(new ASMMove("move", args, toStr())); //I suspect actually this should be an operation? -E
    vector<string> ret_args;
    ret_args.push_back("$ra");
    /* this shouldn't be an ASMMove i don't htink */
    instrs.push_back(new ASMMove("jr", ret_args, toStr()));
}

void LabelTree::munch(InstructionList instrs) {
    /* TODO fill me in! */
    /* a label */
}

void MoveTree::munch(InstructionList instrs) {
    vector<string> args;
    args.push_back(src_->toStr());
    args.push_back(dest_->toStr());
    instrs.push_back(new ASMOperation("move", args, toStr()));
}

/* this is all function stack stuff hmm */
void NewFrameTree::munch(InstructionList instrs) {
    /* needs to expand stack to hold however many local vars we need */
    vector<string> args;
    args.push_back("$t0");
    args.push_back("$sp");
    instrs.push_back(new ASMOperation("lw", args, "#saves the current stack pointer"));
    vector<string> args2;
    args.push_back("$sp");
    args.push_back("$sp");
    int num = -4*(num_locals_+1)
    args.push_back(string(num)); //forgot how to dynamically cast this!!
    instrs.push_back(new ASMOperation("add", args2, "#increments stack for new frame's locals"));
    vector<string> args3;
    args.push_back("($sp)");
    args.push_back("t0");
    instrs.push_back(new ASMOperation("lw", args3, "#puts the return address on the top of the stack"));
       

}

void EndFrameTree::munch(InstructionList instrs) {
    /* needs to pop off those local vars (+ maybe temps if there are any left) */
    // TODO: first go through and delete any temps on top of the stack!!!! Not sure how to access temps from here!!!!
    vector<string> args;
    args.push_back("$sp");
    args.push_back("($sp)");
    instrs.push_back(new ASMOperation("lw", args, "#returns sp to the return address"));

}

void ArgReserveTree::munch(InstructionList instrs) {
    /* TODO fill me in! */
    /* expand stack to fit N-4 number of arguments (we can pass args 1-4 in $a0-$a3) */
}

void ArgPutTree::munch(InstructionList instrs) {
    /* TODO fill me in! */
    /* put arg in register or stack */
}

void ArgRemoveTree::munch(InstructionList instrs) {
    /* TODO fill me in! */
    /* remove args from stack (undo ArgReserve) */
}

void StaticStringTree::munch(InstructionList instrs) {
    /* TODO fill me in! */
    /* a static string i forget the syntax for this tho. .asciiz something */
}

void SeqTree::munch(InstructionList instrs) {
    /* shouldn't be called */
    cerr << "error, called SeqTree::munch!" << endl;
}

void Fragment::munch(InstructionList instrs) {
    /* TODO fill me in (maybe?) */
}

void NotImplExprTree::munch(InstructionList instrs) {
    /* don't call this */
}

void NotImplStmtTree::munch(InstructionList instrs) {
    /* don't call this */
}

} //NAMESPACE
