#ifndef _AST_HH_
#define _AST_HH_

#include <iostream>
#include <cstdio>
#include <sstream>
#include <functional>
#include <string>
#include <cmath>
#include <set>
#include <algorithm>
#include "type.hh"
#include "scope.hh"
#include "irtree.hh"
#include "fragment.hh"

using namespace std;

namespace tiger {

///////////////////////////////////////////////////////////////////////////////
// Base AST node class, to define the hierarchy.
class ASTNode {
    public:
        using value_t = int;  // All values will be floating-point
        using ASTptr = const ASTNode*; // Can't use smart ptr in union :(

        ASTNode() = default;
        virtual ~ASTNode() = default;
        virtual const Type *type_verify(Scope* scope) const = 0; // Determine type of expression
        virtual const IRTree *convert_to_ir(Frame *frame) const = 0; // Convert to IR representation
        virtual value_t eval() const = 0;  // Evaluate expression tree
        virtual std::string toStr() const = 0; // For printing purposes
};

///////////////////////////////////////////////////////////////////////////////
// A node type that evaluates to a single value, NIL
class NilASTNode : public ASTNode {
    public:
        NilASTNode()
            : ASTNode()
        {}
        virtual ~NilASTNode() = default;

        virtual const Type *type_verify(Scope* scope) const {
            return Type::nilType;
        }

        virtual value_t eval() const
        {
            return -1; /// FIX ME
        }

        virtual const IRTree *convert_to_ir(Frame *frame) const {
            return ExprTree::notImpl;
        }

        virtual std::string toStr() const
        {
            return "nil";
        }
};

///////////////////////////////////////////////////////////////////////////////
// A node type representing a "break" statement.
class BreakASTNode : public ASTNode {
    public:
        BreakASTNode(int loc)
            : ASTNode(), location_(loc)
        {}
        virtual ~BreakASTNode() = default;

        virtual const Type *type_verify(Scope* scope) const {
            return Type::nilType;
        }

        virtual value_t eval() const
        {
            return -1; /// FIX ME
        }

        virtual const IRTree *convert_to_ir(Frame *frame) const {
            return StmtTree::notImpl;
        }

        virtual std::string toStr() const
        {
            return "break";
        }

    private:
        int location_;
};

///////////////////////////////////////////////////////////////////////////////
// A node type that evaluates to a numeric constant:
class NumASTNode : public ASTNode {
    public:
        NumASTNode(value_t value)
            : ASTNode(), value_(value)
        {}
        virtual ~NumASTNode() = default;

        virtual const Type *type_verify(Scope* scope) const {
            return Type::intType;
        }

        virtual value_t eval() const
        {
            return value_;
        }

        virtual std::string toStr() const
        {
            return std::to_string(value_);
        }

        virtual const IRTree *convert_to_ir(Frame *frame) const {
            return new MoveTree(new TempTree(new Temp()), new ConstTree(value_));
        }

    private:
        const value_t value_;
};


///////////////////////////////////////////////////////////////////////////////
// A node type that evaluates to a string constant:
class StrASTNode : public ASTNode {
    public:
        StrASTNode(std::string value)
            : ASTNode(), value_(value)
        {}
        virtual ~StrASTNode() = default;

        virtual const Type *type_verify(Scope* scope) const {
            return Type::stringType;
        }

        virtual std::string toStr() const
        {
            return std::string("\"") + value_ + "\"";
        }

        virtual value_t eval() const
        {
            return -1; /// FIX ME
        }

        virtual const IRTree *convert_to_ir(Frame *frame) const {
            return ExprTree::notImpl;
        }


    private:
        const std::string value_;
};


///////////////////////////////////////////////////////////////////////////////
// A node type that represents a name
class NameASTNode : public ASTNode {
    public:
        NameASTNode(std::string value, int location)
            : ASTNode(), value_(value), location_(location)
        {}
        virtual ~NameASTNode() = default;

        value_t eval() const {
            return -1; /// FIX ME
        }

        virtual const Type *type_verify(Scope* scope) const {
            const Type *var_type = scope->search(value_);
            if (var_type == Type::notFoundType) {
                cerr << "ERROR: line " << location_ << endl;
                cerr << "       unknown variable ‘" << value_ << "’" << endl;
                return Type::errorType;
            }
            return var_type;
        }

