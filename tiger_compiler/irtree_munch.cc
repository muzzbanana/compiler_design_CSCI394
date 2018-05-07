#include "irtree.hh"
#include <stack>
#include <iostream>

namespace tiger {

using tt = IRTree::TreeType;

/* --- HELPFUL FUNCTIONS --- */

/* Number of arguments that are currently being passed to a function */
/* This is a stack so we properly keep track of the number of arguments
 * being passed to nested function calls (i.e. f(g(a), h(b,c,d)) ) */
stack<int> current_argcount;

/* Generate a 'sw', 'lw', etc. instruction. (taking 2 parameters) */
void do_move(InstructionList& instrs, string cmd, string dest, string src, string cmt) {
    if (dest != src) {
        vector<string> args;
        args.push_back(dest);
        args.push_back(src);
        instrs.push_back(new ASMMove(cmd, args, cmt));
    } else {
        /* Otherwise... don't do anything. But add the comment to the previous thing to
         * note that we tried */
        instrs.back()->comment_ = instrs.back()->comment_ + " / " + cmt;
    }
}

/* Generate a 'add', 'sub', etc. instruction. (that takes 3 parameters) */
void op_instr(InstructionList& instrs, string cmd, string dest, string src1, string src2, string cmt) {
    vector<string> args;
    args.push_back(dest);
    args.push_back(src1);
    args.push_back(src2);
    instrs.push_back(new ASMMove(cmd, args, cmt));
}

/* Generate instruction to increase stack pointer by four. */
void incr_stack(InstructionList& instrs, string cmt) {
    string comment = cmt;
    if (instrs.size() > 0
            && instrs.back()->instruction_ == "sw"
            && dynamic_cast<const ASMMove*>(instrs.back())->args_[1] == "($sp)") {
        /* If we just stored something on top of stack, erase it -- we are about to clobber it. */
        comment = instrs.back()->comment_ + " / " + comment;
        instrs.pop_back();
    }
    op_instr(instrs, "add", "$sp", "$sp", "4", comment);
}

/* Generate instruction(s) to load the top of stack into
 * the supplied register. */
void pop_into(InstructionList& instrs, string reg, string cmt) {
    if (instrs.size() > 1
            && instrs[instrs.size()-2]->generated_push_
            && instrs.back()->generated_push_) {
        /* Detect if we immediately just pushed something, and replace with a 'move'
         * instead of pushing onto stack and immediately popping back off */
        ASMInstruction *last = instrs.back();
        instrs.pop_back();
        instrs.pop_back();
        string newcmt;
        newcmt = last->comment_ + " / " + cmt;
        do_move(instrs, "move", reg, dynamic_cast<const ASMMove*>(last)->args_[0], newcmt);
        instrs.back()->generated_pop_ = true;
    } else {
        string add_comment = " . . .";
        if (instrs.size() > 0
                && instrs.back()->instruction_ == "sw"
                && instrs.back()->generated_push_
                && dynamic_cast<const ASMMove*>(instrs.back())->args_[0] == reg) {
            /* If we just stored the same register onto the top of the stack, there's no need
             * to load it again. */
            add_comment = cmt;
        } else {
            do_move(instrs, "lw", reg, "($sp)", cmt);
            instrs.back()->generated_pop_ = true;
        }
        incr_stack(instrs, add_comment);
        instrs.back()->generated_pop_ = true;
    }
}

/* Pop into two registers at once to save on stack pointer math. */
/* (top of stack -> reg1, next value -> reg2) */
void pop2_into(InstructionList& instrs, string reg1, string reg2, string cmt) {
    do_move(instrs, "lw", reg1, "($sp)", cmt);
    do_move(instrs, "lw", reg2, "4($sp)", " . . .");
    op_instr(instrs, "add", "$sp", "$sp", "8", " . . .");
}

/* Generate instructions to perform some operation on the top two values
 * of the stack, and push the result onto the stack. */
void do_op(InstructionList& instrs, string op, string cmt) {
    do_move(instrs, "lw", "$t0", "4($sp)", cmt);
    do_move(instrs, "lw", "$t1", "($sp)", " . . .");
    op_instr(instrs, op, "$t0", "$t0", "$t1", " . . .");
    incr_stack(instrs, " . . .");
    instrs.back()->generated_pop_ = true;
    do_move(instrs, "sw", "$t0", "($sp)", " . . .");
    instrs.back()->generated_push_ = true;
}

/* Generate instructions to push a given register onto the stack. */
void push_from(InstructionList& instrs, string reg, string cmt) {
    string sw_comment = " . . .";
    if (instrs.size() > 0
            && instrs.back()->generated_pop_
            && instrs.back()->instruction_ == "add"
            && dynamic_cast<const ASMMove*>(instrs.back())->args_[2] == "4") {
        /* The last thing was adding to the stack pointer,
         * so we don't need to subtract from it again --
         * just get rid of the last instruction */
        sw_comment = instrs.back()->comment_ + " / " + cmt;
        instrs.pop_back();
    } else {
        op_instr(instrs, "add", "$sp", "$sp", "-4", cmt);
        instrs.back()->generated_push_ = true;
    }
    if (instrs.size() > 0
            && instrs.back()->generated_push_
            && instrs.back()->instruction_ == "sw") {
        /* If there's a 'sw something, ($sp)' right before us,
         * then it can't possibly be useful, since we're about
         * to overwrite it. Thus it can be removed. */
        sw_comment = instrs.back()->comment_ + " / " + sw_comment;
        instrs.pop_back();
    }
    if (instrs.size() > 0
            && instrs.back()->generated_pop_
            && instrs.back()->instruction_ == "lw"
            && dynamic_cast<const ASMMove*>(instrs.back())->args_[0] == reg) {
        /* If we just loaded the same register from the stack, there's no
         * need to push the same thing onto the stack again. */
        instrs.back()->comment_ = instrs.back()->comment_ + " / " + sw_comment;
    } else {
        do_move(instrs, "sw", reg, "($sp)", sw_comment);
        instrs.back()->generated_push_ = true;
    }
}

/* Put 'print' and 'print_int' into the thing so we can print stuff out */
void munch_stdlib(InstructionList& instrs) {
    Label *printcommonlabel = new Label("__print_common");

    vector<string> rargs;
    rargs.push_back("$ra");

    instrs.push_back(new ASMLabel(new Label("print", false)));
    do_move(instrs, "li", "$v0", "4", "Load system call code");
    instrs.push_back(new ASMJump("j", printcommonlabel));

    instrs.push_back(new ASMLabel(new Label("print_int", false)));
    do_move(instrs, "li", "$v0", "1", "Load system call code");
    instrs.push_back(new ASMJump("j", printcommonlabel));

    instrs.push_back(new ASMLabel(printcommonlabel));
    pop_into(instrs, "$a0", "get thing to print");
    instrs.push_back(new ASMInstruction("syscall", "Do it!!"));
    push_from(instrs, "$0", "return nothing"); /* return nothing */
    instrs.push_back(new ASMOperation("jr", rargs, ""));
}

/* ---- MUNCH FUNCS ---- */

void FragMove::munch(InstructionList& instrs) const {
    string command;
    vector<string> args;
    if (src_->getType() == tt::BINOP) {
        const BinOpTree *bo = dynamic_cast<const BinOpTree*>(src_);
        if (bo->op_ != IRTree::Operator::DIV) {
            if (bo->op_ == IRTree::Operator::PLUS) {
                command = "add";
            } else if (bo->op_ == IRTree::Operator::MINUS) {
                command = "sub";
            } else if (bo->op_ == IRTree::Operator::MUL) {
                command = "mul";
            } else {
                command = "??";
            }
            do_op(instrs, command, toStr());
        } else {
            /* apparently mips division is kind of complicated... */
            command = "div";
            /* Pop backwards because second operand will be on top of stack */
            pop2_into(instrs, "$t1", "$t0", toStr());
            args.push_back("$t0");
            args.push_back("$t1");
            instrs.push_back(new ASMOperation("div", args, " . . ."));
            vector<string> args2;
            args2.push_back("$t0");
            instrs.push_back(new ASMOperation("mflo", args2, " . . ."));
            push_from(instrs, "$t0", " . . .");
        }
        /* I'm pretty sure that the way it's set up guarantees the
         * arguments to each binop are always on top. So we just need
         * to move the top two arguments from the stack to $t0 + $t1,
         * then do the thing, then store it back on the stack. */
    } else if (src_->getType() == tt::VAR) {
        command = "lw";
        args.push_back("$t0");
        /* Note: we need to subtract an extra 1 from negative arguments,
         * because the thing actually stored at ($fp) is the return address,
         * and the thing at 4($fp) is the old frame pointer value --
         * argument # 0 is at 8($fp). */
        int offset = dynamic_cast<const VarTree*>(src_)->offset_;
        if (offset < 0) {
            offset --;
        }
        args.push_back(to_string(-4*offset) + "($fp)");
        instrs.push_back(new ASMMove(command, args, toStr()));
        push_from(instrs, "$t0", "push var value on stack");
    } else if (dest_->getType() == tt::VAR) {
        pop_into(instrs, "$t0", "get value to put");
        command = "sw";
        args.push_back("$t0");
        int offset = dynamic_cast<const VarTree*>(dest_)->offset_;
        if (offset < 0) {
            offset -= 2;
        }
        args.push_back(to_string(-4*offset) + "($fp)");
        instrs.push_back(new ASMMove(command, args, toStr()));
    } else if (src_->getType() == tt::CALL) {
        command = "jal";
        args.push_back(dynamic_cast<const CallTree*>(src_)->name_->toStr());
        instrs.push_back(new ASMMove(command, args, toStr()));
        /* Put return value on top of stack */
        push_from(instrs, "$v0", "Push return value");
    } else if (src_->getType() == tt::CONST) {
        command = "li";
        args.push_back("$t0");
        args.push_back(to_string(dynamic_cast<const ConstTree*>(src_)->value_));
        instrs.push_back(new ASMMove(command, args, toStr()));
        push_from(instrs, "$t0", " (put on stack...)");
    } else if (src_->getType() == tt::NAME) {
        command = "la";
        args.push_back("$t0");
        args.push_back(dynamic_cast<const NameTree*>(src_)->label_->toStr());
        instrs.push_back(new ASMMove(command, args, toStr()));
        push_from(instrs, "$t0", " (put on stack...)");
    } else {
        command = "move";
        args.push_back(dest_->toStr());
        args.push_back(src_->toStr());
        instrs.push_back(new ASMMove(command, args, toStr()));
    }
}

/* statement trees */

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
    instrs.push_back(new ASMMove(command, args, " . . ."));

