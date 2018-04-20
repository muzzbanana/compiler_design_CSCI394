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
    virtual std::map tempspmap; //to add a temp, tempspmap.emplace(<name>, <sp>) to erase it, tempspmap.erase(tempspmap.find(<name))

protected:
    void munchStmtTree(const IRTree::StmtTree *stmt);
    void munchSeqTree(const IRTree::StmtTree *lhs, const IRTree::Stmt *rhs);
    Temp *munchExprTree(const IRTree::ExprTree *expr);

    virtual void munchMoveTree(const IRTree::ExprTree *dst, const IRTree::ExprTree *src) = 0;
    virtual void munchLabelTree(const Label *label) = 0;
    virtual void munchUJumpTree(const Label *label) = 0;
    virtual void munchCJumpTree(const IRTree::CJumpTree *cj) = 0;
    virtual void munchArgs(const IRTree::ExpList &exps, TempList *tsrc) = 0;

    virtual Temp *munchMemTree(IRTree::MemTree *m) = 0;
    virtual Temp *munchBinOpTree(IRTree::BinOpTree *b) = 0;
    virtual Temp *munchCallTree(IRTree::CallTree *c) = 0;
    virtual Temp *munchConstTree(IRTree::ConstTree *c) = 0;
    virtual Temp *munchTempTree(IRTree::TempTree *t) = 0;
    virtual Temp *munchNameTree(IRTree::NameTree *n) = 0;

    void emit(Instruction *instruction);
    InstructionList instruction_list_;
};

}//namespace tiger
#endif //CODE_GEN_H
