#ifndef CODE_GEN_H
#define CODE_GEN_H

#include <vector>
#include <map>
#include "irtree.hh"
#include "temp.hh"

namespace tiger {

class ASMInstruction;
typedef std::vector<ASMInstruction*> InstructionList;

class CodeGen {
public:
    CodeGen();
    virtual ~CodeGen() = default;

    virtual bool isMove(ASMInstruction *instruction) = 0;
    virtual bool isJump(ASMInstruction *instruction) = 0;
    void emit(InstructionList list);
    /* *** hey this needs angle brackets type after map */
    //virtual std::map tempspmap; //to add a temp, tempspmap.emplace(<name>, <sp>). To erase it, tempspmap.erase(tempspmap.find(<name>))

protected:
    InstructionList instruction_list_;
};

}//namespace tiger
#endif //CODE_GEN_H