    /* We need to unconditionally branch to false label now if
     * we didn't jump to true label. */
    instrs.push_back(new ASMJump("j", f_));
}

void UJumpTree::munch(InstructionList& instrs) const {
    instrs.push_back(new ASMJump("j", label_));
}

void ReturnTree::munch(InstructionList& instrs) const {
    /* Return value will be moved when we encounter END FRAME */
    vector<string> ret_args;
    ret_args.push_back("$ra");
    instrs.push_back(new ASMOperation("jr", ret_args, toStr()));
}

void LabelTree::munch(InstructionList& instrs) const {
    instrs.push_back(new ASMLabel(l_));
}

/* this is all function stack stuff hmm */
void NewFrameTree::munch(InstructionList& instrs) const {
    /* Set frame pointer to point to base of stack frame (which is current $sp - 4) */
    /* it's $sp-4 because we need 0($fp) to be the first local, -4($fp) second local, etc. */
    push_from(instrs, "$fp", "save old frame pointer");
    op_instr(instrs, "sub", "$fp", "$sp", "4", "point at base of frame");
    /* needs to expand stack to hold however many local vars we need */
    do_move(instrs, "move", "$t0", "$sp", "#saves the current return address as a stack pointer");
    op_instr(instrs, "add", "$sp", "$sp", to_string(-4*(num_locals_+1)), "#increments stack for new frame's locals");

    do_move(instrs, "sw", "$t0", "($sp)", "#puts the return address on the top of the stack");
    /* Need to explicitly save return address on stack because MIPS */
    push_from(instrs, "$ra", "save function return addr");
}

