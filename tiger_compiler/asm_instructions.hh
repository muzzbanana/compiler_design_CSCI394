#ifndef ASM_INSTRUCTION_H
#define ASM_INSTRUCTION_H

#include <string>
#include <vector>

#include "temp.hh"

namespace tiger {

class ASMInstruction {
    ASMInstruction(const std::string instruction, std::string comment);
    virtual ~ASMInstruction() = default;

};

// typedef std::vector<ASMInstructions*> InstructionList;

class ASMOperation : public ASMInstruction {
 public:
    ASMOperation(const std::string instruction, const std::vector<std::string> arguments, const std::string comment);
    virtual ~ASMOperation() = default;

    virtual bool isMove() { return false; }
    virtual bool isLabel() { return false; }
    virtual bool isJump() { return false; }
private:
    const std::string instruction_; // things like 'ADD', 'ADDI',...
    const std::string lhs_;         // left hand side of operation
    const std::string rhs_;         // right hand side of operation
    const std::string dst_;         // store result here
};

class ASMMove : public ASMInstruction {
 public:
    ASMMove(const std::string instruction, const std::vector<std::string> arguments, const std::string comment);
    virtual ~ASMMove();

    virtual bool isMove() {return true;}

    Temp *getSrc() { return src; }
    Temp *getDst() { return dst; }

 private:
    const std::string instruction_;
    const std::string src_;
    const std::string dst_;
};

class ASMLabel : public ASMInstruction {
 public:
    ASMLabel(const std::string &assem, Label *label);
    virtual ~ASMLabel() = default;

    virtual bool isLabel() {return true;}
    Label *getLabel() {return label_;}
 private:
    Label *label_;
};

class ASMJump : public ASMInstruction {
 public:
    ASMJump(const std::string instruction, Label *label);
    virtual ~ASMJump() = default;

    virtual bool isJump() {return true;}
    Label *getLabel() {return label_;}
 private:
    Label *label_;
};

class ASMCall : public ASMInstruction {
 public:
    ASMCall(const std::string instruction, Label *label);
    virtual ~ASMCall() = default;

    virtual bool isCall() {return true;}
    Label *getLabel() {return label_;}
 private:
    Label *label_;
};

}//namespace

#endif //ASM_INSTRUCTIONS_H
