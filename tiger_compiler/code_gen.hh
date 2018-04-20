#ifndef CODE_GEN_H
#define CODE_GEN_H

#include <vector>
#include "irtree.hh"
#include "temp.hh"

namespace tiger {

class ASMInstruction;
typedef std::vector<ASMInstruction*> InstructionList;

class CodeGen {
public:
    CodeGen();
    virtual ~CodeGen() = default

    void generate(IRTree::Stmt *s, InstructionList &instList);

    virtual bool isMove(ASMInstruction *instruction) = 0;
    virtual bool isJump(ASMInstruction *instruction) = 0;
    virtual std::map tempspmap; //to add a temp, tempspmap.emplace(<name>, <sp>). To erase it, tempspmap.erase(tempspmap.find(<name>))

protected:
    void emit(ASMInstruction *instruction);
    InstructionList instruction_list_;
};

}//namespace tiger
#endif //CODE_GEN_H