        virtual std::string toStr() const {
            return value_;
        }

        virtual const IRTree *convert_to_ir(Frame *frame) const {
            return new MoveTree(new TempTree(new Temp()), new NameTree(new Label(value_)));
        }


    private:
        const std::string value_;
        int location_;
};


///////////////////////////////////////////////////////////////////////////////
// A node type for a unary operator (e.g., generalizing  AbsASTNode).
// It's templated by another class O for the operator. Any instance of O has an
// operator() that takes a value of type value_t and returns another value of
// type value_t.
// See for example: http://en.cppreference.com/w/cpp/utility/functional/negate
template <template <typename> class O>
    class UnaryASTNode : public ASTNode {
        public:
            // Take a single child to evalute, and a string representation of the node:
            UnaryASTNode(std::string rep, ASTptr child, int loc)
                : ASTNode(), rep_(rep), child_(child), location_(loc)
            {}

            virtual ~UnaryASTNode()
            {
                delete child_;
            }

            virtual const Type *type_verify(Scope* scope) const {
                const Type *child_type = child_->type_verify(scope);
                if (child_type == Type::intType) {
                    cerr << "ERROR: line " << location_ << endl;
                    cerr << "       unary operation ‘" << rep_ << "’ applied to non-integer value ‘"
                        << child_->toStr() << "’" << endl;
                    return Type::errorType;
                }
                return Type::intType;
            }

            value_t eval() const
            {
                auto op = O<value_t>();
                return op(child_->eval());
            }

            virtual std::string toStr() const
            {
                return rep_ + child_->toStr();
            }

            virtual const IRTree *convert_to_ir(Frame *frame) const {
                return ExprTree::notImpl;
            }

        private:
            const std::string rep_;  // String representation of node
            const ASTptr child_;
            int location_;
    };

// Example usage of UnaryASTNode:
using UnaryMinusASTNode = UnaryASTNode<std::negate>;

//////////////////////////////////////////////////////////////////////////
// A unary AST node that doesn't evaluate its argument when calling eval.
template <class O>
    class NoEvalUnaryASTNode : public ASTNode {
        public:
            // Take a single child to evalute, and a string representation of the node:
            NoEvalUnaryASTNode(std::string rep, ASTptr child, int loc)
                : ASTNode(), rep1_(rep), rep2_(""), child_(child), location_(loc)
            {}

            NoEvalUnaryASTNode(std::string rep1, std::string rep2, ASTptr child, int loc)
                : ASTNode(), rep1_(rep1), rep2_(rep2), child_(child), location_(loc)
            {}

            virtual ~NoEvalUnaryASTNode()
            {
                delete child_;
            }

            virtual const Type *type_verify(Scope* scope) const {
                auto op = O();
                return op.type_verify(scope, child_, location_);
            }

            value_t eval() const
            {
                auto op = O();
                return op(child_);
            }

            virtual std::string toStr() const
            {
                return rep1_ + child_->toStr() + rep2_;
            }

            virtual const IRTree *convert_to_ir(Frame *frame) const {
                return ExprTree::notImpl;
            }

        private:
            const std::string rep1_, rep2_;  // String representation of node
            const ASTptr child_;
            int location_;
    };

///////////////////////////////////////////////////////////////////////////////
// A node type for binary operators.
// It's templated by another class O for the operator. Any instance of O has an
// operator() that takes two values of type value_t and returns another value
// of type value_t.
// See for example: http://en.cppreference.com/w/cpp/utility/functional/plus
template <template <typename> class O>
    class BinaryASTNode : public ASTNode {
        public:
            BinaryASTNode(std::string rep, ASTptr left, ASTptr right, int loc)
                : ASTNode(), rep1_(""), rep2_(rep), left_(left), right_(right), location_(loc)
            {}

            BinaryASTNode(std::string rep1, std::string rep2, ASTptr left, ASTptr right, int loc)
                : ASTNode(), rep1_(rep1), rep2_(rep2), left_(left), right_(right), location_(loc)
            {}

