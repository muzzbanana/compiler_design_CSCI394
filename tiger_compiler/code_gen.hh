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
    void emit(Instruction *instruction);
    InstructionList instruction_list_;
};

}//namespace tiger
#endif //CODE_GEN_H
