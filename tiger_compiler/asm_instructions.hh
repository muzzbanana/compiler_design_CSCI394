#ifndef ASM_INSTRUCTION_H
#define ASM_INSTRUCTION_H

#include <string>
#include <sstream>
#include <vector>

#include "temp.hh"

using namespace std;

namespace tiger {

inline void pad_width(stringstream *ss, unsigned width) {
    auto len = ss->str().length();
    for (auto i = len; i < width; i++) {
        (*ss) << " ";
    }
}

class ASMInstruction {
    public:
        ASMInstruction(const string instruction, string comment)
            : instruction_(instruction), comment_(comment) { }
        virtual ~ASMInstruction() = default;

        virtual string toStr() {
            stringstream ss;
            ss << instruction_;
            if (comment_.length() > 0) {
                pad_width(&ss, 23);
                ss << " # " << comment_;
            }
            return ss.str();
        }

        const string instruction_;

        string comment_;

        bool generated_push_ = false;
        bool generated_pop_ = false;
        bool generated_move_sp_ = false;
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
        pad_width(&ss, 23);
        ss << " # " << comment_;
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
        pad_width(&ss, 23);
        ss << " # " << comment_;
        return ss.str();
    }

    vector<string> args_;
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
        pad_width(&ss, 23);
        ss << " # " << comment_;
        return ss.str();
    }
 private:
    Label *label_;
};

}//namespace

#endif //ASM_INSTRUCTIONS_H