            virtual ~BinaryASTNode() {
                delete left_;
                delete right_;
            }

            virtual const Type *type_verify(Scope* scope) const {
                const Type *left_type = left_->type_verify(scope);
                const Type *right_type = right_->type_verify(scope);
                if (left_type == Type::intType && right_type == Type::intType) {
                    return Type::intType;
                } else if (left_type == Type::errorType || right_type == Type::errorType) {
                    /* this means the error was further down below, so we just pass
                     * it up again */
                    return Type::errorType;
                } else {
                    cerr << "ERROR: line " << location_ << endl;
                    cerr << "       in expression " << toStr() << endl;
                    cerr << "       Attempting binary operation on between 1 or more non-integer values" << endl;
                    cerr << "       (‘" << left_->toStr() << "’ is of type ‘" << left_type->toStr() << "’ and ‘"
                        << right_->toStr() << "’ is of type ‘"<< right_type->toStr() << "’)" << endl;
                    return Type::errorType;
                }
            }

            value_t eval() const {
                auto op = O<value_t>();
                return op(left_->eval(), right_->eval());
            }

            virtual std::string toStr() const {
                if (rep1_.length() == 0) {
                    return  "(" + left_->toStr() +
                        " " + rep2_ + " " +
                        right_->toStr() + ")";
                } else {
                    return  "(" + rep1_ + " " + left_->toStr() +
                        " " + rep2_ + " " +
                        right_->toStr() + ")";
                }
            }

            virtual const IRTree *convert_to_ir(Frame *frame) const {
                return ExprTree::notImpl;
            }

        private:
            const std::string rep1_, rep2_;  // String representation of node
            const ASTptr left_, right_;
            int location_;
    };


using PlusASTNode = BinaryASTNode<std::plus>;
using MinusASTNode = BinaryASTNode<std::minus>;
using MultASTNode = BinaryASTNode<std::multiplies>;
using DivASTNode = BinaryASTNode<std::divides>;
using EqualASTNode = BinaryASTNode<std::equal_to>;
using NotEqualASTNode = BinaryASTNode<std::not_equal_to>;
using GreaterASTNode = BinaryASTNode<std::greater>;
using LessASTNode = BinaryASTNode<std::less>;
using LessEqualASTNode = BinaryASTNode<std::less_equal>;
using GreaterEqualASTNode = BinaryASTNode<std::greater_equal>;
using LogicalAndASTNode = BinaryASTNode<std::logical_and>;
using LogicalOrASTNode = BinaryASTNode<std::logical_or>;


///////////////////////////////////////////////////////////////////////////////
// A node type for things that have two children but don't evaluate before
// they get called (e.g. while loops)
// It's templated by another class O for the operator. Any instance of O has an
// operator() that takes two values of type ASTptr and returns a value
// of type value_t.
// See for example: http://en.cppreference.com/w/cpp/utility/functional/plus
template <class O>
    class NoEvalBinaryASTNode : public ASTNode {
        public:
            // parens flag = whether to surround with parens and insert spaces around operators
            NoEvalBinaryASTNode(std::string rep, ASTptr left, ASTptr right, int loc, bool parens=true)
                : ASTNode(), rep1_(""), rep2_(rep), left_(left), right_(right), parens_(parens), location_(loc)
            {}

            NoEvalBinaryASTNode(std::string rep1, std::string rep2, ASTptr left, ASTptr right, int loc, bool parens=true)
                : ASTNode(), rep1_(rep1), rep2_(rep2), left_(left), right_(right), parens_(parens), location_(loc)
            {}

            NoEvalBinaryASTNode(std::string rep1, std::string rep2, std::string rep3, ASTptr left, ASTptr right, int loc, bool parens=true)
                : ASTNode(), rep1_(rep1), rep2_(rep2), rep3_(rep3), left_(left), right_(right), parens_(parens), location_(loc)
            {}

            virtual ~NoEvalBinaryASTNode()
            {
                delete left_;
                delete right_;
            }

            value_t eval() const
            {
                auto op = O();
                return op(left_, right_);
            }

            virtual const Type *type_verify(Scope* scope) const {
                auto op = O();
                return op.type_verify(scope, left_, right_, location_);
            }

