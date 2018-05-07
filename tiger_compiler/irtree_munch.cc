#include "irtree.hh"
#include <iostream>

namespace tiger {

using tt = IRTree::TreeType;

/* Number of arguments that are currently being passed to a function */
int current_argcount = 0;

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
    instrs.push_back(new ASMMove("add", addargs, " . . . "));
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
    instrs.push_back(new ASMMove("lw", getargs2, " . . . "));

    vector<string> addargs;
    addargs.push_back("$sp");
    addargs.push_back("$sp");
    addargs.push_back("8");
    instrs.push_back(new ASMMove("add", addargs, " . . . "));
}

/* Generate instructions to perform some operation on the top two values
 * of the stack, and push the result onto the stack. */
void do_op(InstructionList& instrs, string op, string cmt) {
    vector<string> getargs1;
    getargs1.push_back("$t0");
    getargs1.push_back("4($sp)");
    instrs.push_back(new ASMMove("lw", getargs1, cmt));

    vector<string> getargs2;
    getargs2.push_back("$t1");
    getargs2.push_back("($sp)");
    instrs.push_back(new ASMMove("lw", getargs2, " . . . "));

    vector<string> opargs;
    opargs.push_back("$t0");
    opargs.push_back("$t0");
    opargs.push_back("$t1");
    instrs.push_back(new ASMMove(op, opargs, " . . . "));

    vector<string> putargs;
    putargs.push_back("$t0");
    putargs.push_back("4($sp)");
    instrs.push_back(new ASMMove("sw", putargs, " . . . "));

    vector<string> addargs;
    addargs.push_back("$sp");
    addargs.push_back("$sp");
    addargs.push_back("4");
    instrs.push_back(new ASMMove("add", addargs, " . . . "));
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
    instrs.push_back(new ASMMove("sw", putargs, " . . . "));
}

void FragMove::munch(InstructionList& instrs) const {
    string command;
    vector<string> args;
    if (src_->getType() == tt::BINOP) {
        const BinOpTree *bo = dynamic_cast<const BinOpTree*>(src_);
        if (bo->op_ != IRTree::Operator::MUL) {
            if (bo->op_ == IRTree::Operator::PLUS) {
                command = "add";
            } else if (bo->op_ == IRTree::Operator::MINUS) {
                command = "sub";
            } else if (bo->op_ == IRTree::Operator::DIV) {
                command = "div";
            } else {
                command = "??";
            }
            do_op(instrs, command, toStr());
        } else {
            /* apparently mips multiplication is kind of complicated... */
            command = "mul";
            pop_into(instrs, "$t0", toStr());
            pop_into(instrs, "$t1", " . . . ");
            args.push_back("$t0");
            args.push_back("$t1");
            instrs.push_back(new ASMOperation("mult", args, " . . . "));
            vector<string> args2;
            args2.push_back("$t0");
            instrs.push_back(new ASMOperation("mflo", args2, " . . . "));
            push_from(instrs, "$t0", " . . . ");
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
         * and the thing at 4($fp) is the old frame * pointer value --
         * argument # 0 is at 8($fp). */
        int offset = dynamic_cast<const VarTree*>(src_)->offset_;
        if (offset < 0) {
            offset -= 1;
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
    /* not sure if we need this one ? i don't think so */
}

void TempTree::munch(InstructionList& instrs) const {
    /* never mind */
}

void VarTree::munch(InstructionList& instrs) const {
    /* don't need */
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
    /* j/k I think we should just pass them all on the stack tho */
    /* We will save the pointer-to-arguments-location-in-stack in register
     * $s1. this is callee-saved, so we need to save it, but since we don't
     * use it for anything else we can be confident it won't change. */
    push_from(instrs, "$s1", "save prior s1 value");

    /* save number of arguments being passed to current function */
    current_argcount = num_args_;

    /* Reserve space for arguments */
    if (num_args_ > 0) {
        vector<string> args2;
        args2.push_back("$sp");
        args2.push_back("$sp");
        int num = -4*(num_args_);
        args2.push_back(to_string(num));
        instrs.push_back(new ASMOperation("add", args2, "#increments stack for new frame's args"));
    }

    vector<string> argsM;
    argsM.push_back("$s1");
    argsM.push_back("$sp");
    instrs.push_back(new ASMOperation("move", argsM, "save location of arguments"));
}

void ArgPutTree::munch(InstructionList& instrs) const {
    /* put arg in register or stack */
    /* NOTE: I think we actually can just leave the arguments on the stack where they are
     * with no need to calculate where they go. But we will have to be mindful then that
     * they're actually backwards... like arg 0 is below arg 1 on the stack. This can be
     * fixed in the fragmove var part, though. */
    pop_into(instrs, "$t1", "get argument value");

    vector<string> args;
    args.push_back("$t0");
    args.push_back("$s1");
    args.push_back(to_string(4*(current_argcount-index_-1)));
    instrs.push_back(new ASMOperation("add", args, "#figures out where to put the current argument"));

    vector<string> args2;
    args2.push_back("$t1");
    args2.push_back("($t0)");
    instrs.push_back(new ASMOperation("sw", args2, "#loads the argument into appropriate slot"));
}

void ArgRemoveTree::munch(InstructionList& instrs) const {
    /* remove args from stack (undo ArgReserve) */
    /* first save return value of function */
    pop_into(instrs, "$t0", "save returned func val");
    /* remove arguments from stack */
    vector<string> args;
    args.push_back("$sp");
    args.push_back("$s1");
    instrs.push_back(new ASMOperation("move", args, "#returns sp to the top of argument list"));
    /* Now we're back at the top of the arglist, so we just increase $sp by current_argcount to get back to where we were */
    vector<string> args2;
    args2.push_back("$sp");
    args2.push_back("$sp");
    args2.push_back(to_string(4*current_argcount));
    instrs.push_back(new ASMOperation("addi", args2, "remove arguments from stack"));
    /* Now we just have to restore $s1 to its rightful value */
    pop_into(instrs, "$s1", "restore $s1 value");
    /* put func return value back on top of stack */
    push_from(instrs, "$t0", "put back returned func val");
}

void StaticStringTree::munch(InstructionList& instrs) const {
    /* a static string i forget the syntax for this tho. .asciiz something */
    instrs.push_back(new ASMInstruction(label_->toStr() + ": .asciiz \"" + value_ + "\"", ""));
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
    if (data_segment != NULL) {
        result.push_back(new ASMInstruction(".data", ""));
        data_segment->munch(result);
        result.push_back(new ASMInstruction(".text", ""));
    }
    result.push_back(new ASMLabel(new Label("main", false)));
    text_segment->munch(result);
    return result;
}

} //NAMESPACE
