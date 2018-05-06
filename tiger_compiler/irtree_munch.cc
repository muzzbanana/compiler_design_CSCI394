#include "irtree.hh"
#include <iostream>

namespace tiger {

using tt = IRTree::TreeType;

/* Generate instruction(s) to load the top of stack into
 * the supplied register. */
void pop_into(InstructionList& instrs, string reg, string cmt) {
    vector<string> getargs;
    getargs.push_back(reg);
    getargs.push_back("($sp)");
    instrs.push_back(new ASMMove("lw", getargs, cmt));

    vector<string> addargs;
    addargs.push_back("$sp");
    addargs.push_back("$sp");
    addargs.push_back("4");
    instrs.push_back(new ASMMove("add", addargs, cmt));
}

/* Pop into two registers at once to save on stack pointer math. */
/* (top of stack -> reg1, next value -> reg2) */
void pop2_into(InstructionList& instrs, string reg1, string reg2, string cmt) {
    vector<string> getargs1;
    getargs1.push_back(reg1);
    getargs1.push_back("($sp)");
    instrs.push_back(new ASMMove("lw", getargs1, cmt));

    vector<string> getargs2;
    getargs2.push_back(reg2);
    getargs2.push_back("4($sp)");
    instrs.push_back(new ASMMove("lw", getargs2, cmt));

    vector<string> addargs;
    addargs.push_back("$sp");
    addargs.push_back("$sp");
    addargs.push_back("8");
    instrs.push_back(new ASMMove("add", addargs, cmt));
}

/* Generate instructions to push a given register onto the stack. */
void push_from(InstructionList& instrs, string reg, string cmt) {
    vector<string> addargs;
    addargs.push_back("$sp");
    addargs.push_back("$sp");
    addargs.push_back("-4");
    instrs.push_back(new ASMMove("add", addargs, cmt));

    vector<string> putargs;
    putargs.push_back(reg);
    putargs.push_back("($sp)");
    instrs.push_back(new ASMMove("sw", putargs, cmt));
}

void FragMove::munch(InstructionList& instrs) const {
    string command;
    vector<string> args;
    if (src_->getType() == tt::BINOP) {
        const BinOpTree *bo = dynamic_cast<const BinOpTree*>(src_);
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
        /* I'm pretty sure that the way it's set up guarantees the
         * arguments to each binop are always on top. So we just need
         * to move the top two arguments from the stack to $t0 + $t1,
         * then do the thing, then store it back on the stack. */
        /* Pop off backwards because think about subtraction: we push
         * the first operand onto the stack first, so need to pop first
         * into t1, then t0. */
        pop2_into(instrs, "$t1", "$t0", toStr());
        args.push_back("$t0");
        args.push_back("$t0");
        args.push_back("$t1");
        instrs.push_back(new ASMMove(command, args, toStr()));
        push_from(instrs, "$t0", toStr());
    } else if (src_->getType() == tt::VAR) {
        command = "lw";
        args.push_back("$t0");
        args.push_back(to_string(-4*dynamic_cast<const VarTree*>(src_)->offset_) + "($fp)");
        instrs.push_back(new ASMMove(command, args, toStr()));
        push_from(instrs, "$t0", "push var value on stack");
    } else if (dest_->getType() == tt::VAR) {
        pop_into(instrs, "$t0", "get value to put");
        command = "sw";
        args.push_back("$t0");
        args.push_back(to_string(-4*dynamic_cast<const VarTree*>(dest_)->offset_) + "($fp)");
        instrs.push_back(new ASMMove(command, args, toStr()));
    } else if (src_->getType() == tt::CALL) {
        command = "jal";
        args.push_back(dynamic_cast<const CallTree*>(src_)->name_->toStr());
        instrs.push_back(new ASMMove(command, args, toStr()));
    } else if (src_->getType() == tt::CONST) {
        command = "li";
        args.push_back("$t0");
        args.push_back(to_string(dynamic_cast<const ConstTree*>(src_)->value_));
        instrs.push_back(new ASMMove(command, args, toStr()));
        push_from(instrs, "$t0", toStr());
    } else {
        command = "move";
        args.push_back(dest_->toStr());
        args.push_back(src_->toStr());
        instrs.push_back(new ASMMove(command, args, toStr()));
    }
}

void StmtExprTree::munch(InstructionList& instrs) const {
    /* shouldn't be called! */
    cerr << "error, called StmtExprTree::munch!" << endl;
}

void BinOpTree::munch(InstructionList& instrs) const {
    /* shouldn't be called, since binoptree should only be found inside movetree */
    cerr << "error, called BinOpTree::munch!" << endl;
}

void CallTree::munch(InstructionList& instrs) const {
    /* handled in fragmove */
}

void ConstTree::munch(InstructionList& instrs) const {
    //instrs.push_back(value_->toStr()); //this is probably wrong!
    // see added const section to fragmove
}

void ExprSeqTree::munch(InstructionList& instrs) const {
    /* not here at this point */
}

void MemTree::munch(InstructionList& instrs) const {
    /* i don't think this one is ever used */
}

void NameTree::munch(InstructionList& instrs) const {
    /* TODO fill me in! */
    /* not sure if we need this one ? i don't think so */
}

void TempTree::munch(InstructionList& instrs) const {
    /* TODO fill me in! */
    /* this is going to be tricky! */
}

void VarTree::munch(InstructionList& instrs) const {
    /* TODO fill me in! */
    /* this needs to turn into offset_($fp) i think */
}

void ConditionalExprTree::munch(InstructionList& instrs) const {
    //should be v similar to CJumpTree. What are left and right?
    // oh sorry this one doesn't need filled in.
    // conditionalexprtree gets turned into a cjumptree when we call vectorize()
}

/* statement trees */

void ExprStmtTree::munch(InstructionList& instrs) const {
    /* not here at this point */
}

void CJumpTree::munch(InstructionList& instrs) const {
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
    pop2_into(instrs, "$t1", "$t0", toStr());
    args.push_back("$t0");
    args.push_back("$t1");
    args.push_back(t_->toStr());
    /* this probably shouldn't be an ASMMove but also idk if it matters */
    instrs.push_back(new ASMMove(command, args, toStr()));

    /* We need to unconditionally branch to false label now if
     * we didn't jump to true label. */
    instrs.push_back(new ASMJump("j", f_));
}

void UJumpTree::munch(InstructionList& instrs) const {
    instrs.push_back(new ASMJump("j", label_));
}

void ReturnTree::munch(InstructionList& instrs) const {
    /* TODO use temp stack locations or w/e */
    /* TODO save and restore return value on stack */
    //vector<string> args;
    //args.push_back(expr_->toStr());
    //args.push_back("$v0");
    //instrs.push_back(new ASMMove("move", args, toStr())); //I suspect actually this should be an operation? -E
    /* Return value will be moved when we encounter END FRAME */
    vector<string> ret_args;
    ret_args.push_back("$ra");
    instrs.push_back(new ASMOperation("jr", ret_args, toStr()));
}

void LabelTree::munch(InstructionList& instrs) const {
    instrs.push_back(new ASMLabel(l_));
}

void MoveTree::munch(InstructionList& instrs) const {
    vector<string> args;
    args.push_back(src_->toStr());
    args.push_back(dest_->toStr());
    instrs.push_back(new ASMOperation("move", args, toStr()));
}

/* this is all function stack stuff hmm */
void NewFrameTree::munch(InstructionList& instrs) const {
    /* Set frame pointer to point to base of stack frame (which is current $sp - 4) */
    /* it's $sp-4 because we need 0($fp) to be the first local, -4($fp) second local, etc. */
    push_from(instrs, "$fp", "save old frame pointer");
    vector<string> args0;
    args0.push_back("$fp");
    args0.push_back("$sp");
    args0.push_back("4");
    instrs.push_back(new ASMOperation("sub", args0, "point at base of frame"));
    /* needs to expand stack to hold however many local vars we need */
    vector<string> args1;
    args1.push_back("$t0");
    args1.push_back("$sp");
    instrs.push_back(new ASMOperation("move", args1, "#saves the current return address as a stack pointer")); 
    vector<string> args2;
    args2.push_back("$sp");
    args2.push_back("$sp");
    int num = -4*(num_locals_+1);
    args2.push_back(to_string(num));
    instrs.push_back(new ASMOperation("add", args2, "#increments stack for new frame's locals"));
    vector<string> args3;
    args3.push_back("$t0");
    args3.push_back("($sp)");
    instrs.push_back(new ASMOperation("sw", args3, "#puts the return address on the top of the stack"));
    /* Need to explicitly save return address on stack because MIPS */
    push_from(instrs, "$ra", "save function return addr");
}

void EndFrameTree::munch(InstructionList& instrs) const {
    /* needs to pop off those local vars (+ maybe temps if there are any left) */
    // TODO: first go through and delete any temps on top of the stack!!!! Not sure how to access temps from here!!!!
    /* Put last temp value into $v0 in case we're returning afterwards */
    pop_into(instrs, "$v0", "load return value");
    /* Need to explicitly save return address on stack because MIPS */
    pop_into(instrs, "$ra", "recall function return addr");
    /* Restore old frame pointer */
    vector<string> fargs;
    fargs.push_back("$fp");
    fargs.push_back("4($sp)");
    instrs.push_back(new ASMOperation("lw", fargs, "restore old frame pointer"));
    /* Restore stack pointer (hopefully this lines up properly?!) */
    vector<string> args;
    args.push_back("$sp");
    args.push_back("($sp)");
    instrs.push_back(new ASMOperation("lw", args, "#returns sp to the return address"));
}

void ArgReserveTree::munch(InstructionList& instrs) const {
    /* expand stack to fit N-4 number of arguments (we can pass args 1-4 in $a0-$a3) */
    // if we want to load the first four arguments in this fragment we should do this here
    vector<string> args;
    args.push_back("$t0");
    args.push_back("$sp");
    instrs.push_back(new ASMOperation("move", args, "#saves the current stack pointer"));
    if (num_args_>4) {
        vector<string> args2;
        args2.push_back("$sp");
        args2.push_back("$sp");
        int num = -4*(num_args_-4);
        args2.push_back(to_string(num));
        instrs.push_back(new ASMOperation("add", args2, "#increments stack for new frame's args"));
    }
    vector<string> args3;
    args3.push_back("$t0");
    args3.push_back("($sp)");
    instrs.push_back(new ASMOperation("sw", args3, "#puts the return address on the top of the stack"));
}

void ArgPutTree::munch(InstructionList& instrs) const {
    /* put arg in register or stack */
    vector<string> args;
    args.push_back("$t0");
    args.push_back("$sp");
    args.push_back(to_string(index_));
    instrs.push_back(new ASMOperation("sub", args, "#figures out where to put the current argument"));
    vector<string> args2;
    args2.push_back(arg_->toStr());
    args2.push_back("($t0)");
    instrs.push_back(new ASMOperation("lw", args2, "#loads the argument into the register"));
}

void ArgRemoveTree::munch(InstructionList& instrs) const {
    /* remove args from stack (undo ArgReserve) */
    vector<string> args;
    args.push_back("$sp");
    args.push_back("($sp)");
    instrs.push_back(new ASMOperation("lw", args, "#returns sp to the return address"));
}

void StaticStringTree::munch(InstructionList& instrs) const {
    /* TODO fill me in! */
    /* a static string i forget the syntax for this tho. .asciiz something */
}

void SeqTree::munch(InstructionList& instrs) const {
    /* shouldn't be called */
    cerr << "error, called SeqTree::munch!" << endl;
}

void SemicolonTree::munch(InstructionList& instrs) const {
    pop_into(instrs, "$v0", "discard return value of previous expr");
}

void Fragment::munch(InstructionList& instrs) const {
    for (auto a : content_) {
        a->munch(instrs);
    }
}

void NotImplExprTree::munch(InstructionList& instrs) const {
    /* don't call this */
}

void NotImplStmtTree::munch(InstructionList& instrs) const {
    /* don't call this */
}

InstructionList ProgramFragment::munch() const {
    InstructionList result;
    result.push_back(new ASMLabel(new Label("main", false)));
    if (data_segment != NULL) {
        data_segment->munch(result);
    }
    text_segment->munch(result);
    return result;
}

} //NAMESPACE