            virtual std::string toStr() const
            {
                std::stringstream ss;
                if (parens_) {
                    ss << "(";
                }
                if (rep1_.length() != 0) {
                    ss << rep1_;
                    if (parens_) {
                        ss << " ";
                    }
                }
                ss << left_->toStr();
                if (parens_) {
                    ss << " ";
                }
                if (rep2_.length() != 0) {
                    ss << rep2_;
                    if (parens_) {
                        ss << " ";
                    }
                }
                ss << right_->toStr();
                if (rep3_.length() != 0) {
                    if (parens_) {
                        ss << " ";
                    }
                    ss << rep3_;
                }
                if (parens_) {
                    ss << ")";
                }
                return ss.str();
            }

            virtual const IRTree *convert_to_ir(Frame *frame) const {
                auto op = O();
                return op.convert_to_ir(frame, left_, right_);
            }

        private:
            const std::string rep1_, rep2_, rep3_;  // String representation of node (rep3 goes at end)
            const ASTptr left_, right_;
            const bool parens_;
            int location_;
    };

//////////////////////////////////////////////////////////////////////////////
// A node type for tertiary operators.
// (i.e. if-then-else)
template <class O>
    class TertiaryASTNode : public ASTNode {
        public:
            TertiaryASTNode(std::string rep1, std::string rep2, std::string rep3, std::string rep4,
                    ASTptr left, ASTptr middle, ASTptr right, int loc, bool parens=true)
                : ASTNode(), rep1_(rep1), rep2_(rep2), rep3_(rep3), rep4_(rep4), left_(left),
                middle_(middle), right_(right), parens_(parens), location_(loc)
        {}

            TertiaryASTNode(std::string rep1, std::string rep2, std::string rep3,
                    ASTptr left, ASTptr middle, ASTptr right, int loc, bool parens=true)
                : ASTNode(), rep1_(rep1), rep2_(rep2), rep3_(rep3), rep4_(""), left_(left),
                middle_(middle), right_(right), parens_(parens), location_(loc)
        {}

            TertiaryASTNode(std::string rep1, std::string rep2, ASTptr left, ASTptr middle, ASTptr right,
                    int loc, bool parens=true)
                : ASTNode(), rep1_(rep1), rep2_(rep2), rep3_(""), rep4_(""), left_(left), middle_(middle),
                right_(right), parens_(parens), location_(loc)
        {}

            virtual ~TertiaryASTNode()
            {
                delete left_;
                delete middle_;
                delete right_;
            }

            virtual const Type *type_verify(Scope* scope) const {
                auto op = O();
                return op.type_verify(scope, left_, middle_, right_, location_);
            }

            value_t eval() const
            {
                auto op = O();
                return op(left_, middle_, right_);
            }

            virtual std::string toStr() const
            {
                std::stringstream ss;
                if (parens_) {
                    ss << "(";
                }
                if (rep1_.length() != 0) {
                    ss << rep1_;
                    if (parens_) {
                        ss << " ";
                    }
                }
                ss << left_->toStr();
                if (parens_) {
                    ss << " ";
                }
                if (rep2_.length() != 0) {
                    ss << rep2_;
                    if (parens_) {
                        ss << " ";
                    }
                }
                ss << middle_->toStr();
                if (rep3_.length() != 0) {
                    if (parens_) {
                        ss << " ";
                    }
                    ss << rep3_;
                    if (parens_) {
                        ss << " ";
                    }
                }
                ss << right_->toStr();
                if (rep4_.length() != 0) {
                    if (parens_) {
                        ss << " ";
                    }
                    ss << rep4_;
                }
                if (parens_) {
                    ss << ")";
                }
                return ss.str();
            }

            virtual const IRTree *convert_to_ir(Frame *frame) const {
                return ExprTree::notImpl;
            }


