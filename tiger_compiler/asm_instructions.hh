#ifndef ASM_INSTRUCTION_H
#define ASM_INSTRUCTION_H

#include <string>
#include <vector>

#include "temp.hh"

namespace tiger {

class ASMInstruction {
    ASMInstruction();
    virtual ~ASMInstruction() = default;

};

typedef std::vector<ASMInstructions*> programInstructions;

class ASMOperation : public ASMInstruction {
 public:
    ASMOperation();
    virtual ~ASMOperation() = default;

};

class ASMMove : public ASMInstruction{
 public:
    ASMMove();
    virtual ~ASMMove();

    virtual bool isMove() {return true;}

    Temp *getSrc() { return src; }
    Temp *getDst() { return dst; }

 private:
    Temp *dst;
    Temp *src;
};

class ASMLabel : public Instruction
{
 public:
    ASMLabel(const std::string &assem, Label *label);
    virtual ~ASMLabel() = default;

    virtual bool isLabel() {return true;}
    Label *getLabel() {return label_;}
 private:
    Label *label_;
};

}

#endif //ASM_INSTRUCTIONS_H
