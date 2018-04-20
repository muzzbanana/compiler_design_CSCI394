#ifndef CODE_GEN_H
#define CODE_GEN_H

#include <vector>
#include "irtree.hh"
#include "temp.hh"

namespace tiger {

class Instruction;
typedef std::vector<Instruction*> InstructionList;

class CodeGen {
public:
    CodeGen();
    virtual ~CodeGen() = default

    void generate(IRTree::Stmt *s, InstructionList &instList);

    virtual bool isMove(ASMInstruction *instruction) = 0;
    virtual bool isJump(ASMInstruction *instruction) = 0;

protected:
    void munchStm(IRTree::Stmt *stmt);
    void munchSEQ(IRTree::Stmt *lhs, IRTree::Stmt *rhs);
    Temp *munchExp(IRTree::Expr *expr);

    virtual void munchMOVE(IRTree::Expr *dst, IRTree::Expr *src) = 0;
    virtual void munchLABEL(Label *label) = 0;
    virtual void munchJUMP(Label *label) = 0;
    virtual void munchCJUMP(IRTree::CJUMP *cj) = 0;
    virtual void munchEXPR(IRTree::Expr *Expr) = 0;
    virtual void munchArgs(const IRTree::ExpList &exps, TempList *tsrc) = 0;

    virtual Temp *munchMEM(IRTree::MEM *m) = 0;
    virtual Temp *munchBINOP(IRTree::BINOP *b) = 0;
    virtual Temp *munchCALL(IRTree::CALL *c) = 0;
    virtual Temp *munchCONST(IRTree::CONST *c) = 0;
    virtual Temp *munchTEMP(IRTree::TEMP *t) = 0;
    virtual Temp *munchNAME(IRTree::NAME *n) = 0;

    void emit(Instruction *inst);
    InstructionList ilist;
};

}//namespace tiger
#endif //CODE_GEN_H
