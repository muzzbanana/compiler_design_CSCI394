#ifndef _IRTREE_HH_
#define _IRTREE_HH_

#include <sstream>
#include <iostream>
#include "fragment.hh"
#include "frame.hh"
#include "temp.hh"
#include "code_gen.hh"
#include "asm_instructions.hh"

using namespace std;

namespace tiger {

class ExprTree;
typedef vector<const ExprTree*> ExprList;

class Fragment;

class IRTree {
    public:
        enum class TreeType {
            STMT,       /* a statement wrapped in an expr */
            BINOP,      /* a binary operation +, -, *, /, <, >, <=, >=, <>, = */
            CALL,       /* a function call */
            CONST,      /* a constant int */
            ESEQ,       /* an expression-sequence */
            MEM,        /* memory access? not used at the moment */
            NAME,       /* a name representing data at a label */
            TEMP,       /* a temp */
            VAR,        /* access to a variable on the stack */
            COND,       /* conditional expression */
            EXPR,       /* an expression wrapped in a stmt */
            CJUMP,      /* a conditional jump */
            UJUMP,      /* an unconditional jump */
            RETURN,     /* return a temp */
            LABEL,      /* a label */
            MOVE,       /* move data from somewhere to somewhere else */
            NEWFRAME,   /* new frame with new frame pointer etc */
            ENDFRAME,   /* pop a frame off the stack */
            ARGRESERVE, /* reserve space for some arguments */
            ARGPUT,     /* put an argument onto the stack */
            ARGREMOVE,  /* remove arguments from stack */
            STATICSTR,  /* static string data */
            SEQ,        /* a sequence of stmts */
            SEMICOLON,  /* throw away a return value */

            NOTIMPL,    /* not implemented yet */

            VECTORIZED, /* has been flattened into a Fragment */
            VECMOVE,    /* special restricted kind of move tree for use in fragments */
        };

        enum class Operator {
            EQ, NE, LT, GT, LE, GE, PLUS, MINUS, MUL, DIV
        };

        IRTree(TreeType type);
        virtual ~IRTree() = default;

        TreeType getType() const { return type_; }

        bool isExpr() const { return is_expr_; }

        virtual string toStr() const = 0;

        virtual Fragment *vectorize(const Temp *result) const = 0;

        virtual void munch(InstructionList& instrs) const = 0;

    protected:
        TreeType type_; /* what's its specific type? */
        bool is_expr_;  /* is it expression? (if no, is stmt) */
};

class ExprTree : public IRTree {
    public:
        static const ExprTree *notImpl;

        ExprTree(IRTree::TreeType type);
        virtual string toStr() const = 0;
};

class StmtTree : public IRTree {
    public:
        static const StmtTree *notImpl;

        StmtTree(IRTree::TreeType type);
        virtual string toStr() const = 0;
};

/* ===== EXPRESSION TREES ===== */

class NotImplExprTree : public ExprTree {
    public:
        NotImplExprTree() : ExprTree(IRTree::TreeType::NOTIMPL) {}
        ~NotImplExprTree() = default;
        string toStr() const { return "<not implemented Expr>"; };

        virtual Fragment *vectorize(const Temp *result) const { return NULL; }

        virtual void munch(InstructionList& instrs) const;
};

class StmtExprTree : public ExprTree {
    /* Wrap a statement as an expr, so we can use MOVE and whatnot
     * as part of expressions. */
    public:
        StmtExprTree(const StmtTree *stmt);

        const StmtTree *stmt_;

        string toStr() const;

        virtual Fragment *vectorize(const Temp *result) const;

        virtual void munch(InstructionList& instrs) const;
};

class BinOpTree : public ExprTree {
    public:
        // enum class Operator {
        //     PLUS, MINUS, MUL, DIV
        // };

        BinOpTree(IRTree::Operator op, const ExprTree *left, const ExprTree *right);
        ~BinOpTree() = default;

        IRTree::Operator op_;
        const ExprTree *left_;
        const ExprTree *right_;

        string toStr() const;

        virtual Fragment *vectorize(const Temp *result) const;

        virtual void munch(InstructionList& instrs) const;
};

class NameTree;

class CallTree : public ExprTree {
    public:
        CallTree(NameTree *name, const ExprList args);
        ~CallTree() = default;

        NameTree *name_;
        ExprList args_;

        string toStr() const;

        virtual Fragment *vectorize(const Temp *result) const;

        virtual void munch(InstructionList& instrs) const;
};

class ConstTree : public ExprTree {
    public:
        ConstTree(int value);
        ~ConstTree() = default;

        int value_;

        string toStr() const;

        virtual Fragment *vectorize(const Temp *result) const;

        virtual void munch(InstructionList& instrs) const;
};

class ExprSeqTree : public ExprTree {
    public:
        ExprSeqTree(const StmtTree *stmt, const ExprTree *expr, bool discard_intermediate = true);
        ~ExprSeqTree() = default;

        const StmtTree *stmt_;
        const ExprTree *expr_;

        string toStr() const;

        virtual Fragment *vectorize(const Temp *result) const;

        virtual void munch(InstructionList& instrs) const;

