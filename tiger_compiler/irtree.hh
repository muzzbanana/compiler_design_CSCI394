#ifndef _IRTREE_HH_
#define _IRTREE_HH_

#include <sstream>
#include "fragment.hh"
#include "temp.hh"

using namespace std;

namespace tiger {

class ExprTree;
typedef vector<ExprTree*> ExprList;

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
            NOTIMPL,
        };

        IRTree(TreeType type);
        virtual ~IRTree() = default;

        TreeType getType() { return type_; }

        virtual string toStr() = 0;

    protected:
        TreeType type_;
};

class ExprTree : public IRTree {
    public:
        static ExprTree *notImpl;

        ExprTree(IRTree::TreeType type);
        virtual string toStr() = 0;
};

class StmtTree : public IRTree {
    public:
        static StmtTree *notImpl;

        StmtTree(IRTree::TreeType type);
        virtual string toStr() = 0;
};

/* ===== EXPRESSION TREES ===== */

class NotImplExprTree : public ExprTree {
    public:
        NotImplExprTree() : ExprTree(IRTree::TreeType::NOTIMPL) {}
        ~NotImplExprTree() = default;
        string toStr() { return "<not implemented Expr>"; };
};

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

        string toStr();
};

class NameTree;

class CallTree : public ExprTree {
    public:
        CallTree(NameTree *name, const ExprList args);
        ~CallTree() = default;

        NameTree *name_;
        ExprList args_;

        string toStr();
};

class ConstTree : public ExprTree {
    public:
        ConstTree(int value);
        ~ConstTree() = default;

        int value_;

        string toStr();
};

class ExprSeqTree : public ExprTree {
    public:
        ExprSeqTree(StmtTree *stmt, ExprTree *expr);
        ~ExprSeqTree() = default;

        StmtTree *stmt_;
        ExprTree *expr_;

        string toStr();
};

class MemTree : public ExprTree {
    public:
        MemTree(ExprTree *expr);
        ~MemTree() = default;

        ExprTree *expr_;

        string toStr();
};

class NameTree : public ExprTree {
    public:
        NameTree(Label *label);
        ~NameTree() = default;

        Label *label_;

        string toStr();
};

class TempTree : public ExprTree {
    public:
        TempTree(Temp *temp);
        ~TempTree() = default;

        Temp *temp_;

        string toStr();
};

/* ===== STATEMENT TREES ===== */

class NotImplStmtTree : public StmtTree {
    public:
        NotImplStmtTree() : StmtTree(IRTree::TreeType::NOTIMPL) {}
        ~NotImplStmtTree() = default;
        string toStr() { return "<not implemented Stmt>"; };
};

class ExprStmtTree : public StmtTree {
    public:
        ExprStmtTree(ExprTree *expr);
        ~ExprStmtTree() = default;

        ExprTree *expr_;

        string toStr();
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

        string toStr();
};

class UJumpTree : public StmtTree {
    public:
        UJumpTree(Label *label);
        ~UJumpTree() = default;

        Label *label_;

        string toStr();
};

class ReturnTree : public StmtTree {
    public:
        ReturnTree(ExprTree *expr);
        ~ReturnTree() = default;

        ExprTree *expr_;

        string toStr();
};

class LabelTree : public StmtTree {
    public:
        LabelTree(Label *l);
        ~LabelTree() = default;

        Label *l_;

        string toStr();
};

class MoveTree : public StmtTree {
    public:
        MoveTree(ExprTree *dest, ExprTree *src);
        ~MoveTree() = default;

        ExprTree *dest_;
        ExprTree *src_;

        string toStr();
};

class SeqTree : public StmtTree {
    public:
        SeqTree();
        SeqTree(StmtTree *left, StmtTree *right);
        ~SeqTree() = default;

        StmtTree *left_;
        StmtTree *right_;

        string toStr();
};

}//namespace

#endif
