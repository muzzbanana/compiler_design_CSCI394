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
            STMT,
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

        enum class Operator {
            EQ, NE, LT, GT, LE, GE, PLUS, MINUS, MUL, DIV
        };

        IRTree(TreeType type);
        virtual ~IRTree() = default;

        TreeType getType() { return type_; }

        bool isExpr() const { return is_expr_; }

        virtual string toStr() const = 0;

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
};

class StmtExprTree : public ExprTree {
    /* Wrap a statement as an expr, so we can use MOVE and whatnot
     * as part of expressions. */
    public:
        StmtExprTree(const StmtTree *stmt);

        const StmtTree *stmt_;

        string toStr() const;
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
};

class NameTree;

class CallTree : public ExprTree {
    public:
        CallTree(NameTree *name, const ExprList args);
        ~CallTree() = default;

        NameTree *name_;
        ExprList args_;

        string toStr() const;
};

class ConstTree : public ExprTree {
    public:
        ConstTree(int value);
        ~ConstTree() = default;

        int value_;

        string toStr() const;
};

class ExprSeqTree : public ExprTree {
    public:
        ExprSeqTree(const StmtTree *stmt, const ExprTree *expr);
        ~ExprSeqTree() = default;

        const StmtTree *stmt_;
        const ExprTree *expr_;

        string toStr() const;
};

class MemTree : public ExprTree {
    public:
        MemTree(const ExprTree *expr);
        ~MemTree() = default;

        const ExprTree *expr_;

        string toStr() const;
};

class NameTree : public ExprTree {
    public:
        NameTree(Label *label);
        ~NameTree() = default;

        Label *label_;

        string toStr() const;
};

class TempTree : public ExprTree {
    public:
        TempTree(Temp *temp);
        ~TempTree() = default;

        Temp *temp_;

        string toStr() const;
};

/* ===== STATEMENT TREES ===== */

class NotImplStmtTree : public StmtTree {
    public:
        NotImplStmtTree() : StmtTree(IRTree::TreeType::NOTIMPL) {}
        ~NotImplStmtTree() = default;
        string toStr() const { return "<not implemented Stmt>"; };
};

class ExprStmtTree : public StmtTree {
    public:
        ExprStmtTree(const ExprTree *expr);
        ~ExprStmtTree() = default;

        const ExprTree *expr_;

        string toStr() const;
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
};

class UJumpTree : public StmtTree {
    public:
        UJumpTree(Label *label);
        ~UJumpTree() = default;

        Label *label_;

        string toStr() const;
};

class ReturnTree : public StmtTree {
    public:
        ReturnTree(const ExprTree *expr);
        ~ReturnTree() = default;

        const ExprTree *expr_;

        string toStr() const;
};

class LabelTree : public StmtTree {
    public:
        LabelTree(Label *l);
        ~LabelTree() = default;

        Label *l_;

        string toStr() const;
};

class MoveTree : public StmtTree {
    public:
        MoveTree(const ExprTree *dest, const ExprTree *src);
        ~MoveTree() = default;

        const ExprTree *dest_;
        const ExprTree *src_;

        string toStr() const;
};

class SeqTree : public StmtTree {
    public:
        SeqTree();
        SeqTree(const StmtTree *left, const StmtTree *right);
        ~SeqTree() = default;

        const StmtTree *left_;
        const StmtTree *right_;

        string toStr() const;
};

}//namespace

#endif