void EndFrameTree::munch(InstructionList& instrs) const {
    /* needs to pop off those local vars (+ maybe temps if there are any left) */
    /* Put last temp value into $v0 in case we're returning afterwards */
    pop_into(instrs, "$v0", "load return value");
    /* Need to explicitly save return address on stack because MIPS */
    pop_into(instrs, "$ra", "recall function return addr");
    /* Restore old frame pointer */
    do_move(instrs, "lw", "$fp", "4($sp)", "restore old frame pointter");
    /* Restore stack pointer (hopefully this lines up properly?!) */
    do_move(instrs, "lw", "$sp", "($sp)", "#returns sp to the return address");
}

void ArgReserveTree::munch(InstructionList& instrs) const {
    /* expand stack to fit N-4 number of arguments (we can pass args 1-4 in $a0-$a3) */
    // if we want to load the first four arguments in this fragment we should do this here
    /* j/k I think we should just pass them all on the stack tho */
    /* We will save the pointer-to-arguments-location-in-stack in register
     * $s1. this is callee-saved, so we need to save it, but since we don't
     * use it for anything else we can be confident it won't change. */
    push_from(instrs, "$s1", "save prior s1 value");

    /* save number of arguments being passed to current function */
    current_argcount.push(num_args_);

    /* Reserve space for arguments */
    if (num_args_ > 0) {
        op_instr(instrs, "add", "$sp", "$sp", to_string(-4*(num_args_)), toStr());
    }

    do_move(instrs, "move", "$s1", "$sp", "save location of arguments");
}