        private:
            const std::string rep1_, rep2_, rep3_, rep4_;  // String representation of node
            const ASTptr left_, middle_, right_;
            bool parens_;
            int location_;
    };


//////////////////////////////////////////////////////////////////////////////
// A node type for tertiary operators.
// (i.e. if-then-else)
template <class O>
    class QuaternaryASTNode : public ASTNode {
        public:
            QuaternaryASTNode(std::string rep1, std::string rep2, std::string rep3, std::string rep4,
                    ASTptr one, ASTptr two, ASTptr three, ASTptr four, int loc, bool parens=true)
                : ASTNode(), rep1_(rep1), rep2_(rep2), rep3_(rep3), rep4_(rep4), rep5_(""),
                one_(one), two_(two), three_(three), four_(four), parens_(parens), location_(loc)
        {}

            QuaternaryASTNode(std::string rep1, std::string rep2, std::string rep3, std::string rep4, std::string rep5,
                    ASTptr one, ASTptr two, ASTptr three, ASTptr four, int loc, bool parens=true)
                : ASTNode(), rep1_(rep1), rep2_(rep2), rep3_(rep3), rep4_(rep4), rep5_(rep5),
                one_(one), two_(two), three_(three), four_(four), parens_(parens), location_(loc)
        {}

            virtual ~QuaternaryASTNode()
            {
                delete one_;
                delete two_;
                delete three_;
                delete four_;
            }

            virtual const Type *type_verify(Scope* scope) const {
                auto op = O();
                return op.type_verify(scope, one_, two_, three_, four_, location_);
            }

            value_t eval() const
            {
                auto op = O();
                return op(one_, two_, three_, four_);
            }

            virtual std::string toStr() const
            {
                std::stringstream ss;
                if (parens_) {
                    ss << "(";
                }
                if (rep1_.length() != 0) {
                    ss << rep1_;
                    if (parens_) {
                        ss << " ";
                    }
                }
                ss << one_->toStr();
                if (rep2_.length() != 0) {
                    if (parens_) {
                        ss << " ";
                    }
                    ss << rep2_;
                    if (parens_) {
                        ss << " ";
                    }
                }
                ss << two_->toStr();
                if (rep3_.length() != 0) {
                    if (parens_) {
                        ss << " ";
                    }
                    ss << rep3_;
                    if (parens_) {
                        ss << " ";
                    }
                }
                ss << three_->toStr();
                if (rep4_.length() != 0) {
                    if (parens_) {
                        ss << " ";
                    }
                    ss << rep4_;
                    if (parens_) {
                        ss << " ";
                    }
                }
                ss << four_->toStr();
                if (rep5_.length() != 0) {
                    if (parens_) {
                        ss << " ";
                    }
                    ss << rep5_;
                }
                if (parens_) {
                    ss << ")";
                }
                return ss.str();
                return  "(" + rep1_ +
                    " " + one_->toStr() +
                    " " + rep2_ +
                    " " + two_->toStr() +
                    " " + rep3_ +
                    " " + three_->toStr() +
                    " " + rep4_ +
                    " " + four_->toStr() + ")";
            }

            virtual const IRTree *convert_to_ir(Frame *frame) const {
                return ExprTree::notImpl;
            }


        private:
            const std::string rep1_, rep2_, rep3_, rep4_, rep5_;  // String representation of node
            const ASTptr one_, two_, three_, four_;
            bool parens_; // whether to surround with parentheses and put spaces around the 'rep' strings
            int location_;
    };


//////////////////////////////////////////////////////////////////////////////
// A node type for vectors of AST nodes.
// (e.g. declaration lists, parameter lists, statement sequences, ....)
template <class O, class E> // E is the elements of the vector (?)
    class VectorASTNode : public ASTNode {
        public:
            using VASTptr = const VectorASTNode*; // Can't use smart ptr in union :(

            VectorASTNode(std::string separator, int loc)
                : ASTNode(), sep_(separator), first_(""), last_(""), location_(loc)
            {}

            VectorASTNode(std::string separator, std::string first, std::string last, int loc)
                : ASTNode(), sep_(separator), first_(first), last_(last), location_(loc)
            {}

            virtual ~VectorASTNode() {
                for (auto a : vec_) {
                    delete a;
                }
            }

            virtual const Type *type_verify(Scope* scope) const {
                auto op = O();
                return op.type_verify(scope, vec_, location_);
            }

            value_t eval() const
            {
                auto op = O();
                return op(vec_);
            }