    private:
        bool discard_intermediate_;
};

class MemTree : public ExprTree {
    public:
        MemTree(const ExprTree *expr);
        ~MemTree() = default;

        const ExprTree *expr_;

        string toStr() const;

        virtual Fragment *vectorize(const Temp *result) const;

        virtual void munch(InstructionList& instrs) const;
};

class NameTree : public ExprTree {
    /* Represents static data that has a label
     * in the code. e.g. string literals. */
    public:
        NameTree(Label *label);
        ~NameTree() = default;

        Label *label_;

        string toStr() const;

        virtual Fragment *vectorize(const Temp *result) const;

        virtual void munch(InstructionList& instrs) const;
};

class TempTree : public ExprTree {
    /* Represents a temporary value for intermediate
     * results in calculations. */
    public:
        TempTree(const Temp *temp);
        ~TempTree() = default;

        const Temp *temp_;

        string toStr() const;

        virtual Fragment *vectorize(const Temp *result) const;

        virtual void munch(InstructionList& instrs) const;
};

class VarTree : public ExprTree {
    /* Represents a variable; has an index
     * representing its offset from the frame
     * pointer (which is negative for arguments,
     * positive for local variables) */
    public:
        VarTree(std::string name, int offset);
        ~VarTree() = default;

        std::string name_;
        int offset_;

        string toStr() const;

        virtual Fragment *vectorize(const Temp *result) const;

        virtual void munch(InstructionList& instrs) const;
};

/* Represents a conditional expression (an if-statement that returns a value.)
 * Decomposes to a CJumpTree + some other stuff when vectorized. */
class ConditionalExprTree : public ExprTree {
    public:
        ConditionalExprTree(IRTree::Operator comp, const ExprTree *left, const ExprTree *right,
                const ExprTree *trueVal, const ExprTree *falseVal);
        ~ConditionalExprTree() = default;

        IRTree::Operator comp_;

        const ExprTree *left_;
        const ExprTree *right_;

        const ExprTree *trueVal_;
        const ExprTree *falseVal_;

        string toStr() const;

        virtual Fragment *vectorize(const Temp *result) const;

        virtual void munch(InstructionList& instrs) const;
};

/* ===== STATEMENT TREES ===== */

class NotImplStmtTree : public StmtTree {
    public:
        NotImplStmtTree() : StmtTree(IRTree::TreeType::NOTIMPL) {}
        ~NotImplStmtTree() = default;
        string toStr() const { return "<not implemented Stmt>"; };

        virtual Fragment *vectorize(const Temp *result) const { return NULL; }

        virtual void munch(InstructionList& instrs) const;
};

class ExprStmtTree : public StmtTree {
    public:
        ExprStmtTree(const ExprTree *expr);
        ~ExprStmtTree() = default;

        const ExprTree *expr_;

        string toStr() const;

        virtual Fragment *vectorize(const Temp *result) const;

        virtual void munch(InstructionList& instrs) const;
};

/* Tree that appears when we throw away the result of an expression. */
/* (Which is represented by the semicolon in Tiger.) */
class SemicolonTree : public StmtTree {
    public:
        SemicolonTree();
        ~SemicolonTree() = default;

        string toStr() const;

        virtual Fragment *vectorize(const Temp *result) const;

        virtual void munch(InstructionList& instrs) const;
};

class CJumpTree : public StmtTree {
    public:
        // enum class Comparison {
        //     EQ, NE, LT, GT, LE, GE,
        // };

        CJumpTree(IRTree::Operator comp, const ExprTree *left, const ExprTree *right, Label *t, Label *f);
        ~CJumpTree() = default;

        IRTree::Operator comp_;
        const ExprTree *left_;
        const ExprTree *right_;
        Label *t_;
        Label *f_;

        string toStr() const;

        virtual Fragment *vectorize(const Temp *result) const;

        virtual void munch(InstructionList& instrs) const;
};

class UJumpTree : public StmtTree {
    public:
        UJumpTree(const Label *label);
        ~UJumpTree() = default;

        const Label *label_;

        string toStr() const;

        virtual Fragment *vectorize(const Temp *result) const;

        virtual void munch(InstructionList& instrs) const;
};

class ReturnTree : public StmtTree {
    public:
        ReturnTree(const ExprTree *expr);
        ~ReturnTree() = default;

        const ExprTree *expr_;

        string toStr() const;

        virtual Fragment *vectorize(const Temp *result) const;

        virtual void munch(InstructionList& instrs) const;
};

class LabelTree : public StmtTree {
    public:
        LabelTree(const Label *l);
        ~LabelTree() = default;

        const Label *l_;

        string toStr() const;

        virtual Fragment *vectorize(const Temp *result) const;

        virtual void munch(InstructionList& instrs) const;
};

class MoveTree : public StmtTree {
    public:
        MoveTree(const ExprTree *dest, const ExprTree *src);
        ~MoveTree() = default;

        const ExprTree *dest_;
        const ExprTree *src_;

        string toStr() const;

        virtual Fragment *vectorize(const Temp *result) const;

        virtual void munch(InstructionList& instrs) const;
};

class NewFrameTree : public StmtTree {
    public:
        NewFrameTree(const int num_locals);
        ~NewFrameTree() = default;