void ArgPutTree::munch(InstructionList& instrs) const {
    /* put arg in register or stack */
    pop_into(instrs, "$t0", "get argument value");

    op_instr(instrs, "add", "$t1", "$s1", to_string(4*(current_argcount.top()-index_-1)), "#figures out where to put the current argument");

    do_move(instrs, "sw", "$t0", "($t1)", "#loads the argument into appropriate slot");
}

void ArgRemoveTree::munch(InstructionList& instrs) const {
    /* remove args from stack (undo ArgReserve) */
    /* first save return value of function */
    pop_into(instrs, "$v0", "save returned func val");
    /* remove arguments from stack */
    do_move(instrs, "move", "$sp", "$s1", "#returns sp to the top of argument list");
    /* Now we're back at the top of the arglist, so we just increase $sp by current_argcount.back() to get back to where we were */
    op_instr(instrs, "add", "$sp", "$sp", to_string(4*current_argcount.top()), toStr());
    current_argcount.pop();
    /* Now we just have to restore $s1 to its rightful value */
    pop_into(instrs, "$s1", "restore $s1 value");
    /* put func return value back on top of stack */
    push_from(instrs, "$v0", "put back returned func val");
}

void StaticStringTree::munch(InstructionList& instrs) const {
    /* static string */
    instrs.push_back(new ASMInstruction(label_->toStr() + ": .asciiz \"" + value_ + "\"", ""));
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
    cerr << "not implemented Expr!" << endl;
}

void NotImplStmtTree::munch(InstructionList& instrs) const {
    /* don't call this */
    cerr << "not implemented Stmt!" << endl;
}

InstructionList ProgramFragment::munch() const {
    InstructionList result;
    if (data_segment != NULL) {
        result.push_back(new ASMInstruction(".data", ""));
        data_segment->munch(result);
        result.push_back(new ASMInstruction(".text", ""));
    }
    result.push_back(new ASMLabel(new Label("main", false)));
    text_segment->munch(result);
    munch_stdlib(result);
    return result;
}

/* Trees that have no munch... */
void CallTree::munch(InstructionList& instrs) const { }
void ConstTree::munch(InstructionList& instrs) const { }
void ExprSeqTree::munch(InstructionList& instrs) const { }
void MemTree::munch(InstructionList& instrs) const { }
void NameTree::munch(InstructionList& instrs) const { }
void TempTree::munch(InstructionList& instrs) const { }
void VarTree::munch(InstructionList& instrs) const { }
void ConditionalExprTree::munch(InstructionList& instrs) const { }
void ExprStmtTree::munch(InstructionList& instrs) const { }
void SeqTree::munch(InstructionList& instrs) const { }
void MoveTree::munch(InstructionList& instrs) const { }
void StmtExprTree::munch(InstructionList& instrs) const { }
void BinOpTree::munch(InstructionList& instrs) const { }

} //NAMESPACE