            void add_node(const E* node) {
                vec_.push_back(node);
            }

            virtual std::string toStr() const
            {
                std::stringstream ss;
                if (first_.length() != 0) {
                    ss << first_;
                }
                for (unsigned i = 0; i < vec_.size(); i++) {
                    if (i != 0) {
                        ss << sep_;
                    }
                    ss << vec_[i]->toStr();
                }
                if (last_.length() != 0) {
                    ss << last_;
                }
                return ss.str();
            }

            virtual const IRTree *convert_to_ir(Frame *frame) const {
                return ExprTree::notImpl;
            }

        private:
            std::vector<const E*> vec_;
            const std::string sep_, first_, last_;
            int location_;
    };


class Assignment {
    public:
        // TODO implement variable assignment
        ASTNode::value_t operator() (ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
            return -1;
        }

        const Type *type_verify(Scope* scope, ASTNode::ASTptr left_, ASTNode::ASTptr right_, int location_) {
            const Type *name_type = left_->type_verify(scope);
            const Type *value_type = right_->type_verify(scope);
            if (name_type == Type::notFoundType) {
                cerr << "ERROR: line " << location_ << endl;
                cerr << "       unknown variable ‘" << left_->toStr() << "’" << endl;
                return Type::errorType;
            }

            if (name_type == value_type && name_type != Type::errorType) {
                return value_type;
            } else if (name_type != Type::errorType && value_type != Type::errorType) {
                cerr << "ERROR: line " << location_ << endl;
                cerr << "       cannot assign expression of type ‘" << value_type->toStr() << "’ to variable ‘"
                    << left_->toStr() << "’, which is of type ‘" << name_type->toStr() << "’." << endl;
                return Type::errorType;
            } else {
                return Type::errorType;
            }
        }

        const ExprTree *convert_to_ir(Frame *frame, ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
            return ExprTree::notImpl;
        }
};

using AssignASTNode = NoEvalBinaryASTNode<Assignment>;


class IfThenElse {
    public:
        ASTNode::value_t operator() (ASTNode::ASTptr left_, ASTNode::ASTptr middle_, ASTNode::ASTptr right_) {
            if (left_->eval()) {
                return middle_->eval();
            } else if (right_ != NULL) {
                return right_->eval();
            } else {
                return -1; /// FIX ME
            }
        }

        const Type *type_verify(Scope* scope, ASTNode::ASTptr left_, ASTNode::ASTptr middle_, ASTNode::ASTptr right_, int location_);
};

using ConditionalASTNode = TertiaryASTNode<IfThenElse>;


class WhileDo {
    public:
        ASTNode::value_t operator() (ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
            ASTNode::value_t result = -1; // WHAT SHOULD IT RETURN
            while (left_->eval()) {
                result = right_->eval();
            }
            return result;
        }

        const Type *type_verify(Scope* scope, ASTNode::ASTptr left_, ASTNode::ASTptr right_, int location_);

        const ExprTree *convert_to_ir(Frame *frame, ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
            return ExprTree::notImpl;
        }
};

using WhileLoopASTNode = NoEvalBinaryASTNode<WhileDo>;


class ForTo {
    public:
        ASTNode::value_t operator() (ASTNode::ASTptr one_, ASTNode::ASTptr two_, ASTNode::ASTptr three_, ASTNode::ASTptr four_) {
            return -1;
        }

        const Type *type_verify(Scope* scope, ASTNode::ASTptr one_, ASTNode::ASTptr two_,
                                ASTNode::ASTptr three_, ASTNode::ASTptr four_, int location_);
};

using ForLoopASTNode = QuaternaryASTNode<ForTo>;


class UntypedVarDeclaration {
    public:
        ASTNode::value_t operator() (ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
            return -1;
        }

        const Type *type_verify(Scope* scope, ASTNode::ASTptr left_, ASTNode::ASTptr right_, int location_);

        const ExprTree *convert_to_ir(Frame *frame, ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
            return ExprTree::notImpl;
        }
};

using UntypedVarDeclASTNode = NoEvalBinaryASTNode<UntypedVarDeclaration>;


class TypedVarDeclaration {
    public:
        ASTNode::value_t operator() (ASTNode::ASTptr left_, ASTNode::ASTptr middle_, ASTNode::ASTptr right_) {
            return -1;
        }