        const int num_locals_;

        string toStr() const;

        virtual Fragment *vectorize(const Temp *result) const;

        virtual void munch(InstructionList& instrs) const;
};

class EndFrameTree : public StmtTree {
    public:
        EndFrameTree();
        ~EndFrameTree() = default;

        string toStr() const;

        virtual Fragment *vectorize(const Temp *result) const;

        virtual void munch(InstructionList& instrs) const;
};

/* For growing stack when we pass arguments. */
class ArgReserveTree : public StmtTree {
    public:
        ArgReserveTree(const int num_args);
        ~ArgReserveTree() = default;

        const int num_args_;

        string toStr() const;

        virtual Fragment *vectorize(const Temp *result) const;

        virtual void munch(InstructionList& instrs) const;
};

/* Put an argument at a certain index */
class ArgPutTree : public StmtTree {
    public:
        ArgPutTree(const int index, const ExprTree *arg);
        ~ArgPutTree() = default;

        const int index_;

        const ExprTree *arg_;

        string toStr() const;

        virtual Fragment *vectorize(const Temp *result) const;

        virtual void munch(InstructionList& instrs) const;
};

/* Undo the last ArgReserve */
class ArgRemoveTree : public StmtTree {
    public:
        ArgRemoveTree(const int amount);
        ~ArgRemoveTree() = default;

        const int amount_;

        string toStr() const;

        virtual Fragment *vectorize(const Temp *result) const;

        virtual void munch(InstructionList& instrs) const;
};

/* A static string pointer */
class StaticStringTree : public StmtTree {
    public:
        StaticStringTree(const Label *label, const string value);
        ~StaticStringTree() = default;

        const Label *label_;
        const string value_;

        string toStr() const;

        virtual Fragment *vectorize(const Temp *result) const;

        virtual void munch(InstructionList& instrs) const;
};

class SeqTree : public StmtTree {
    public:
        SeqTree();
        SeqTree(const StmtTree *left, const StmtTree *right);
        ~SeqTree() = default;

        const StmtTree *left_;
        const StmtTree *right_;

        string toStr() const;

        virtual Fragment *vectorize(const Temp *result) const;

        virtual void munch(InstructionList& instrs) const;
};

/* ===== FRAGMENT ===== */

/* Represents a series of statements that put their
 * result into a temp. */
class Fragment : public IRTree {
    public:
        Fragment(const Temp *result_temp);

        void append(const StmtTree *stmt);
        void concat(const Fragment *vec);

        string toStr() const;

        vector<const StmtTree*> content_;

        const Temp *result_temp_;

        Fragment *vectorize(const Temp *result) const {
            cerr << "Hey this tree was already vectorized!" << endl;
            return NULL;
        }

        virtual void munch(InstructionList& instrs) const;
};

/* Represents a 'move' instruction in a Fragment --
 * can only move (a) tmp to tmp (b) tmp to var
 * (c) var to tmp (d) arithmetic instruction to tmp
 * (e) result of function call to tmp */
class FragMove : public StmtTree {
    public:
        FragMove(const ExprTree *dest, const ExprTree *src);

        Fragment *vectorize(const Temp *result) const {
            cerr << "Hey this was already vectorized!" << endl;
            return NULL;
        }

        string toStr() const;

        const ExprTree *dest_;
        const ExprTree *src_;

        virtual void munch(InstructionList& instrs) const;
};

/* This is what a ProgramTree vectorizes into.
 * It has a 'data segment' and a 'text segment'. */
class ProgramFragment {
    public:
        ProgramFragment() = default;
        ~ProgramFragment() = default;

        const Fragment *data_segment;
        const Fragment *text_segment;

        virtual string toStr() const {
            stringstream ss;
            if (data_segment) {
                ss << "-- DATA SEGMENT --\n";
                ss << data_segment->toStr();
                ss << "\n-- TEXT SEGMENT --\n";
            }
            ss << text_segment->toStr();
            return ss.str();
        }

        InstructionList munch() const;
};

/* This is what gets returned from convert_ast. It has
 * a data segment and a text segment but might have arbitrary
 * nesting of expressions and whatnot. Calling vectorize(const Temp *result)
 * on it will cause it to produce a ProgramFragment. */
class ProgramTree {
    public:
        ProgramTree() = default;
        ~ProgramTree() = default;

        const StmtTree *data_segment;
        const StmtTree *text_segment;

        const ProgramFragment *vectorize() const {
            ProgramFragment *frag = new ProgramFragment;
            if (data_segment) {
                frag->data_segment = data_segment->vectorize(NULL);
            } else {
                frag->data_segment = NULL;
            }
            const Temp *textresult = new Temp();
            frag->text_segment = text_segment->vectorize(textresult);
            return frag;
        }

        virtual string toStr() const {
            stringstream ss;
            if (data_segment) {
                ss << "-- DATA SEGMENT --\n";
                ss << data_segment->toStr();
                ss << "\n-- TEXT SEGMENT --\n";
            }
            ss << text_segment->toStr();
            return ss.str();
        }
};


}//namespace

#endif
