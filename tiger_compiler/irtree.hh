#ifndef _IRTREE_HH_
#define _IRTREE_HH_

#include "fragment.hh"
#include "temp.hh"

namespace tiger {

class ExprTree;
typedef std::vector<ExprTree*> ExprList;

class IRTree {
    public:
        enum class TreeType {
            BINOP,
            CALL,
            CONST,
            ESEQ,
            MEM,
            NAME,
            TEMP,
            EXPR,
            CJUMP,
            UJUMP,
            RETURN,
            LABEL,
            MOVE,
            SEQ,
        };

        IRTree(TreeType type);
        virtual ~IRTree() =default;

        TreeType getType() { return type_; }

    protected:
        TreeType type_;
};

class ExprTree : public IRTree {
    public:
        ExprTree(IRTree::TreeType type);
};

class StmtTree : public IRTree {
    public:
        StmtTree(IRTree::TreeType type);
};

/* ===== EXPRESSION TREES ===== */

class BinOpTree : public ExprTree {
    public:
        enum class Operator {
            PLUS, MINUS, MUL, DIV,
        };

        BinOpTree(Operator op, BinOpTree *left, BinOpTree *right);
        ~BinOpTree() = default;

        Operator op_;
        ExprTree *left_;
        ExprTree *right_;
};

class NameTree;

class CallTree : public ExprTree {
    public:
        CallTree(NameTree *name, const ExprList args);
        ~CallTree() = default;

        NameTree *name_;
        ExprList args_;
};

class ConstTree : public ExprTree {
    public:
        ConstTree(int value);
        ~ConstTree() = default;

        int value_;
};

class ExprSeqTree : public ExprTree {
    public:
        ExprSeqTree(StmtTree *stmt, ExprTree *expr);
        ~ExprSeqTree() = default;

        StmtTree *stmt_;
        ExprTree *expr_;
};

class MemTree : public ExprTree {
    public:
        MemTree(ExprTree *expr);
        ~MemTree() = default;

        ExprTree *expr_;
};

class NameTree : public ExprTree {
    public:
        NameTree(Label *label);
        ~NameTree() = default;

        Label *label_;
};

class TempTree : public ExprTree {
    public:
        TempTree(Temp *temp);
        ~TempTree() = default;

        Temp *temp_;
};

/* ===== STATEMENT TREES ===== */

class ExprStmtTree : public StmtTree {
    public:
        ExprStmtTree(ExprTree *expr);
        ~ExprStmtTree() = default;

        ExprTree *expr_;
};

class CJumpTree : public StmtTree {
    public:
        enum class Comparison {
            EQ, NE, LT, GT, LE, GE,
        };

        CJumpTree(Comparison comp, ExprTree *left, ExprTree *right, Label *t, Label *f);
        ~CJumpTree() = default;

        Comparison comp_;
        ExprTree *left_;
        ExprTree *right_;
        Label *t_;
        Label *f_;
};

class UJumpTree : public StmtTree {
    public:
        UJumpTree(Label *label);
        ~UJumpTree() = default;

        Label *label_;
};

class ReturnTree : public StmtTree {
    public:
        ReturnTree(ExprTree *expr);
        ~ReturnTree() = default;

        ExprTree *expr_;
};

class LabelTree : public StmtTree {
    public:
        LabelTree(Label *l);
        ~LabelTree() = default;

        Label *l_;
};

class MoveTree : public StmtTree {
    public:
        MoveTree(ExprTree *dest, ExprTree *src);
        ~MoveTree() = default;

        ExprTree *dest_;
        ExprTree *src_;
};

class SeqTree : public StmtTree {
    public:
        SeqTree();
        SeqTree(StmtTree *left, StmtTree *right);
        ~SeqTree() = default;

        StmtTree *left_;
        StmtTree *right_;
};

}//namespace

#endif