        const Type *type_verify(Scope* scope, ASTNode::ASTptr left_, ASTNode::ASTptr middle_, ASTNode::ASTptr right_, int location_);
};

using TypedVarDeclASTNode = TertiaryASTNode<TypedVarDeclaration>;


class TypeDeclaration {
    public:
        ASTNode::value_t operator() (ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
            return -1;
        }

        const Type *type_verify(Scope* scope, ASTNode::ASTptr left_, ASTNode::ASTptr right_, int location_);

        const ExprTree *convert_to_ir(Frame *frame, ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
            return ExprTree::notImpl;
        }
};

using TypeDeclASTNode = NoEvalBinaryASTNode<TypeDeclaration>;

// e.g. let decl1 decl2 decl3 in exp1; exp2; exp3; end

class LetBlock {
    public:
        ASTNode::value_t operator() (ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
            return -1;
        }

        const Type *type_verify(Scope* scope, ASTNode::ASTptr left_, ASTNode::ASTptr right_, int location_);

        const ExprTree *convert_to_ir(Frame *frame, ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
            return ExprTree::notImpl;
        }
};

using LetASTNode = NoEvalBinaryASTNode<LetBlock>;

// any type of declaration -- var, func, or type

class Declaration {
    public:
        ASTNode::value_t operator() (ASTNode::ASTptr child_) {
            return -1;
        }

        const Type *type_verify(Scope* scope, ASTNode::ASTptr child_, int location_);
};

using DeclarationASTNode = NoEvalUnaryASTNode<Declaration>;

// a sequence of declarations (the first part of a let)

class DeclList {
    public:
        ASTNode::value_t operator() (std::vector<const DeclarationASTNode*> declarations) {
            return -1;
        }

        const Type *type_verify(Scope* scope, std::vector<const DeclarationASTNode*> vec_, int location_);
};

using DeclListASTNode = VectorASTNode<DeclList, DeclarationASTNode>;

// e.g. a; b; c; d; OR a, b, c, d

class ExprSeq {
    public:
        ASTNode::value_t operator() (std::vector<const ASTNode*> members) {
            return -1;
        }

        const Type *type_verify(Scope* scope, std::vector<const ASTNode*> vec_, int location_);
};

using ExprSeqASTNode = VectorASTNode<ExprSeq, ASTNode>;

// e.g. a = b

class FieldMember {
    public:
        ASTNode::value_t operator() (ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
            return -1;
        }

        const Type *type_verify(Scope* scope, ASTNode::ASTptr left_, ASTNode::ASTptr right_, int location_);

        const ExprTree *convert_to_ir(Frame *frame, ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
            return ExprTree::notImpl;
        }
};

using FieldMemberASTNode = NoEvalBinaryASTNode<FieldMember>;

// e.g. a = b, c = d (in a type instantiation inside curly brackets)

class FieldList {
    public:
        ASTNode::value_t operator() (std::vector<const FieldMemberASTNode*> members) {
            return -1;
        }

        const Type *type_verify(Scope* scope, std::vector<const FieldMemberASTNode*> vec_, int location_);
};

using FieldListASTNode = VectorASTNode<FieldList, FieldMemberASTNode>;

// e.g. typename { field1 = val1, field2 = val2 }

class TypeInstantiation {
    public:
        ASTNode::value_t operator() (ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
            return -1;
        }

        const Type *type_verify(Scope* scope, ASTNode::ASTptr left_, ASTNode::ASTptr right_, int location_);

        const ExprTree *convert_to_ir(Frame *frame, ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
            return ExprTree::notImpl;
        }
};

using TypeInstASTNode = NoEvalBinaryASTNode<TypeInstantiation>;

// the thing that goes on the right side of a type declaration

class TypeValue {
    public:
        ASTNode::value_t operator() (ASTNode::ASTptr child_) {
            return -1;
        }

        const Type *type_verify(Scope* scope, ASTNode::ASTptr child_, int location_);
};

using TypeASTNode = NoEvalUnaryASTNode<TypeValue>;

// one field in a record type, e.g. a : int

class RecordField {
    public:
        ASTNode::value_t operator() (ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
            return -1;
        }

