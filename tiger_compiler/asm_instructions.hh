#ifndef ASM_INSTRUCTION_H
#define ASM_INSTRUCTION_H

#include <string>
#include <sstream>
#include <vector>

#include "temp.hh"

using namespace std;

namespace tiger {

class ASMInstruction {
    public:
        ASMInstruction(const string instruction, string comment)
            : instruction_(instruction), comment_(comment) { }
        virtual ~ASMInstruction() = default;

        virtual string toStr() { return instruction_ + " \t; " + comment_; }

    protected:
        const string instruction_;
        const string comment_;

};

// typedef vector<ASMInstructions*> InstructionList;

class ASMOperation : public ASMInstruction {
 public:
    ASMOperation(const string instruction, const vector<string> args, const string comment)
        : ASMInstruction(instruction, comment), args_(args) { }
    virtual ~ASMOperation() = default;

    virtual bool isMove() { return false; }
    virtual bool isLabel() { return false; }
    virtual bool isJump() { return false; }

    virtual string toStr() {
        stringstream ss;
        ss << instruction_;
        ss << " ";
        for (unsigned int i = 0; i < args_.size(); i++) {
            if (i != 0) {
                ss << ", ";
            }
            ss << args_[i];
        }
        ss << " \t; " << comment_;
        return ss.str();
    }
private:
    const vector<string> args_;
};

class ASMMove : public ASMInstruction {
 public:
    ASMMove(const string instruction, const vector<string> args, const string comment)
        : ASMInstruction(instruction, comment), args_(args) { }
    virtual ~ASMMove() { };

    virtual bool isMove() {return true;}

    vector<string> getArgs() { return args_; }

    virtual string toStr() {
        stringstream ss;
        ss << instruction_;
        ss << " ";
        for (unsigned int i = 0; i < args_.size(); i++) {
            if (i != 0) {
                ss << ", ";
            }
            ss << args_[i];
        }
        ss << " \t; " << comment_;
        return ss.str();
    }
 private:
    const vector<string> args_;
};

class ASMLabel : public ASMInstruction {
 public:
    ASMLabel(const Label *label) : ASMInstruction("", ""), label_(label) { }
    virtual ~ASMLabel() = default;

    virtual bool isLabel() {return true;}
    const Label *getLabel() {return label_;}

    virtual string toStr() {
        stringstream ss;
        ss << label_->toStr();
        ss << ":";
        /* NOTE: I forget the mips label syntax exactly (are there dots?) */
        return ss.str();
    }
 private:
    const Label *label_;
};

class ASMJump : public ASMInstruction {
 public:
    ASMJump(const string instruction, const Label *label)
        : ASMInstruction(instruction, ""), label_(label) { }
    virtual ~ASMJump() = default;

    virtual bool isJump() {return true;}
    const Label *getLabel() {return label_;}

    virtual string toStr() {
        stringstream ss;
        ss << instruction_;
        ss << " ";
        ss << label_->toStr();
        return ss.str();
    }
 private:
    const Label *label_;
};

class ASMCall : public ASMInstruction {
 public:
    ASMCall(const string instruction, Label *label);
    virtual ~ASMCall() = default;

    virtual bool isCall() {return true;}
    Label *getLabel() {return label_;}

    virtual string toStr() {
        stringstream ss;
        ss << "jal ";
        ss << label_->toStr();
        ss << " \t; " << comment_;
        return ss.str();
    }
 private:
    Label *label_;
};

}//namespace

#endif //ASM_INSTRUCTIONS_H