        const Type *type_verify(Scope* scope, ASTNode::ASTptr left_, ASTNode::ASTptr right_, int location_);

        const ExprTree *convert_to_ir(Frame *frame, ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
            return ExprTree::notImpl;
        }
};

using RecordFieldASTNode = NoEvalBinaryASTNode<RecordField>;

// a record type -- i.e. a sequence of RecordFields

class RecordTypeAST {
    public:
        ASTNode::value_t operator() (std::vector<const RecordFieldASTNode*> fields) {
            return -1;
        }

        const Type *type_verify(Scope* scope, std::vector<const RecordFieldASTNode*> vec_, int location_);
};

using RecordTypeASTNode = VectorASTNode<RecordTypeAST, RecordFieldASTNode>;

// "array of" whatever type

class ArrayTypeAST {
    public:
        ASTNode::value_t operator() (ASTNode::ASTptr child_) {
            return -1;
        }

        const Type *type_verify(Scope* scope, ASTNode::ASTptr child_, int location_);
};

using ArrayTypeASTNode = NoEvalUnaryASTNode<ArrayTypeAST>;

// represents lvalue.identifier

class DotAccess {
    public:
        ASTNode::value_t operator() (ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
            return -1;
        }

        const Type *type_verify(Scope* scope, ASTNode::ASTptr left_, ASTNode::ASTptr right_, int location_);

        const ExprTree *convert_to_ir(Frame *frame, ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
            return ExprTree::notImpl;
        }
};

using DotASTNode = NoEvalBinaryASTNode<DotAccess>;

// represents array[index]

class IndexAccess {
    public:
        ASTNode::value_t operator() (ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
            return -1;
        }

        const Type *type_verify(Scope* scope, ASTNode::ASTptr left_, ASTNode::ASTptr right_, int location_);

        const ExprTree *convert_to_ir(Frame *frame, ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
            return ExprTree::notImpl;
        }
};

using IndexASTNode = NoEvalBinaryASTNode<IndexAccess>;

// represents type[length] of value

class ArrayValue {
    public:
        ASTNode::value_t operator() (ASTNode::ASTptr left_, ASTNode::ASTptr middle_, ASTNode::ASTptr right_) {
            return -1;
        }

        const Type *type_verify(Scope* scope, ASTNode::ASTptr left_, ASTNode::ASTptr middle_, ASTNode::ASTptr right_, int location_);
};

using ArrayASTNode = TertiaryASTNode<ArrayValue>;

// represents function declerations

class UnTypedFuncDecl {
    public:
        ASTNode::value_t operator() (ASTNode::ASTptr left_, ASTNode::ASTptr middle_, ASTNode::ASTptr right_) {
            return -1;
        }

        const Type *type_verify(Scope* scope, ASTNode::ASTptr left_, ASTNode::ASTptr middle_, ASTNode::ASTptr right_, int location_);
};

using UnTypedFuncDeclASTNode = TertiaryASTNode<UnTypedFuncDecl>;

// represents function declerations

class TypedFuncDecl {
    public:
        ASTNode::value_t operator() (ASTNode::ASTptr one_, ASTNode::ASTptr two_, ASTNode::ASTptr three_, ASTNode::ASTptr four_) {
            return -1;
        }

        const Type *type_verify(Scope* scope, ASTNode::ASTptr name_node, ASTNode::ASTptr params_node, ASTNode::ASTptr rettype_node, ASTNode::ASTptr funcbody_node, int location_);
};

using TypedFuncDeclASTNode = QuaternaryASTNode<TypedFuncDecl>;

// represents function calls

class FuncCall {
    public:
        ASTNode::value_t operator() (ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
            return -1;
        }

        const Type *type_verify(Scope* scope, ASTNode::ASTptr left_, ASTNode::ASTptr right_, int location_);

        const ExprTree *convert_to_ir(Frame *frame, ASTNode::ASTptr left_, ASTNode::ASTptr right_) {
            return ExprTree::notImpl;
        }
};

using FuncCallASTNode = NoEvalBinaryASTNode<FuncCall>;

} // namespace

#endif
